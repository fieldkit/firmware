#include "delimited_buffer.h"

namespace phylum {

void *delimited_buffer::reserve(size_t length, sector_offset_t &start_position) {
    assert(buffer_.valid());

    // If this is our first record, write the offset value first.
    if (buffer_.position() == 0) {
        auto offset = 0u;
        auto needed = varint_encoding_length(offset);
        auto p = buffer_.take(needed);
        if (*p != 0xff) {
            phydebug_dump_memory("overwrite ", p, needed);
            assert(*p == 0xff);
        }
        varint_encode(offset, p, needed);
    }

    // Verify enough room for the record and its prefix.
    auto delimiter_overhead = varint_encoding_length(length);

    // Encode the length before the record.
    auto position_before = buffer_.position();
    auto p = buffer_.take(delimiter_overhead + length);
    assert(p != nullptr);

    varint_encode(length, p, delimiter_overhead);

    auto position_after = buffer_.position();

    phyverbosef("reserve: %zu (+%d) = %zu before=%zu after=%zu", length, delimiter_overhead,
                length + delimiter_overhead, position_before, position_after);

    // This is where we'll ask the caller to construct their record.
    auto allocated = p + delimiter_overhead;

    // TODO Check the whole region?
    if (*allocated != 0xff) {
        phydebug_dump_memory("overwrite ", allocated, length);
        assert(*allocated == 0xff);
    }

    start_position = position_before;

    return allocated;
}

} // namespace phylum
