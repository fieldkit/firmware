#include "common.h"
#include "protobuf.h"
#include "pool.h"
#include "config.h"

namespace fk {

bool pb_encode_string(pb_ostream_t *stream, const pb_field_t *field, void *const *arg) {
    if (arg == nullptr) {
        return true;
    }

    if (!pb_encode_tag_for_field(stream, field)) {
        return false;
    }

    auto str = (const char *)*arg;
    if (str == nullptr) {
        return pb_encode_string(stream, (uint8_t *)nullptr, 0);
    }

    return pb_encode_string(stream, (uint8_t *)str, strlen(str));
}

bool pb_decode_string(pb_istream_t *stream, const pb_field_t *, void **arg) {
    auto pool = (Pool *)(*arg);
    auto len = stream->bytes_left;

    if (len == 0) {
        (*arg) = (void *)"";
        return true;
    }

    auto *ptr = (uint8_t *)pool->malloc(len + 1);
    if (!pb_read(stream, ptr, len)) {
        return false;
    }

    ptr[len] = 0;

    (*arg) = (void *)ptr;

    return true;
}

bool pb_encode_array(pb_ostream_t *stream, const pb_field_t *field, void *const *arg) {
    auto array = (pb_array_t *)*arg;
    if (array == nullptr) {
        return true;
    }

    auto ptr = (uint8_t *)array->buffer;
    if (array->fields != nullptr) {
        for (size_t i = 0; i < array->length; ++i) {
            if (!pb_encode_tag_for_field(stream, field)) {
                return false;
            }

            if (!pb_encode_submessage(stream, array->fields, ptr)) {
                return false;
            }

            ptr += array->itemSize;
        }
    }
    else {
        auto length = array->itemSize * array->length;

        // Confusing that we're using PB_WT_STRING here, I know. This is wire
        // type 2. Whic his "length delimited" and used fro string, bytes,
        // embedded messages and packed repeated fields (us here)
        if (!pb_encode_tag(stream, PB_WT_STRING, field->tag)) {
            return false;
        }

        if (!pb_encode_varint(stream, length)) {
            return false;
        }

        if (!pb_write(stream, ptr, length)) {
            return false;
        }
    }

    return true;
}

bool pb_encode_uint32_array(pb_ostream_t *stream, const pb_field_t *field, void *const *arg) {
    auto array = (pb_array_t *)*arg;
    if (array == nullptr) {
        return true;
    }

    auto ptr = (uint32_t *)array->buffer;
    for (size_t i = 0; i < array->length; ++i) {
        if (!pb_encode_tag_for_field(stream, field)) {
            return false;
        }

        if (!pb_encode_varint(stream, *ptr)) {
            return false;
        }

        ptr++;
    }

    return true;
}

bool pb_decode_array(pb_istream_t *stream, const pb_field_t *field, void **arg) {
    auto array = (pb_array_t *)*arg;

    if (array->decode_item_fn != nullptr) {
        return array->decode_item_fn(stream, array);
    }

    return true;
}

bool pb_encode_data(pb_ostream_t *stream, const pb_field_t *field, void *const *arg) {
    auto data = (pb_data_t *)*arg;

    if (data == nullptr) {
        return true;
    }

    if (!pb_encode_tag_for_field(stream, field)) {
        return false;
    }

    if (!pb_encode_varint(stream, data->length)) {
        return false;
    }

    auto ptr = (uint8_t *)data->buffer;
    if (ptr != nullptr) {
        if (!pb_write(stream, ptr, data->length)) {
            return false;
        }
    }

    return true;
}

pb_data_t *pb_data_allocate(Pool *pool, size_t size) {
    auto data = reinterpret_cast<pb_data_t *>(pool->malloc(sizeof(pb_data_t) + size));
    data->buffer = ((uint8_t *)data) + sizeof(pb_data_t);
    data->length = size;
    return data;
}

bool pb_decode_data(pb_istream_t *stream, const pb_field_t *field, void **arg) {
    auto pool = (Pool *)(*arg);
    auto length = stream->bytes_left;
    auto data = pb_data_allocate(pool, length);

    if (!pb_read(stream, (pb_byte_t *)data->buffer, length)) {
        return false;
    }

    (*arg) = (void *)data;

    return true;
}

static size_t pb_varint_size(uint32_t low, uint32_t high) {
    size_t i = 0;
    low >>= 7;

    while (i < 4 && (low != 0 || high != 0)) {
        i++;
        low >>= 7;
    }

    if (high) {
        high >>= 3;
        while (high) {
            i++;
            high >>= 7;
        }
    }

    i++;

    return i;
}

size_t pb_varint_size(uint32_t value) {
    if (value <= 0x7F) {
        return 1;
    }
    else {
        return pb_varint_size(value, 0);
    }
}

pb_data_t *pb_get_data_if_provided(void *arg, Pool &pool) {
    if (arg == &pool) {
        return nullptr;
    }
    return reinterpret_cast<pb_data_t *>(arg);
}

const char *pb_get_string_if_provided(void *arg, Pool &pool) {
    if (arg == &pool) {
        return nullptr;
    }
    return (const char *)arg;
}

const char *pb_data_to_hex_string(pb_data_t const *data, Pool &pool) {
    return bytes_to_hex_string_pool((uint8_t *)data->buffer, data->length, pool);
}

pb_data_t *pb_data_create(void const *data, size_t size, Pool &pool) {
    auto copy = pool.copy(data, size);
    return pool.malloc_with<pb_data_t>({
        .length = size,
        .buffer = copy,
    });
}

bool pb_encode_logs(pb_ostream_t *stream, const pb_field_t *field, void * const *arg) {
    if (!pb_encode_tag_for_field(stream, field)) {
        return false;
    }

    // Calculate the size of the string we're going to write and then
    // write that to the stream.
    auto size = 0u;
    auto &lb = fk_log_buffer();
    for (auto c : lb) {
        if (c != 0) {
            size++;
        }
    }

    if (!pb_encode_varint(stream, size)) {
        return false;
    }

    // Write the logs, buffered to avoid tons of calls.
    auto position = 0u;
    auto copied = 0u;
    uint8_t buffer[StackBufferSize];
    for (auto c : lb) {
        // Keep ourselves from somehow accidentally writing more than
        // we planned to write.
        if (copied == size) {
            break;
        }
        if (c != 0) {
            buffer[position++] = c;
            copied++;
            if (position == StackBufferSize) {
                pb_write(stream, buffer, position);
                position = 0;
            }
        }
    }

    if (position > 0) {
        pb_write(stream, buffer, position);
    }

    return true;
}

} // namespace fk
