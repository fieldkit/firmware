#pragma once

#include "writer.h"
#include "reader.h"
#include "simple_buffer.h"

namespace phylum {

class cobs_writer : public io_writer {
private:
    io_writer *target_{ nullptr };
    write_buffer &buffer_;
    bool return_bytes_wrote_{ true };

public:
    cobs_writer(io_writer *target, write_buffer &buffer);
    virtual ~cobs_writer();

public:
    void return_bytes_wrote(bool value) {
        return_bytes_wrote_ = value;
    }

public:
    int32_t write(uint8_t const *data, size_t size) override;

};

class cobs_reader : public io_reader {
private:
    io_reader *target_{ nullptr };

public:
    cobs_reader(io_reader *target);

public:
    int32_t read(uint8_t *data, size_t size) override;

};

} // namespace phylum
