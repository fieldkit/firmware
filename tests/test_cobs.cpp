#include <cobs.h>

#include "phylum_tests.h"
#include "geometry.h"

using namespace phylum;

class CobsFixture : public PhylumFixture {
protected:
    FlashMemory memory{ 256 };

};

class malloc_write_buffer : public write_buffer {
public:
    malloc_write_buffer(size_t size) : write_buffer((uint8_t *)malloc(size), size) {
    }

    virtual ~malloc_write_buffer() {
        ::free(ptr());
    }
};

TEST_F(CobsFixture, Write_Examples_0) {
    malloc_write_buffer destination_buffer{ 1024 };
    malloc_write_buffer lookahead{ 256 };
    buffer_writer destination{ destination_buffer };
    cobs_writer writer{ &destination, lookahead };

    auto test = "Jacob Lewallen";
    EXPECT_EQ(writer.write((uint8_t *)test, strlen(test)), (int32_t)strlen(test) + 2);

    uint8_t expected[strlen(test) + 2];
    expected[0] = strlen(test) + 1;
    expected[strlen(test) + 1] = 0;
    memcpy(expected + 1, test, strlen(test));

    EXPECT_EQ(memcmp(expected, destination_buffer.ptr(), sizeof(expected)), 0);

    read_buffer buffer = destination_buffer.read_back();
    buffer_reader verifying{ buffer };
    cobs_reader reader{ &verifying };
    uint8_t actual[1024];
    EXPECT_EQ(reader.read(actual, sizeof(actual)), (int32_t)strlen(test));

    EXPECT_EQ(memcmp(test, test, strlen(test)), 0);
}

TEST_F(CobsFixture, Write_Examples_1) {
    malloc_write_buffer destination_buffer{ 1024 };
    malloc_write_buffer lookahead{ 256 };
    buffer_writer destination{ destination_buffer };
    cobs_writer writer{ &destination, lookahead };

    uint8_t original[] = { 0x0 };
    uint8_t expected[] = { 0x1, 0x1, 0x0 };
    EXPECT_EQ(writer.write(original, sizeof(original)), (int32_t)sizeof(expected));

    // phydebug_dump_memory("original ", original, sizeof(original));
    // phydebug_dump_memory("expected ", expected, sizeof(expected));
    // phydebug_dump_memory("actual   ", destination_buffer.ptr(), destination_buffer.position());

    EXPECT_EQ(memcmp(expected, destination_buffer.ptr(), sizeof(expected)), 0);

    read_buffer buffer = destination_buffer.read_back();
    buffer_reader verifying{ buffer };
    cobs_reader reader{ &verifying };
    uint8_t actual[1024];
    EXPECT_EQ(reader.read(actual, sizeof(actual)), (int32_t)sizeof(original));

    EXPECT_EQ(memcmp(original, actual, sizeof(original)), 0);
}

TEST_F(CobsFixture, Write_Examples_2) {
    malloc_write_buffer destination_buffer{ 1024 };
    malloc_write_buffer lookahead{ 256 };
    buffer_writer destination{ destination_buffer };
    cobs_writer writer{ &destination, lookahead };

    uint8_t original[] = { 0x00, 0x00 };
    uint8_t expected[] = { 0x01, 0x01, 0x01, 0x00 };
    ASSERT_EQ(writer.write(original, sizeof(original)), (int32_t)sizeof(expected));

    ASSERT_EQ(memcmp(expected, destination_buffer.ptr(), sizeof(expected)), 0);

    read_buffer buffer = destination_buffer.read_back();
    buffer_reader verifying{ buffer };
    cobs_reader reader{ &verifying };
    uint8_t actual[1024];
    EXPECT_EQ(reader.read(actual, sizeof(actual)), (int32_t)sizeof(original));

    EXPECT_EQ(memcmp(original, actual, sizeof(original)), 0);
}

TEST_F(CobsFixture, Write_Examples_3) {
    malloc_write_buffer destination_buffer{ 1024 };
    malloc_write_buffer lookahead{ 256 };
    buffer_writer destination{ destination_buffer };
    cobs_writer writer{ &destination, lookahead };

    uint8_t original[] = { 0x11, 0x22, 0x00, 0x33 };
    uint8_t expected[] = { 0x03, 0x11, 0x22, 0x02, 0x33, 0x00 };
    ASSERT_EQ(writer.write(original, sizeof(original)), (int32_t)sizeof(expected));

    ASSERT_EQ(memcmp(expected, destination_buffer.ptr(), sizeof(expected)), 0);

    read_buffer buffer = destination_buffer.read_back();
    buffer_reader verifying{ buffer };
    cobs_reader reader{ &verifying };
    uint8_t actual[1024];
    EXPECT_EQ(reader.read(actual, sizeof(actual)), (int32_t)sizeof(original));

    EXPECT_EQ(memcmp(original, actual, sizeof(original)), 0);
}

