#include "tests.h"
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

namespace fk {

FK_ENABLE_TYPE_NAME(SimpleWorker);

}

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

TEST_F(PoolSuite, SimpleCollection) {
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

    nested.emplace(0, std::move(integers));
}

TEST_F(PoolSuite, SimpleHashMap) {
    StaticPool<2048> pool("Pool");
    hash_map<uint32_t, uint32_t> integers{ pool };
    integers.put(100, 2423);
    integers.put(200, 9658);
    integers.put(300, 3293);

    uint32_t value = 0;

    ASSERT_TRUE(integers.get(300, value));
    ASSERT_EQ(value, 3293u);

    ASSERT_TRUE(integers.get(100, value));
    ASSERT_EQ(value, 2423u);
}

TEST_F(PoolSuite, StandardPool) {
    auto pool = create_standard_pool_inside("standard");

    pool->malloc(256);
    pool->malloc(256);
    pool->malloc(512);
    pool->malloc(128);
    pool->malloc(512);
    pool->malloc(786);

    delete pool;
}

void free_pool(Pool *pool) {
    delete pool;
}

TEST_F(PoolSuite, ChainedPoolWrapper) {
    PoolPointer<SimpleWorker> *wrapped_concrete = create_chained_pool_wrapper<SimpleWorker>();

    PoolPointer<Worker> *wrapped = create_chained_pool_wrapper<Worker, PoolPointer<Worker>, SimpleWorker>();

    delete wrapped;

    delete wrapped_concrete;

    auto pool = create_pool_inside("ok");

    free_pool(pool);
}

class DummyObject {
private:
    uint8_t data_[32];

};

TEST_F(PoolSuite, Clear) {
    auto pool = create_standard_pool_inside("test");

    new (pool) DummyObject();

    ASSERT_EQ(pool->used(), sizeof(StandardPool) + sizeof(DummyObject));

    pool->clear();

    ASSERT_EQ(pool->used(), sizeof(StandardPool));

    new (pool) DummyObject();

    ASSERT_EQ(pool->used(), sizeof(StandardPool) + sizeof(DummyObject));

    pool->clear();

    ASSERT_EQ(pool->used(), sizeof(StandardPool));

    delete pool;
}

TEST_F(PoolSuite, AllocatingSiblings) {
    // log_configure_level(LogLevels::DEBUG);

    fk_standard_page_log();
    // One page taken up by module factory ctor, still need to fix that.
    ASSERT_EQ(fk_standard_page_meminfo().used, 0u);

    StandardPool pool{ "test-siblings" };
    ASSERT_EQ(pool.size(), 8192u);

    ASSERT_EQ(fk_standard_page_meminfo().used, 1u);

    for (auto i = 0u; i < 20; ++i) {
        pool.malloc(2048);
        pool.malloc(2048);
        pool.malloc(2048);
        pool.malloc(2048);
        pool.malloc(2048);
        pool.clear();
    }

    ASSERT_EQ(pool.size(), 8192u);
    ASSERT_EQ(fk_standard_page_meminfo().used, 1u);

    for (auto i = 0u; i < 20; ++i) {
        StandardPool pool{ "test" };
        pool.malloc(2048);
        pool.malloc(2048);
        pool.malloc(2048);
        pool.malloc(2048);
        pool.malloc(2048);
    }

    ASSERT_EQ(fk_standard_page_meminfo().used, 1u);

    fk_standard_page_log();
}

TEST_F(PoolSuite, Subpool) {
    StandardPool pool{ "top" };

    ASSERT_EQ(pool.used(), 0u);

    loginfo("spawn");
    Pool *child = pool.subpool("child", 128);
    loginfo("alloc 96");
    child->malloc(96);

    ASSERT_EQ(pool.used(), (128u + sizeof(StandardPool)) * 1);

    // Force another allocation in the child.
    loginfo("alloc 96");
    child->malloc(96);

    loginfo("clearing child");
    child->clear();

    loginfo("alloc 96");
    child->malloc(96);

    loginfo("alloc 96");
    child->malloc(96);

    ASSERT_EQ(pool.used(), (128u + sizeof(StandardPool)) * 2);

    loginfo("clearing");
    pool.clear();
}

TEST_F(PoolSuite, SubpoolMultiple) {
    StandardPool pool{ "top" };

    loginfo("spawn");

    ASSERT_EQ(pool.size(), 8192u);
    ASSERT_EQ(pool.used(), 0u);

    Pool *child1 = pool.subpool("child1", 128);
    loginfo("+child1 alloc 96.0");
    child1->malloc(96);
    loginfo("-child1 alloc 96.0");

    ASSERT_EQ(pool.used(), (128u + sizeof(StandardPool)) * 1);

    Pool *child2 = pool.subpool("child2", 128);
    loginfo("+child2 alloc 96.1");
    child2->malloc(96);
    loginfo("-child2 alloc 96.1");

    ASSERT_EQ(pool.used(), (128u + sizeof(StandardPool)) * 2);

    loginfo("+child2 alloc 96.2");
    child2->malloc(96);
    loginfo("-child2 alloc 96.2");

    ASSERT_EQ(pool.used(), (128u + sizeof(StandardPool)) * 3);

    loginfo("clearing");
    pool.clear();

    ASSERT_EQ(pool.used(), 0u);

    Pool *child3 = pool.subpool("child3", 128);
    loginfo("+child3 alloc 96.1");
    child3->malloc(96);
    loginfo("-child3 alloc 96.1");

    ASSERT_EQ(pool.used(), (128u + sizeof(StandardPool)) * 1);
}
