#include "string_format.h"

#include <directory_chain.h>
#include <directory_tree.h>
#include <file_appender.h>
#include <file_reader.h>
#include <super_chain.h>

#include "phylum_tests.h"
#include "geometry.h"

using namespace phylum;

template <typename T> class IndexedFixture : public PhylumFixture {};

template<typename directory_type, typename tree_type>
class file_ops {
private:
    phyctx pc_;
    super_chain &sc_;
    directory_type dir_;

public:
    file_ops(phyctx pc, super_chain &sc) : pc_(pc), sc_(sc), dir_{ pc, sc.directory_tree() } {
    }

public:
    phyctx &pc() {
        return pc_;
    }

    directory_type &dir() {
        return dir_;
    }

public:
    int32_t mount() {
        return 0;
    }

    int32_t format() {
        auto err = dir_.format();
        if (err < 0) {
            return err;
        }

        err = sc_.update(dir_.to_tree_ptr());
        if (err < 0) {
            return err;
        }

        return 0;
    }

    int32_t touch(const char *name) {
        auto err = dir_.template touch_indexed<tree_type>(name);
        if (err < 0) {
            return err;
        }

        err = sc_.update(dir_.to_tree_ptr());
        if (err < 0) {
            return err;
        }

        return 0;
    }

    int32_t index_if_necessary(file_appender &appender, record_number_t record_number) {
        return appender.index_if_necessary<tree_type>(record_number);
    }

    int32_t seek_position(file_reader &reader, file_size_t position) {
        auto err = reader.seek_position<tree_type>(position);
        if (err < 0) {
            return err;
        }

        return err;
    }

    int32_t seek_record(file_reader &reader, record_number_t record) {
        auto err = reader.seek_record<tree_type>(record);
        if (err < 0) {
            return err;
        }

        return err;
    }

};

template<typename TLayout, typename TDirectory, typename TTree>
struct test_types {
    using layout_type = TLayout;
    using directory_type = TDirectory;
    using tree_type = TTree;
};

typedef ::testing::Types<
    test_types<layout_4096, directory_tree, tree_sector<uint32_t, uint32_t, 63>>,
    test_types<layout_4096, directory_tree, tree_sector<uint32_t, uint32_t, 405>>>
    Implementations;

TYPED_TEST_SUITE(IndexedFixture, Implementations);

TYPED_TEST(IndexedFixture, TouchedIndexed) {
    using layout_type = typename TypeParam::layout_type;
    using directory_type = typename TypeParam::directory_type;
    using tree_type = typename TypeParam::tree_type;
    using file_ops_type = file_ops<directory_type, tree_type>;

    layout_type layout;
    FlashMemory memory{ layout.sector_size };

    memory.mounted<super_chain>([&](super_chain &super) {
        file_ops_type fops{ memory.pc(), super };
        ASSERT_EQ(fops.format(), 0);
        ASSERT_EQ(fops.touch("data.txt"), 0);
    });
}

TYPED_TEST(IndexedFixture, TouchedIndexed_10) {
    using layout_type = typename TypeParam::layout_type;
    using directory_type = typename TypeParam::directory_type;
    using tree_type = typename TypeParam::tree_type;
    using file_ops_type = file_ops<directory_type, tree_type>;

    layout_type layout;
    FlashMemory memory{ layout.sector_size };

    std::vector<std::string> files;

    memory.mounted<super_chain>([&](super_chain &super) {
        file_ops_type fops{ memory.pc(), super };
        ASSERT_EQ(fops.format(), 0);
    });

    for (auto i = 0; i < 10; ++i) {
        memory.mounted<super_chain>([&](super_chain &super) {
            std::string name = string_format("data-%d.txt", i);
            file_ops_type fops{ memory.pc(), super };
            ASSERT_EQ(fops.touch(name.c_str()), 0);
            files.push_back(name);
        });
    }

    memory.mounted<super_chain>([&](super_chain &super) {
        directory_type dir{ memory.pc(), super.directory_tree() };

        for (auto name : files) {
            ASSERT_EQ(dir.template find(name.c_str(), open_file_config{ }), 1);
        }
    });
}

TYPED_TEST(IndexedFixture, TouchedIndexed_100_FindAfterEach) {
    using layout_type = typename TypeParam::layout_type;
    using directory_type = typename TypeParam::directory_type;
    using tree_type = typename TypeParam::tree_type;
    using file_ops_type = file_ops<directory_type, tree_type>;

    layout_type layout;
    FlashMemory memory{ layout.sector_size };

    memory.mounted<super_chain>([&](super_chain &super) {
        file_ops_type fops{ memory.pc(), super };
        ASSERT_EQ(fops.format(), 0);
    });

    std::vector<std::string> files;

    for (auto i = 0; i < 100; ++i) {
        std::string name = string_format("data-%d.txt", i);

        phydebugf("touching %s", name.c_str());

        auto pass = true;

        memory.mounted<super_chain>([&](super_chain &super) {
            file_ops_type fops{ memory.pc(), super };
            ASSERT_EQ(fops.touch(name.c_str()), 0);
            ASSERT_EQ(fops.dir().find(name.c_str(), open_file_config{ }), 1);
        });

        memory.mounted<super_chain>([&](super_chain &super) {
            file_ops_type fops{ memory.pc(), super };
            ASSERT_EQ(fops.dir().find(name.c_str(), open_file_config{ }), 1);
        });

        ASSERT_TRUE(pass);

        files.push_back(name);
    }
}

