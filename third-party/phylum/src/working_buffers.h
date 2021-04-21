#pragma once

#if defined(__linux__)
#include <map>
#endif

#include "simple_buffer.h"

namespace phylum {

class working_buffers {
protected:
    struct page_t {
        uint8_t *buffer{ nullptr };
        size_t size{ 0 };
        dhara_sector_t sector{ InvalidSector };
        bool dirty{ false };
        int32_t refs{ 0 };
        int32_t hits{ 0 };
        uint32_t used{ 0 };
    };

    static constexpr size_t Size = 8;
    size_t buffer_size_{ 0 };
    page_t pages_[Size];
    size_t highwater_{ 0 };
    uint32_t counter_{ 0 };
    size_t reads_{ 0 };
    size_t writes_{ 0 };
    size_t misses_{ 0 };

#if defined(__linux__)
    struct sector_statistics_t {
        size_t reads{ 0 };
        size_t writes{ 0 };
        size_t misses{ 0 };
    };
    std::map<dhara_sector_t, sector_statistics_t> statistics_;
#endif

public:
    working_buffers(size_t buffer_size) : buffer_size_(buffer_size) {
        for (auto i = 0u; i < Size; ++i) {
            pages_[i] = { };
        }
    }

    virtual ~working_buffers() {
        phyinfof("wbuffers::dtor hw=%zu reads=%zu writes=%zu misses=%zu", highwater_, reads_, writes_, misses_);
        debug();
    }

public:
    size_t buffer_size() {
        return buffer_size_;
    }

public:
    void lend(uint8_t *ptr, size_t size) {
        assert(buffer_size_ == size);
        for (auto i = 0u; i < Size; ++i) {
            if (pages_[i].buffer == nullptr) {
                pages_[i].buffer = ptr;
                pages_[i].size = size;
                break;
            }
        }
    }

public:
    using miss_function_t = std::function<int32_t(dhara_sector_t, uint8_t *, size_t)>;
    using flush_function_t = std::function<int32_t(dhara_sector_t, uint8_t *, size_t)>;

    int32_t dirty_sector(dhara_sector_t sector) {
        auto err = -1;

        for (auto i = 0u; i < Size; ++i) {
            auto &p = pages_[i];
            if (p.buffer != nullptr) {
                if (p.sector == sector) {
                    phydebugf("wbuffers[%d] dirty sector=%d", i, sector);
                    p.dirty = true;
                    err = 0;
                }
            }
        }

        return err;
    }

    int32_t flush_sector(dhara_sector_t sector, flush_function_t flush) {
        auto flushed = false;

        for (auto i = 0u; i < Size; ++i) {
            auto &p = pages_[i];
            if (p.buffer != nullptr) {
                if (p.sector == sector) {
                    if (flushed) {
                        debug();
                        assert(!flushed);
                    }
                    if (!p.dirty) {
                        phywarnf("flush of clean page sector");
                    }

                    phydebugf("wbuffers[%d] flush sector=%d", i, sector);

                    auto err = flush(sector, p.buffer, buffer_size_);
                    if (err < 0) {
                        return err;
                    }

                    flushed = true;
                    p.dirty = false;
                    writes_++;
#if defined(__linux__)
                    statistics_[sector].writes++;
#endif
                }
            }
        }

        if (!flushed) {
            return -1;
        }

        return 0;
    }

