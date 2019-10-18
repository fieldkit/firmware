#pragma once

#include "pool.h"
#include "io.h"
#include "circular_buffer.h"

namespace fk {

class SdCardFile : public Writer, public Reader {
public:
    virtual bool close() = 0;
    virtual size_t file_size() = 0;
    virtual operator bool() const = 0;
    virtual int32_t seek_beginning() = 0;
    virtual int32_t seek_end() = 0;

};

class SdCard {
public:
    virtual bool begin() = 0;

public:
    virtual bool append_logs(circular_buffer<char> &buffer) = 0;
    virtual bool is_file(const char *path) = 0;
    virtual bool is_directory(const char *path) = 0;
    virtual bool mkdir(const char *path) = 0;
    virtual bool unlink(const char *path) = 0;
    virtual SdCardFile *open(const char *path, bool writing, Pool &pool) = 0;
    virtual bool format() = 0;

};

SdCard *get_sd_card();

}