TEST_F(CobsFixture, Write_Examples_4) {
    malloc_write_buffer destination_buffer{ 1024 };
    malloc_write_buffer lookahead{ 256 };
    buffer_writer destination{ destination_buffer };
    cobs_writer writer{ &destination, lookahead };

    uint8_t original[] = { 0x11, 0x22, 0x33, 0x44 };
    uint8_t expected[] = { 0x05, 0x11, 0x22, 0x33, 0x44, 0x00 };
    ASSERT_EQ(writer.write(original, sizeof(original)), (int32_t)sizeof(expected));

    ASSERT_EQ(memcmp(expected, destination_buffer.ptr(), sizeof(expected)), 0);

    read_buffer buffer = destination_buffer.read_back();
    buffer_reader verifying{ buffer };
    cobs_reader reader{ &verifying };
    uint8_t actual[1024];
    EXPECT_EQ(reader.read(actual, sizeof(actual)), (int32_t)sizeof(original));

    EXPECT_EQ(memcmp(original, actual, sizeof(original)), 0);
}

TEST_F(CobsFixture, Write_Examples_5) {
    malloc_write_buffer destination_buffer{ 1024 };
    malloc_write_buffer lookahead{ 256 };
    buffer_writer destination{ destination_buffer };
    cobs_writer writer{ &destination, lookahead };

    uint8_t original[] = { 0x11, 0x00, 0x00, 0x00 };
    uint8_t expected[] = { 0x02, 0x11, 0x01, 0x01, 0x01, 0x00 };
    ASSERT_EQ(writer.write(original, sizeof(original)), (int32_t)sizeof(expected));

    ASSERT_EQ(memcmp(expected, destination_buffer.ptr(), sizeof(expected)), 0);

    read_buffer buffer = destination_buffer.read_back();
    buffer_reader verifying{ buffer };
    cobs_reader reader{ &verifying };
    uint8_t actual[1024];
    EXPECT_EQ(reader.read(actual, sizeof(actual)), (int32_t)sizeof(original));

    EXPECT_EQ(memcmp(original, actual, sizeof(original)), 0);
}

TEST_F(CobsFixture, Write_Examples_6) {
    malloc_write_buffer destination_buffer{ 1024 };
    malloc_write_buffer lookahead{ 256 };
    buffer_writer destination{ destination_buffer };
    cobs_writer writer{ &destination, lookahead };

    uint8_t original[254];
    uint8_t expected[256];
    expected[0] = 0xff;
    for (auto i = 0; i < 255; ++i) {
        original[i    ] = i + 1;
        expected[i + 1] = i + 1;
    }
    expected[255] = 0;

    // phydebug_dump_memory("original ", original, sizeof(original));
    // phydebug_dump_memory("expected ", expected, sizeof(expected));

    ASSERT_EQ(writer.write(original, sizeof(original)), (int32_t)sizeof(expected));

    // phydebug_dump_memory("actual ", destination_buffer.ptr(), destination_buffer.position());

    ASSERT_EQ(memcmp(expected, destination_buffer.ptr(), sizeof(expected)), 0);

    read_buffer buffer = destination_buffer.read_back();
    buffer_reader verifying{ buffer };
    cobs_reader reader{ &verifying };
    uint8_t actual[1024];
    memset(actual, 0xff, sizeof(actual));
    EXPECT_EQ(reader.read(actual, sizeof(actual)), (int32_t)sizeof(original));

    // phydebug_dump_memory("verify ", actual, sizeof(actual));

    EXPECT_EQ(memcmp(original, actual, sizeof(original)), 0);
}

