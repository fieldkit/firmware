#include <directory_chain.h>
#include <directory_tree.h>
#include <free_sectors_chain.h>
#include <data_chain.h>
#include <tree_sector.h>

#include "phylum_tests.h"
#include "geometry.h"

using namespace phylum;

template <typename T> class FreeSectorsFixture : public PhylumFixture {};

typedef ::testing::Types<
    std::pair<layout_256, tree_sector<uint32_t, uint32_t, 6>>,
    std::pair<layout_4096, tree_sector<uint32_t, uint32_t, 64>>
    >
    Implementations;

TYPED_TEST_SUITE(FreeSectorsFixture, Implementations);

using dir_type = directory_chain;

class test_data_chain : public data_chain {
public:
    test_data_chain(phyctx pc, head_tail_t chain, const char *prefix = "tdc")
        : data_chain(pc, chain, prefix) {
    }

public:
    int32_t grow_by(size_t blocks) {
        phydebugf("grow by %d blocks", blocks);
        suppress_logs sl;
        auto lock = db().writing(sector());
        for (auto i = 0u; i < blocks; ++i) {
            auto err = grow_tail(lock);
            if (err < 0) {
                return err;
            }

            err = lock.flush(lock.sector());
            if (err < 0) {
                return err;
            }
        }
        return 0;
    }
};

TYPED_TEST(FreeSectorsFixture, FreeSectorsChain_DataChains_SingleBlock) {
    typename TypeParam::first_type layout;
    FlashMemory memory{ layout.sector_size };

    memory.mounted<dir_type>([&](auto &dir) {
        test_data_chain dc{ memory.pc(), head_tail_t{ } };
        ASSERT_EQ(dc.create_if_necessary(), 0);

        free_sectors_chain fsc{ memory.pc(), head_tail_t{ } };
        ASSERT_EQ(fsc.create_if_necessary(), 0);

        dhara_sector_t sector = 0;
        ASSERT_EQ(fsc.dequeue(&sector), 0);

        ASSERT_EQ(fsc.add_chain(dc.head()), 0);

        ASSERT_EQ(fsc.dequeue(&sector), 1);
        ASSERT_EQ(sector, dc.head());

        ASSERT_EQ(fsc.dequeue(&sector), 0);
    });
}

TYPED_TEST(FreeSectorsFixture, FreeSectorsChain_DataChains_TwoBlocks) {
    typename TypeParam::first_type layout;
    FlashMemory memory{ layout.sector_size };

    memory.mounted<dir_type>([&](auto &dir) {
        test_data_chain dc{ memory.pc(), head_tail_t{ } };
        ASSERT_EQ(dc.create_if_necessary(), 0);
        ASSERT_EQ(dc.grow_by(1), 0);

        ASSERT_NE(dc.head(), dc.tail());

        free_sectors_chain fsc{ memory.pc(), head_tail_t{ } };
        ASSERT_EQ(fsc.create_if_necessary(), 0);

        dhara_sector_t sector = 0;
        ASSERT_EQ(fsc.dequeue(&sector), 0);

        ASSERT_EQ(fsc.add_chain(dc.head()), 0);

        ASSERT_EQ(fsc.dequeue(&sector), 1);
        ASSERT_EQ(sector, dc.head());

        ASSERT_EQ(fsc.dequeue(&sector), 1);
        ASSERT_EQ(sector, dc.tail());

        ASSERT_EQ(fsc.dequeue(&sector), 0);
    });
}

TYPED_TEST(FreeSectorsFixture, FreeSectorsChain_DataChains_SeveralBlock) {
    typename TypeParam::first_type layout;
    FlashMemory memory{ layout.sector_size };

    memory.mounted<dir_type>([&](auto &dir) {
        test_data_chain dc{ memory.pc(), head_tail_t{ } };
        ASSERT_EQ(dc.create_if_necessary(), 0);
        ASSERT_EQ(dc.grow_by(10), 0);

        ASSERT_NE(dc.head(), dc.tail());

        free_sectors_chain fsc{ memory.pc(), head_tail_t{ } };
        ASSERT_EQ(fsc.create_if_necessary(), 0);

        dhara_sector_t sector = 0;
        ASSERT_EQ(fsc.dequeue(&sector), 0);

        ASSERT_EQ(fsc.add_chain(dc.head()), 0);

        for (auto i = 0; i <= 10; ++i) {
            ASSERT_EQ(fsc.dequeue(&sector), 1);
            ASSERT_GT(sector, 0u);
        }

        ASSERT_EQ(fsc.dequeue(&sector), 0);
    });
}

