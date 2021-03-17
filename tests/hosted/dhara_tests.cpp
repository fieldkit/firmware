#include "tests.h"
#include "common.h"
#include "hal/linux/linux.h"
#include "storage/dhara.h"
#include "storage/lfs_driver.h"
#include "storage/record_appender.h"
#include "storage/partitioned_reader.h"
#include "utilities.h"

using namespace fk;

FK_DECLARE_LOGGER("dhara");

class DharaSuite : public ::testing::Test {
protected:
    void SetUp() override {
        log_configure_level(LogLevels::INFO);
    }

    void TearDown() override {
        log_configure_level(LogLevels::NONE);
    }

};

TEST_F(DharaSuite, CreateAndMount) {
    LinuxDataMemory memory{ 100 };
    StandardPool pool{ "dhara" };

    ASSERT_TRUE(memory.begin());

    Dhara dhara;
    ASSERT_TRUE(dhara.begin(&memory, true, pool));

    auto sector = (uint8_t *)pool.malloc(dhara.page_size());

    memcpy(sector, "jacob", strlen("jacob"));

    if (!dhara.write(0, sector, dhara.page_size())) {
        logerror("write");
    }

    if (!dhara.write(1, sector, dhara.page_size())) {
        logerror("write");
    }

    if (!dhara.sync()) {
        logerror("sync");
    }

    if (!dhara.write(1, sector, dhara.page_size())) {
        logerror("write");
    }

    if (!dhara.sync()) {
        logerror("sync");
    }
}

TEST_F(DharaSuite, LfsMountOnly) {
    LinuxDataMemory memory{ 1024 };
    StandardPool pool{ "dhara" };

    log_configure_level(LogLevels::DEBUG);

    ASSERT_TRUE(memory.begin());

    LfsDriver lfs_driver;
    Dhara dhara;
    ASSERT_TRUE(dhara.begin(&memory, true, pool));

    loginfo("mounting lfs!");

    ASSERT_TRUE(lfs_driver.begin(dhara.map(), true, pool));

    lfs_unmount(lfs_driver.lfs());

    if (!dhara.sync()) {
        logerror("sync");
    }

    loginfo("mounting lfs!");

    ASSERT_TRUE(lfs_driver.begin(dhara.map(), false, pool));

    lfs_unmount(lfs_driver.lfs());

    if (!dhara.sync()) {
        logerror("sync");
    }
}

TEST_F(DharaSuite, LfsAppendOne) {
    LinuxDataMemory memory{ 100 };
    StandardPool pool{ "dhara" };

    log_configure_level(LogLevels::DEBUG);

    ASSERT_TRUE(memory.begin());

    Dhara dhara;
    ASSERT_TRUE(dhara.begin(&memory, true, pool));

    loginfo("mounting lfs!");

    LfsDriver lfs_driver;
    ASSERT_TRUE(lfs_driver.begin(dhara.map(), true, pool));

    FileMap map{ &lfs_driver, "data", 5, pool };
    RecordAppender appender{ &lfs_driver, &map, 1024 * 100, pool };
    ReadingRecord readings{ 0, 0 };
    auto appended = appender.append_data_record(&readings.record, pool);
    ASSERT_TRUE(appended);

    lfs_unmount(lfs_driver.lfs());

    if (!dhara.sync()) {
        logerror("sync");
    }
}

TEST_F(DharaSuite, LfsAppendMultiple) {
    LinuxDataMemory memory{ 100 };
    StandardPool pool{ "dhara" };

    log_configure_level(LogLevels::DEBUG);

    ASSERT_TRUE(memory.begin());

    Dhara dhara;
    ASSERT_TRUE(dhara.begin(&memory, true, pool));

    loginfo("mounting lfs!");

    LfsDriver lfs_driver;
    ASSERT_TRUE(lfs_driver.begin(dhara.map(), true, pool));

    FileMap map{ &lfs_driver, "data", 5, pool };
    RecordAppender appender{ &lfs_driver, &map, 1024 * 100, pool };
    ReadingRecord readings{ 0, 0 };
    for (auto i = 0u; i < 10; ++i) {
        StandardPool iter{ "iter" };
        auto appended = appender.append_data_record(&readings.record, iter);
        ASSERT_TRUE(appended);
        ASSERT_EQ(appended->record, i);
    }

    lfs_unmount(lfs_driver.lfs());

    if (!dhara.sync()) {
        logerror("sync");
    }
}