    uint8_t *open_sector(dhara_sector_t sector, bool read_only, miss_function_t miss, flush_function_t flush) {
        auto selected = -1;
        auto flushing = -1;

        reads_++;
#if defined(__linux__)
        statistics_[sector].reads++;
#endif

        for (auto i = 0u; i < Size; ++i) {
            auto &p = pages_[i];
            if (p.buffer == nullptr) break;
            if (p.sector == InvalidSector) {
                // Free page.
            }
            if (p.sector == sector) {
                // Otherwise, this sector is open for writing as we
                // speak. Which should never happen.
                if (read_only && p.refs >= 0) {
                    assert(p.refs >= 0);
                    p.refs++;
                }
                else {
                    assert(p.refs <= 0);
                    p.refs--;
                }

                counter_++;

                p.used = counter_;
                p.hits++;

                phydebugf("wbuffers[%d]: reusing refs=%d", i, p.refs);

                if (false) {
                    phydebug_dump_memory("reuse[%d, sector=%d] ", p.buffer, buffer_size_, i, p.sector);
                }

                return p.buffer;
            }
            else {
                if (p.refs == 0) {
                    if (p.dirty) {
                        // TODO Check age
                        flushing = i;
                    }
                    else {
                        if (selected == -1)  {
                            selected = i;
                        }
                        else {
                            if (p.sector == InvalidSector && pages_[selected].sector != InvalidSector) {
                                selected = i;
                            }
                            else if (p.used < pages_[selected].used) {
                                selected = i;
                            }
                        }
                    }
                }
            }
        }

        if (selected < 0) {
            if (flushing) {
                phydebugf("wbuffers[%d]: flush-alloc", flushing);

                debug();

                auto &p = pages_[flushing];
                assert(p.refs == 0);

                auto err = flush(p.sector, p.buffer, buffer_size_);
                if (err < 0) {
                    assert(err >= 0);
                    return { };
                }

                p.dirty = false;
                p.sector = InvalidSector;
                writes_++;

                selected = flushing;
            }
            else {
                debug();
                assert(selected >= 0);
            }
        }
        else {
            phydebugf("wbuffers[%d]: allocating sector=%d", selected, sector);
        }

        // Load the sector.
        auto &p = pages_[selected];
        memset(p.buffer, 0xff, buffer_size_);
        auto err = miss(sector, p.buffer, buffer_size_);
        if (err < 0) {
            assert(err >= 0);
            return { };
        }

        // If we just zeroed the buffer, this will be zero otherwise
        // this'll have the number of bytes we read.
        if (err > 0) {
            misses_++;
#if defined(__linux__)
            statistics_[sector].misses++;
#endif
        }

        if (read_only) {
            p.refs++;
        }
        else {
            p.refs--;
        }

        counter_++;

        p.sector = sector;
        p.used = counter_;
        p.hits = 0;

        if (false) {
            phydebug_dump_memory("alloc[%d, sector=%d] ", p.buffer, buffer_size_, selected, sector);
        }

        update_highwater();

        return p.buffer;
    }

    int32_t debug() {
        for (auto i = 0u; i < Size; ++i) {
            auto &p = pages_[i];
            if (p.buffer != nullptr) {
                phydebugf("wbuffers[%d] sector=%d dirty=%d refs=%d hits=%d used=%d", i, p.sector, p.dirty, p.refs, p.hits, p.used);
            }
        }

#if defined(__linux__)
        if (false) {
            for (auto i : statistics_) {
                phydebugf("wbuffers[-] sector=%d reads=%zu writes=%zu misses=%zu",
                        i.first, i.second.reads, i.second.writes, i.second.misses);
            }
        }
#endif

        return 0;
    }

    int32_t update_highwater() {
        auto hw = 0u;

        for (auto i = 0u; i < Size; ++i) {
            if (pages_[i].buffer == nullptr) break;
            if (pages_[i].refs != 0) {
                hw++;
            }
        }

        if (hw > highwater_) {
            highwater_ = hw;
        }

        return 0;
    }

    simple_buffer allocate(size_t size) {
        assert(size == buffer_size_);

        update_highwater();

        auto selected = -1;

        for (auto i = 0u; i < Size; ++i) {
            auto &p = pages_[i];
            if (p.buffer == nullptr) break;
            if (p.refs == 0) {
                if (selected < 0) {
                    selected = i;
                }
                else {
                    if (pages_[selected].sector != InvalidSector && p.sector == InvalidSector) {
                        selected = i;
                    }
                }
            }
        }

        assert(selected >= 0);

        counter_++;

        auto &p = pages_[selected];
        p.used = counter_;
        p.sector = InvalidSector;
        p.hits = 0;
        p.refs--;

        update_highwater();

        phydebugf("wbuffers[%d]: allocate sector=%d hw=%zu", selected, p.sector, highwater_);
        auto free_fn = std::bind(&working_buffers::free, this, std::placeholders::_1);
        return simple_buffer{ p.buffer, size, free_fn };
    }

    void free(uint8_t const *ptr) {
        assert(ptr != nullptr);
        for (auto i = 0u; i < Size; ++i) {
            auto &p = pages_[i];
            if (p.buffer == ptr) {
                phydebugf("wbuffers[%d]: free refs-before=%d sector=%d", i, p.refs, p.sector);

                assert(p.refs != 0);

                if (p.refs > 0) {
                    p.refs--;
                }
                else {
                    p.refs++;
                }

                if (false) {
                    phydebug_dump_memory("free[%d, sector=%d] ", p.buffer, buffer_size_, i, p.sector);
                }
                break;
            }
        }
    }

};

class malloc_working_buffers : public working_buffers {
private:
    uint8_t *memory_{ nullptr };

public:
    malloc_working_buffers(size_t buffer_size) : working_buffers(buffer_size) {
        memory_ = (uint8_t *)malloc(Size * buffer_size);
        memset(memory_, 0xff, Size * buffer_size);
        for (auto i = 0u; i < Size; ++i) {
            this->lend(memory_ + (i * buffer_size), buffer_size);
        }
    }

    virtual ~malloc_working_buffers() {
        ::free(memory_);
    }
};

} // namespace phylum
