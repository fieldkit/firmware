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

SdCardFile *LinuxSdCard::open(const char *name, Pool &pool) {
    return new (pool) LinuxSdCardFile();
}

int32_t LinuxSdCardFile::write(uint8_t const *buffer, size_t size) {
    return 0;
}

size_t LinuxSdCardFile::file_size() {
    return 0;
}

bool LinuxSdCardFile::close() {
    return true;
}

}