TEST_F(DharaSuite, LfsCreateWriteRead) {
    LinuxDataMemory memory{ 100 };
    StandardPool pool{ "dhara" };

    log_configure_level(LogLevels::DEBUG);

    ASSERT_TRUE(memory.begin());

    Dhara dhara;
    ASSERT_TRUE(dhara.begin(&memory, true, pool));

    loginfo("mounting lfs!");

    LfsDriver lfs_driver;
    ASSERT_TRUE(lfs_driver.begin(dhara.map(), true, pool));

    auto lfs = lfs_driver.lfs();

    lfs_file_t file;
    uint32_t counter = 0;
    lfs_file_open(lfs, &file, "counter", LFS_O_RDWR | LFS_O_CREAT);
    lfs_file_read(lfs, &file, &counter, sizeof(counter));
    counter += 1;
    lfs_file_rewind(lfs, &file);
    lfs_file_write(lfs, &file, &counter, sizeof(counter));
    lfs_file_close(lfs, &file);

    loginfo("counter: %d", counter);

    lfs_unmount(lfs_driver.lfs());

    if (!dhara.sync()) {
        logerror("sync");
    }

    lfs_file_open(lfs, &file, "counter", LFS_O_RDWR | LFS_O_CREAT);
    lfs_file_read(lfs, &file, &counter, sizeof(counter));
    counter += 1;
    lfs_file_rewind(lfs, &file);
    lfs_file_write(lfs, &file, &counter, sizeof(counter));
    lfs_file_close(lfs, &file);

    loginfo("counter: %d", counter);

    if (!dhara.sync()) {
        logerror("sync");
    }
}

typedef struct sector_header_t {
    uint32_t version;
} sector_header_t;

TEST_F(DharaSuite, RewriteSector0OverAndOver) {
    LinuxDataMemory memory{ 100 };
    StandardPool pool{ "dhara" };

    ASSERT_TRUE(memory.begin());

    Dhara dhara;
    ASSERT_TRUE(dhara.begin(&memory, true, pool));

    auto sector = (uint8_t *)pool.malloc(dhara.page_size());
    auto ptr = (sector_header_t *)sector;
    bzero(sector, dhara.page_size());

    for (auto i = 0; i < 100; ++i) {
        dhara_page_t page = 0;
        if (!dhara.find(0, &page)) {
            logerror("find");
        }

        if (i > 0) {
            if (!dhara.read(0, sector, dhara.page_size())) {
                logerror("read");
            }

            ptr->version++;
        }

        loginfo("%d page=%d", ptr->version, page);

        if (!dhara.write(0, sector, dhara.page_size())) {
            logerror("write");
        }

        if (!dhara.sync()) {
            logerror("sync");
        }
    }
}

#define DE_CREATE           (1)
#define DE_UNLINK           (2)
#define DE_SECTOR           (4)
#define DE_UPDATE           (5)
#define DE_INLINE           (6)

struct phy_superblock_t {
    char magic[8];
    uint32_t version;
    uint32_t nsectors;
};

struct phy_directory_entry_t {
    uint8_t type;
};

struct phy_sector_chain_t {
    phy_directory_entry_t entry;
    uint32_t head;
    uint32_t nsectors;
    uint32_t tail;
};

struct phy_attribute_t {
    uint8_t type;
    uint32_t size : 24;
};

struct phy_file_create_t {
    phy_directory_entry_t entry;
    uint32_t id;
    uint32_t size;
    uint32_t head;
    uint32_t tail;
    char name[128];
    uint16_t nattrs;
};

struct phy_file_sector_t {
    phy_directory_entry_t entry;
    uint32_t id;
    uint32_t size;
    uint32_t sector;
    uint16_t nattrs;
};

struct phy_file_unlink_t {
    phy_directory_entry_t entry;
    uint32_t id;
};

struct phy_file_data_inline_t {
    phy_directory_entry_t entry;
    uint32_t id;
    uint32_t size;
};

struct phy_file_data_link_t {
    phy_directory_entry_t entry;
    uint32_t id;
    uint32_t sector;
};

int32_t phy_sector_chain_initialize(phy_sector_chain_t *sc, uint32_t head) {
    return 0;
}

int32_t phy_sector_chain_iterate(phy_sector_chain_t *sc) {
    return 0;
}

int32_t phy_sector_chain_eoc_get(phy_sector_chain_t *sc) {
    return 0;
}

dhara_sector_t phy_sector_chain_sector_get(phy_sector_chain_t *sc) {
    return 0;
}

class DirectoryPage {
private:
    uint8_t *ptr_{ nullptr };
    size_t size_{ 0 };
    size_t id_{ 0 };
    phy_file_create_t *file_create_{ nullptr };
    uint8_t *tail_{ nullptr };

public:
    DirectoryPage(uint8_t *ptr, size_t size);

public:
    bool touch(const char *name);
    bool unlink(const char *name);

};

DirectoryPage::DirectoryPage(uint8_t *ptr, size_t size) : ptr_(ptr), size_(size) {
}

bool DirectoryPage::touch(const char *name) {
    auto fc = reinterpret_cast<phy_file_create_t *>(tail_);
    fc->entry.type = DE_CREATE;
    strncpy(fc->name, name, sizeof(fc->name));
    fc->id = ++id_;
    fc->size = 0;
    tail_ += sizeof(phy_file_create_t);

    return true;
}

bool DirectoryPage::unlink(const char *name) {
    auto fc = reinterpret_cast<phy_file_unlink_t *>(tail_);
    fc->entry.type = DE_UNLINK;
    fc->id = 0;
    tail_ += sizeof(phy_file_unlink_t);

    return true;
}
