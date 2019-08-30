#include <gtest/gtest.h>

#include "pool.h"
#include "worker.h"

using namespace fk;

class PoolSuite : public ::testing::Test {
protected:

};

TEST_F(PoolSuite, Basic) {
    StaticPool<1024> pool("Pool");

    void *p1 = pool.malloc(256);
    ASSERT_NE(p1, nullptr);
    ASSERT_EQ(pool.allocated(), (size_t)256);

    void *p2 = pool.malloc(256);
    ASSERT_NE(p2, nullptr);
    ASSERT_EQ(pool.allocated(), (size_t)512);

    pool.clear();
    ASSERT_EQ(pool.allocated(), (size_t)0);
}

TEST_F(PoolSuite, Alignment) {
    StaticPool<1024> pool("Pool");

    void *p1 = pool.malloc(13);
    ASSERT_NE(p1, nullptr);
    ASSERT_EQ(pool.allocated(), (size_t)16);
}

TEST_F(PoolSuite, Subpool) {
    StaticPool<2048> pool("Pool");

    void *p1 = pool.malloc(256);
    ASSERT_NE(p1, nullptr);
    ASSERT_EQ(pool.allocated(), (size_t)256);
    ASSERT_FALSE(pool.frozen());

    Pool child = pool.freeze("Child");
    ASSERT_EQ(child.size(), (size_t)(2048 - 256));
    ASSERT_EQ(child.allocated(), (size_t)0);

    ASSERT_TRUE(pool.frozen());

    pool.clear();
    ASSERT_FALSE(pool.frozen());
}

class SimpleWorker {
public:
    void run(Pool &pool)  {
    }

    uint8_t priority() const {
        return OS_PRIORITY_NORMAL;
    }

    const char *name() {
        return "simple";
    }
};

TEST_F(PoolSuite, Workers) {
    auto worker = create_pool_worker<SimpleWorker>(1024);
    delete worker;
}