TEST_F(CobsFixture, Write_Examples_7) {
    malloc_write_buffer destination_buffer{ 1024 };
    malloc_write_buffer lookahead{ 256 };
    buffer_writer destination{ destination_buffer };
    cobs_writer writer{ &destination, lookahead };

    uint8_t original[255];
    uint8_t expected[257];
    for (auto i = 0; i < 255; ++i) {
        original[i    ] = i;
        expected[i + 1] = i;
    }
    expected[0] = 0x01;
    expected[1] = 0xff;
    expected[256] = 0;

    // phydebug_dump_memory("original ", original, sizeof(original));
    // phydebug_dump_memory("expected ", expected, sizeof(expected));

    ASSERT_EQ(writer.write(original, sizeof(original)), (int32_t)sizeof(expected));

    // phydebug_dump_memory("actual ", destination_buffer.ptr(), destination_buffer.position());

    ASSERT_EQ(memcmp(expected, destination_buffer.ptr(), sizeof(expected)), 0);

    read_buffer buffer = destination_buffer.read_back();
    buffer_reader verifying{ buffer };
    cobs_reader reader{ &verifying };
    uint8_t actual[1024];
    memset(actual, 0xff, sizeof(actual));
    EXPECT_EQ(reader.read(actual, sizeof(actual)), (int32_t)sizeof(original));

    // phydebug_dump_memory("verify ", actual, sizeof(actual));

    EXPECT_EQ(memcmp(original, actual, sizeof(original)), 0);
}

TEST_F(CobsFixture, Write_Examples_8) {
    malloc_write_buffer destination_buffer{ 1024 };
    malloc_write_buffer lookahead{ 256 };
    buffer_writer destination{ destination_buffer };
    cobs_writer writer{ &destination, lookahead };

    uint8_t original[255];
    for (auto i = 0u; i < sizeof(original); ++i) {
        original[i] = i + 1;
    }

    uint8_t expected[258];
    expected[0] = 255;
    for (auto i = 0; i < 255; ++i) {
        expected[i + 1] = i + 1;
    }
    expected[255] = 0x02;
    expected[256] = 0xff;
    expected[257] = 0;

    // phydebug_dump_memory("original ", original, sizeof(original));
    // phydebug_dump_memory("expected ", expected, sizeof(expected));

    EXPECT_EQ(writer.write(original, sizeof(original)), (int32_t)sizeof(expected));

    // phydebug_dump_memory("actual ", destination_buffer.ptr(), destination_buffer.position());

    EXPECT_EQ(memcmp(expected, destination_buffer.ptr(), sizeof(expected)), 0);

    read_buffer buffer = destination_buffer.read_back();
    buffer_reader verifying{ buffer };
    cobs_reader reader{ &verifying };
    uint8_t actual[1024];
    EXPECT_EQ(reader.read(actual, sizeof(actual)), (int32_t)sizeof(original));

    EXPECT_EQ(memcmp(original, actual, sizeof(original)), 0);
}

TEST_F(CobsFixture, Write_Examples_9) {
    malloc_write_buffer destination_buffer{ 1024 };
    malloc_write_buffer lookahead{ 256 };
    buffer_writer destination{ destination_buffer };
    cobs_writer writer{ &destination, lookahead };

    uint8_t original[255];
    for (auto i = 0u; i < sizeof(original); ++i) {
        original[i] = i + 2;
    }

    uint8_t expected[258];
    expected[0] = 255;
    for (auto i = 0u; i < 255; ++i) {
        expected[i + 1] = i + 2;
    }
    expected[255] = 1;
    expected[256] = 1;
    expected[257] = 0;

    // phydebug_dump_memory("original ", original, sizeof(original));
    // phydebug_dump_memory("expected ", expected, sizeof(expected));

    EXPECT_EQ(writer.write(original, sizeof(original)), (int32_t)sizeof(expected));

    EXPECT_EQ(memcmp(expected, destination_buffer.ptr(), sizeof(expected)), 0);

    read_buffer buffer = destination_buffer.read_back();
    buffer_reader verifying{ buffer };
    cobs_reader reader{ &verifying };
    uint8_t actual[1024];
    EXPECT_EQ(reader.read(actual, sizeof(actual)), (int32_t)sizeof(original));

    EXPECT_EQ(memcmp(original, actual, sizeof(original)), 0);
}

TEST_F(CobsFixture, Write_Examples_10) {
    malloc_write_buffer destination_buffer{ 1024 };
    malloc_write_buffer lookahead{ 256 };
    buffer_writer destination{ destination_buffer };
    cobs_writer writer{ &destination, lookahead };

    uint8_t original[255];
    for (auto i = 0u; i < sizeof(original); ++i) {
        original[i] = i + 3;
    }

    uint8_t expected[257];
    expected[0] = 254;
    for (auto i = 0u; i < 255; ++i) {
        expected[i + 1] = i + 3;
    }
    expected[254] = 2;
    expected[255] = 1;
    expected[256] = 0;

    // phydebug_dump_memory("original ", original, sizeof(original));
    // phydebug_dump_memory("expected ", expected, sizeof(expected));

    EXPECT_EQ(writer.write(original, sizeof(original)), (int32_t)sizeof(expected));

    // phydebug_dump_memory("actual ", destination_buffer.ptr(), destination_buffer.position());

    EXPECT_EQ(memcmp(expected, destination_buffer.ptr(), sizeof(expected)), 0);

    read_buffer buffer = destination_buffer.read_back();
    buffer_reader verifying{ buffer };
    cobs_reader reader{ &verifying };
    uint8_t actual[1024];
    memset(actual, 0xff, sizeof(actual));
    EXPECT_EQ(reader.read(actual, sizeof(actual)), (int32_t)sizeof(original));

    EXPECT_EQ(memcmp(original, actual, sizeof(original)), 0);
}

