#include "hal/linux/linux.h"

namespace fk {

LinuxSdCard::LinuxSdCard() {
}

bool LinuxSdCard::begin() {
    return true;
}

bool LinuxSdCard::append_logs(circular_buffer<char> &buffer) {
    return true;
}

bool LinuxSdCard::append_logs(circular_buffer<char> &buffer, circular_buffer<char>::iterator iter) {
    return true;
}

bool LinuxSdCard::close_logs() {
    return true;
}

bool LinuxSdCard::is_file(const char *path) {
    return false;
}

bool LinuxSdCard::is_directory(const char *path) {
    return false;
}

bool LinuxSdCard::mkdir(const char *path) {
    return true;
}

bool LinuxSdCard::unlink(const char *path) {
    return false;
}

SdCardFile *LinuxSdCard::open(const char *path, bool writing, Pool &pool) {
    return new (pool) LinuxSdCardFile();
}

bool LinuxSdCard::format() {
    return false;
}

int32_t LinuxSdCardFile::write(uint8_t const *buffer, size_t size) {
    return 0;
}

int32_t LinuxSdCardFile::read(uint8_t *buffer, size_t size) {
    return 0;
}

int32_t LinuxSdCardFile::seek_beginning() {
    return 0;
}

int32_t LinuxSdCardFile::seek_end() {
    return 0;
}

size_t LinuxSdCardFile::file_size() {
    return 0;
}

bool LinuxSdCardFile::close() {
    return true;
}

}