TYPED_TEST(IndexedFixture, TouchedIndexed_100) {
    using layout_type = typename TypeParam::layout_type;
    using directory_type = typename TypeParam::directory_type;
    using tree_type = typename TypeParam::tree_type;
    using file_ops_type = file_ops<directory_type, tree_type>;

    layout_type layout;
    FlashMemory memory{ layout.sector_size };

    memory.mounted<super_chain>([&](super_chain &super) {
        file_ops_type fops{ memory.pc(), super };
        ASSERT_EQ(fops.format(), 0);
    });

    std::vector<std::string> files;

    for (auto i = 0; i < 100; ++i) {
        std::string name = string_format("data-%d.txt", i);
        phydebugf("touching %s", name.c_str());
        memory.mounted<super_chain>([&](super_chain &super) {
            file_ops_type fops{ memory.pc(), super };
            ASSERT_EQ(fops.touch(name.c_str()), 0);
            memory.buffers().debug();
        });
        files.push_back(name);
    }

    memory.mounted<super_chain>([&](super_chain &super) {
        directory_type dir{ memory.pc(), super.directory_tree() };
        for (auto name : files) {
            ASSERT_EQ(dir.find(name.c_str(), open_file_config{ }), 1);
        }
    });
}

template<typename file_ops_type>
static void write_large_file(file_ops_type &fops, size_t total_bytes, size_t &written, record_number_t &records) {
    phydebugf("suppressing 1MB of writes from debug");
    suppress_logs sl;

    file_appender opened{ fops.pc(), &fops.dir(), fops.dir().open() };

    while (written < total_bytes) {
        ASSERT_GE(fops.index_if_necessary(opened, records), 0);
        auto wrote = opened.write(lorem1k);
        ASSERT_GT(wrote, 0);
        written += wrote;
        records++;
    }

    ASSERT_EQ(opened.flush(), 0);
}

template <typename file_ops_type>
static void write_large_file_delimited(file_ops_type &fops, size_t total_bytes, size_t &written, record_number_t &records) {
    // phydebugf("suppressing 1MB of writes from debug %zu", strlen(lorem1k));
    // suppress_logs sl;

    file_appender opened{ fops.pc(), &fops.dir(), fops.dir().open() };

    while (written < total_bytes) {
        ASSERT_GE(fops.index_if_necessary(opened, records), 0);
        phydebugf("writing record=%d position=%d", records, written);

        ASSERT_EQ(opened.write_delimiter(strlen(lorem1k)), 2);
        written += 2;

        auto wrote = opened.write(lorem1k);
        ASSERT_GT(wrote, 0);
        written += wrote;
        records++;
    }

    ASSERT_EQ(opened.flush(), 0);
}

TYPED_TEST(IndexedFixture, WriteFile_Large) {
    using layout_type = typename TypeParam::layout_type;
    using directory_type = typename TypeParam::directory_type;
    using tree_type = typename TypeParam::tree_type;
    using file_ops_type = file_ops<directory_type, tree_type>;

    layout_type layout;
    FlashMemory memory{ layout.sector_size };

    memory.mounted<super_chain>([&](super_chain &super) {
        file_ops_type fops{ memory.pc(), super };
        ASSERT_EQ(fops.format(), 0);
        ASSERT_EQ(fops.touch("data.txt"), 0);

        ASSERT_EQ(fops.dir().find("data.txt", open_file_config{ }), 1);

        record_number_t record_number;
        size_t written = 0u;
        write_large_file(fops, 1024u * 1024u, written, record_number);
    });
}