TEST_F(CobsFixture, Write_MultipleWrites) {
    malloc_write_buffer destination_buffer{ 1024 };
    malloc_write_buffer lookahead{ 256 };
    buffer_writer destination{ destination_buffer };
    cobs_writer writer{ &destination, lookahead };

    uint8_t original[16];
    for (auto i = 0u; i < sizeof(original); ++i) {
        original[i] = i + 1;
    }

    EXPECT_GT(writer.write(original, sizeof(original)), 0);
    EXPECT_GT(writer.write(original, sizeof(original)), 0);
    EXPECT_GT(writer.write(original, sizeof(original)), 0);
    EXPECT_GT(writer.write(original, sizeof(original)), 0);

    // phydebug_dump_memory("encoded ", destination_buffer.ptr(), destination_buffer.position());

    read_buffer buffer = destination_buffer.read_back();
    buffer_reader verifying{ buffer };
    cobs_reader reader{ &verifying };
    uint8_t actual[1024];
    bzero(actual, sizeof(actual));
    EXPECT_EQ(reader.read(actual, sizeof(actual)), 16);
    EXPECT_EQ(memcmp(original, actual, sizeof(original)), 0);

    EXPECT_EQ(reader.read(actual, sizeof(actual)), 16);
    EXPECT_EQ(memcmp(original, actual, sizeof(original)), 0);

    EXPECT_EQ(reader.read(actual, sizeof(actual)), 16);
    EXPECT_EQ(memcmp(original, actual, sizeof(original)), 0);

    EXPECT_EQ(reader.read(actual, sizeof(actual)), 16);
    EXPECT_EQ(memcmp(original, actual, sizeof(original)), 0);

    EXPECT_EQ(reader.read(actual, sizeof(actual)), 0);

    // phydebug_dump_memory("actual ", actual, sizeof(actual));

    EXPECT_EQ(memcmp(original, actual, sizeof(original)), 0);
}

TEST_F(CobsFixture, Write_MultipleWrites_NullBordered) {
    malloc_write_buffer destination_buffer{ 1024 };
    malloc_write_buffer lookahead{ 256 };
    buffer_writer destination{ destination_buffer };
    cobs_writer writer{ &destination, lookahead };

    uint8_t original[16];
    for (auto i = 0u; i < sizeof(original); ++i) {
        original[i] = i;
    }
    original[15] = 0;

    EXPECT_GT(writer.write(original, sizeof(original)), 0);
    EXPECT_GT(writer.write(original, sizeof(original)), 0);
    EXPECT_GT(writer.write(original, sizeof(original)), 0);
    EXPECT_GT(writer.write(original, sizeof(original)), 0);

    // phydebug_dump_memory("encoded ", destination_buffer.ptr(), destination_buffer.position());

    read_buffer buffer = destination_buffer.read_back();
    buffer_reader verifying{ buffer };
    cobs_reader reader{ &verifying };
    uint8_t actual[1024];
    bzero(actual, sizeof(actual));
    EXPECT_EQ(reader.read(actual, sizeof(actual)), 16);
    EXPECT_EQ(memcmp(original, actual, sizeof(original)), 0);

    EXPECT_EQ(reader.read(actual, sizeof(actual)), 16);
    EXPECT_EQ(memcmp(original, actual, sizeof(original)), 0);

    EXPECT_EQ(reader.read(actual, sizeof(actual)), 16);
    EXPECT_EQ(memcmp(original, actual, sizeof(original)), 0);

    EXPECT_EQ(reader.read(actual, sizeof(actual)), 16);
    EXPECT_EQ(memcmp(original, actual, sizeof(original)), 0);

    EXPECT_EQ(reader.read(actual, sizeof(actual)), 0);

    // phydebug_dump_memory("actual ", actual, sizeof(actual));

    EXPECT_EQ(memcmp(original, actual, sizeof(original)), 0);
}
