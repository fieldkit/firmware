#pragma once

namespace phylum {

class sector_geometry {
private:
    working_buffers &buffers_;
    sector_map &map_;

public:
    sector_geometry(working_buffers &buffers, sector_map &map) : buffers_(buffers), map_(map) {
    }

public:
    class verify_sector {
    private:
        dhara_sector_t sector_;
        delimited_buffer buffer_;

    public:
        verify_sector(working_buffers &buffers, sector_map &map, dhara_sector_t sector) : sector_(sector), buffer_(std::move(buffers.allocate(map.sector_size()))) {
            buffer_.unsafe_all([&](uint8_t *ptr, size_t size) {
                auto err = map.read(sector_, ptr, size);
                // phydebug_dump_memory("verify ", ptr, size);
                return err;
            });
        }

    public:
        testing::AssertionResult end(size_t n) {
            auto iter = buffer_.begin();
            while (n > 0) {
                if (iter == buffer_.end()) {
                    return testing::AssertionFailure() << "unexpected end of records";
                }

                ++iter;
                --n;
            }

            // This is awkward because there's two ways to end, one
            // includes a NULL terminator, 0-length record and the
            // other is unparseable length.
            if (iter.size_of_record() <= 0) {
                return testing::AssertionSuccess();
            }
            return testing::AssertionFailure() << "too many records";
        }

        template <typename T>
        testing::AssertionResult nth(size_t n, T expected) {
            return nth<T>(n, expected, nullptr, 0);
        }

        template <typename T>
        testing::AssertionResult nth(size_t n, T expected, uint32_t data) {
            return nth<T>(n, expected, (uint8_t *)&data, sizeof(uint32_t));
        }

        template <typename T>
        testing::AssertionResult nth(size_t n, T expected, uint8_t const *expected_payload, size_t payload_size) {
            auto iter = buffer_.begin();
            while (n > 0) {
                if (iter == buffer_.end()) {
                    return testing::AssertionFailure() << "unexpected end of records";
                }
                ++iter;
                --n;
            }
            if (iter == buffer_.end()) {
                return testing::AssertionFailure() << "unexpected end of records";
            }

            auto rp = *iter;
            auto actual = rp.as<T>();
            if (memcmp(&expected, actual, sizeof(T)) == 0) {
                if (payload_size > 0) {
                    auto err = rp.read_data<T>([&](auto data_buffer) {
                        auto ptr = data_buffer.cursor();
                        auto size = data_buffer.available();
                        if (memcmp(expected_payload, ptr, size) == 0) {
                            return 0;
                        }
                        phydebug_dump_memory("payload-actual   ", (uint8_t *)ptr, size);
                        phydebug_dump_memory("payload-expected ", (uint8_t *)expected_payload, payload_size);
                        return -1;
                    });
                    if (err < 0) {
                        return testing::AssertionFailure() << "unexpected record payload";
                    }
                    return testing::AssertionSuccess();
                }
                return testing::AssertionSuccess();
            }
            phydebug_dump_memory("actual   ", (uint8_t *)actual, sizeof(T));
            phydebug_dump_memory("expected ", (uint8_t *)&expected, sizeof(T));
            return testing::AssertionFailure() << "unexpected record data";
        }

        template <typename T> testing::AssertionResult header(T expected) {
            return nth(0, expected);
        }
    };

public:
    verify_sector sector(dhara_sector_t sector) {
        return verify_sector{ buffers_, map_, sector };
    }
};

} // namespace phylum