TYPED_TEST(IndexedFixture, WriteFile_Large_Position_SeekBeginningAndEnd) {
    using layout_type = typename TypeParam::layout_type;
    using directory_type = typename TypeParam::directory_type;
    using tree_type = typename TypeParam::tree_type;
    using file_ops_type = file_ops<directory_type, tree_type>;

    layout_type layout;
    FlashMemory memory{ layout.sector_size };

    record_number_t record_number = 0;
    size_t written = 0u;

    memory.mounted<super_chain>([&](super_chain &super) {
        file_ops_type fops{ memory.pc(), super };
        ASSERT_EQ(fops.format(), 0);
        ASSERT_EQ(fops.touch("data.txt"), 0);

        ASSERT_EQ(fops.dir().find("data.txt", open_file_config{ }), 1);

        write_large_file(fops, 1024u * 1024u, written, record_number);
    });

    memory.mounted<super_chain>([&](super_chain &super) {
        file_ops_type fops{ memory.pc(), super };

        ASSERT_EQ(fops.dir().find("data.txt", open_file_config{ }), 1);
        file_reader reader{ memory.pc(), &fops.dir(), fops.dir().open() };

        ASSERT_EQ(fops.seek_position(reader, 0), 0);

        ASSERT_EQ(reader.position(), (file_size_t)0);

        ASSERT_EQ(fops.seek_position(reader, UINT32_MAX), (int32_t)written);

        ASSERT_EQ(reader.position(), (file_size_t)written);
    });
}

TYPED_TEST(IndexedFixture, WriteFile_OneIndex_Records_SeekBeginningAndEnd) {
    using layout_type = typename TypeParam::layout_type;
    using directory_type = typename TypeParam::directory_type;
    using tree_type = typename TypeParam::tree_type;
    using file_ops_type = file_ops<directory_type, tree_type>;

    layout_type layout;
    FlashMemory memory{ layout.sector_size };

    record_number_t record_number = 0;
    size_t written = 0u;

    memory.mounted<super_chain>([&](super_chain &super) {
        file_ops_type fops{ memory.pc(), super };
        ASSERT_EQ(fops.format(), 0);
        ASSERT_EQ(fops.touch("data.txt"), 0);

        ASSERT_EQ(fops.dir().find("data.txt", open_file_config{ }), 1);

        write_large_file_delimited(fops, 1024u * 65u, written, record_number);

        // TODO Verify we actually indexed.
    });

    memory.mounted<super_chain>([&](super_chain &super) {
        file_ops_type fops{ memory.pc(), super };

        ASSERT_EQ(fops.dir().find("data.txt", open_file_config{ }), 1);
        file_reader reader{ memory.pc(), &fops.dir(), fops.dir().open() };

        ASSERT_EQ(fops.seek_record(reader, 0), 0);

        ASSERT_EQ(reader.position(), 0u);

        ASSERT_EQ(fops.seek_record(reader, UINT32_MAX), (int32_t)record_number);

        ASSERT_EQ(reader.position(), written);
    });
}

TYPED_TEST(IndexedFixture, WriteFile_Large_Records_SeekBeginningAndEnd) {
    using layout_type = typename TypeParam::layout_type;
    using directory_type = typename TypeParam::directory_type;
    using tree_type = typename TypeParam::tree_type;
    using file_ops_type = file_ops<directory_type, tree_type>;

    layout_type layout;
    FlashMemory memory{ layout.sector_size };

    record_number_t record_number = 0;
    size_t written = 0u;

    memory.mounted<super_chain>([&](super_chain &super) {
        file_ops_type fops{ memory.pc(), super };
        ASSERT_EQ(fops.format(), 0);
        ASSERT_EQ(fops.touch("data.txt"), 0);

        ASSERT_EQ(fops.dir().find("data.txt", open_file_config{ }), 1);

        write_large_file_delimited(fops, 1024u * 1024u, written, record_number);
    });

    memory.mounted<super_chain>([&](super_chain &super) {
        file_ops_type fops{ memory.pc(), super };

        ASSERT_EQ(fops.dir().find("data.txt", open_file_config{ }), 1);
        file_reader reader{ memory.pc(), &fops.dir(), fops.dir().open() };

        ASSERT_EQ(fops.seek_record(reader, 0), 0);

        ASSERT_EQ(reader.position(), 0u);

        ASSERT_EQ(fops.seek_record(reader, UINT32_MAX), (int32_t)record_number);

        ASSERT_EQ(reader.position(), written);
    });
}

TYPED_TEST(IndexedFixture, WriteFile_CreatingBeforeWriting_ReproduceOverwrittenTerminator) {
    using layout_type = typename TypeParam::layout_type;
    using directory_type = typename TypeParam::directory_type;
    using tree_type = typename TypeParam::tree_type;
    using file_ops_type = file_ops<directory_type, tree_type>;

    layout_type layout;
    FlashMemory memory{ layout.sector_size };

    memory.mounted<super_chain>([&](super_chain &super) {
        file_ops_type fops{ memory.pc(), super };
        ASSERT_EQ(fops.format(), 0);
        ASSERT_EQ(fops.touch("data.txt"), 0);

        ASSERT_EQ(fops.dir().find("data.txt", open_file_config{ }), 1);
        file_appender opened{ fops.pc(), &fops.dir(), fops.dir().open() };

        auto wrote = opened.write(lorem1k);
        ASSERT_GT(wrote, 0);

        ASSERT_EQ(opened.flush(), 0);
    });
}
