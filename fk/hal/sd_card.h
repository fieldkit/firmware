#pragma once

#include <fk-app-protocol.h>

#include "pool.h"
#include "io.h"
#include "circular_buffer.h"
#include "collections.h"

namespace fk {

class SdCardFile : public Writer, public Reader {
public:
    virtual bool close() = 0;
    virtual size_t file_size() = 0;
    virtual int32_t seek_beginning() = 0;
    virtual int32_t seek_end() = 0;
    virtual int32_t seek_from_end(int32_t offset) = 0;
    virtual bool is_open() const = 0;

    operator bool() const {
        return is_open();
    }

};

enum OpenFlags { Read = 1, Write = 2, Append = 4};

/*
inline OpenFlags operator|(OpenFlags a, OpenFlags b) {
    return static_cast<OpenFlags>(static_cast<int>(a) | static_cast<int>(b));
}
*/

class SdCard {
public:
    virtual bool begin() = 0;

public:
    virtual bool append_logs(circular_buffer<char> &buffer) = 0;
    virtual bool append_logs(circular_buffer<char> &buffer, circular_buffer<char>::iterator iter) = 0;
    virtual bool append_logs(uint8_t const *buffer, size_t size) = 0;
    virtual bool close_logs() = 0;
    virtual bool is_file(const char *path) = 0;
    virtual bool is_directory(const char *path) = 0;
    virtual bool mkdir(const char *path) = 0;
    virtual bool unlink(const char *path) = 0;
    virtual SdCardFile *open(const char *path, OpenFlags flags, Pool &pool) = 0;
    virtual bool format() = 0;
    virtual bool ls(const char *path, size_t skip, fk_app_DirectoryEntry **files, size_t &number_entries,
                    size_t &total_entries, Pool &pool) {
        return true;
    }
    virtual void name(const char *name) {
    }
};

SdCard *get_sd_card();

} // namespace fk
