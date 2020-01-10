#include <gtest/gtest.h>

#include "pool.h"
#include "worker.h"
#include "collections.h"

using namespace fk;

FK_DECLARE_LOGGER("tests");

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

class SimpleWorker : public Worker {
public:
    SimpleWorker() {
    }

    SimpleWorker(Pool *pool) {
    }

public:
    void run(Pool &pool) override {
    }

    uint8_t priority() const {
        return OS_PRIORITY_NORMAL;
    }

    const char *name() {
        return "simple";
    }
};

struct complex_object_t {
    int32_t value1;
    int32_t value2;
};

struct nested_collection_t {
    int32_t value1;
    collection<uint32_t> numbers;

    nested_collection_t(int32_t v, collection<uint32_t> &&n) : value1(v), numbers(std::move(n)) {
    }
};

TEST_F(PoolSuite, Collections) {
    StaticPool<2048> pool("Pool");
    collection<uint32_t> integers{ pool };
    integers.add(100);
    integers.add(200);
    integers.add(300);

    for (auto i : integers) {
        loginfo("%d", i);
    }

    loginfo("%zd / %zd", pool.used(), pool.size());

    collection<complex_object_t> objects{ pool };
    objects.add({
        .value1 = 100,
        .value2 = 200,
    });
    objects.add({
        .value1 = 300,
        .value2 = 400,
    });
    for (auto &o : objects) {
        loginfo("%d, %d", o.value1, o.value2);
    }

    collection<complex_object_t> ctor_moved{ std::move(objects) };

    for (auto &o : ctor_moved) {
        loginfo("%d, %d", o.value1, o.value2);
    }

    collection<complex_object_t> assigned_moved{ pool };

    assigned_moved = std::move(ctor_moved);

    for (auto &o : assigned_moved) {
        loginfo("%d, %d", o.value1, o.value2);
    }

    collection<nested_collection_t> nested{ pool };

    // nested.add({ 0, std::move(integers) });
    nested.emplace(0, std::move(integers));
}

TEST_F(PoolSuite, ChainedPool) {
    auto pool = create_chained_pool_inside("chained");

    pool->malloc(256);
    pool->malloc(256);
    pool->malloc(512);
    pool->malloc(128);
    pool->malloc(512);
    pool->malloc(786);

    delete pool;
}

TEST_F(PoolSuite, ChainedPoolWrapper) {
    PoolPointer<SimpleWorker> *wrapped_concrete = create_chained_pool_wrapper<SimpleWorker>();

    PoolPointer<Worker> *wrapped = create_chained_pool_wrapper<Worker, PoolPointer<Worker>, SimpleWorker>();

    delete wrapped;

    delete wrapped_concrete;
}