TYPED_TEST(FreeSectorsFixture, FreeSectorsChain_DataChains_ReuseConsumedNode) {
    typename TypeParam::first_type layout;
    FlashMemory memory{ layout.sector_size };

    memory.mounted<dir_type>([&](auto &dir) {
        test_data_chain dc1{ memory.pc(), head_tail_t{ } };
        ASSERT_EQ(dc1.create_if_necessary(), 0);
        ASSERT_EQ(dc1.grow_by(1), 0);
        ASSERT_NE(dc1.head(), dc1.tail());

        test_data_chain dc2{ memory.pc(), head_tail_t{ } };
        ASSERT_EQ(dc2.create_if_necessary(), 0);
        ASSERT_EQ(dc2.grow_by(1), 0);
        ASSERT_NE(dc2.head(), dc2.tail());

        free_sectors_chain fsc{ memory.pc(), head_tail_t{ } };
        ASSERT_EQ(fsc.create_if_necessary(), 0);

        dhara_sector_t sector = 0;
        ASSERT_EQ(fsc.dequeue(&sector), 0);

        ASSERT_EQ(fsc.add_chain(dc1.head()), 0);

        ASSERT_EQ(fsc.dequeue(&sector), 1);
        ASSERT_EQ(sector, dc1.head());

        ASSERT_EQ(fsc.dequeue(&sector), 1);
        ASSERT_EQ(sector, dc1.tail());

        ASSERT_EQ(fsc.dequeue(&sector), 0);

        ASSERT_EQ(fsc.add_chain(dc2.head()), 0);

        ASSERT_EQ(fsc.dequeue(&sector), 1);
        ASSERT_EQ(sector, dc2.head());

    });
}

TYPED_TEST(FreeSectorsFixture, FreeSectorsChain_Trees_SingleSector) {
    using tree_type = typename TypeParam::second_type;
    typename TypeParam::first_type layout;
    FlashMemory memory{ layout.sector_size };

    memory.mounted<dir_type>([&](auto &dir) {
        tree_type tree{ memory.pc(), tree_ptr_t{ memory.allocator().allocate() } };
        ASSERT_EQ(tree.create(), 0);

        for (auto i = 1u; i < 1; ++i) {
            ASSERT_EQ(tree.add(i, i), 0);
        }

        free_sectors_chain fsc{ memory.pc(), head_tail_t{ } };
        ASSERT_EQ(fsc.create_if_necessary(), 0);

        dhara_sector_t sector = 0;
        ASSERT_EQ(fsc.dequeue(&sector), 0);

        ASSERT_EQ(fsc.add_tree(tree.to_tree_ptr()), 0);

        ASSERT_EQ(fsc.dequeue(&sector), 1);

        ASSERT_EQ(fsc.dequeue(&sector), 0);
    });
}

TYPED_TEST(FreeSectorsFixture, FreeSectorsChain_Trees_MultipleSectors) {
    using tree_type = typename TypeParam::second_type;
    typename TypeParam::first_type layout;
    FlashMemory memory{ layout.sector_size };

    auto &allocator = memory.allocator();

    memory.mounted<dir_type>([&](auto &dir) {
        tree_type tree{ memory.pc(), tree_ptr_t{ memory.allocator().allocate() } };
        ASSERT_EQ(tree.create(), 0);

        {
            suppress_logs sl;
            auto nsectors = allocator.allocated() + 2;
            for (auto i = 0u; allocator.allocated() < nsectors; ++i) {
                ASSERT_EQ(tree.add(i, i), 0);
            }
        }

        EXPECT_EQ(tree.log(), 0);

        free_sectors_chain fsc{ memory.pc(), head_tail_t{ } };
        ASSERT_EQ(fsc.create_if_necessary(), 0);

        dhara_sector_t sector = 0;
        ASSERT_EQ(fsc.dequeue(&sector), 0);

        ASSERT_EQ(fsc.add_tree(tree.to_tree_ptr()), 0);

        ASSERT_EQ(fsc.dequeue(&sector), 1);
        phydebugf("dequeued: %d", sector);

        ASSERT_EQ(fsc.dequeue(&sector), 1);
        phydebugf("dequeued: %d", sector);

        ASSERT_EQ(fsc.dequeue(&sector), 1);
        phydebugf("dequeued: %d", sector);

        ASSERT_EQ(fsc.dequeue(&sector), 0);
        phydebugf("dequeued: %d", sector);
    });
}

TYPED_TEST(FreeSectorsFixture, FreeSectorsChain_Trees_Large) {
    using tree_type = typename TypeParam::second_type;
    typename TypeParam::first_type layout;
    FlashMemory memory{ layout.sector_size };

    auto &allocator = memory.allocator();

    memory.mounted<dir_type>([&](auto &dir) {
        tree_type tree{ memory.pc(), tree_ptr_t{ memory.allocator().allocate() } };
        ASSERT_EQ(tree.create(), 0);

        free_sectors_chain fsc{ memory.pc(), head_tail_t{ } };
        ASSERT_EQ(fsc.create_if_necessary(), 0);

        dhara_sector_t sector = 0;
        ASSERT_EQ(fsc.dequeue(&sector), 0);

        phydebugf("allocated: %d", allocator.allocated());

        {
            suppress_logs sl;
            auto nsectors = allocator.allocated() + 50;
            for (auto i = 0u; allocator.allocated() < nsectors; ++i) {
                ASSERT_EQ(tree.add(i, i), 0);
            }
        }

        tree.log();

        ASSERT_EQ(fsc.add_tree(tree.to_tree_ptr()), 0);

        std::map<dhara_sector_t, bool> returned;

        auto total_dequeued = 0u;
        while (fsc.dequeue(&sector) == 1) {
            phyinfof("dequeued: %d", sector);
            ASSERT_FALSE(returned[sector]);
            returned[sector] = true;
            total_dequeued++;
        }

        ASSERT_EQ(total_dequeued, 51u);
    });
}
