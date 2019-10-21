#include "base64.h"

namespace fk {

Base64Reader::Base64Reader(Reader *target) : target_(target) {
}

static size_t hexchr2bin(uint8_t const hex, uint8_t *p) {
    if (hex >= '0' && hex <= '9') {
        *p = hex - '0';
    }
    else if (hex >= 'A' && hex <= 'F') {
        *p = hex - 'A' + 10;
    }
    else if (hex >= 'a' && hex <= 'f') {
        *p = hex - 'a' + 10;
    }
    else {
        return 0;
    }
    return 1;
}

int32_t Base64Reader::read(uint8_t *buffer, size_t size) {
    for (size_t i = 0; i < size; ++i) {
        uint8_t encoded[2];
        if (target_->read(encoded, sizeof(encoded)) != 2) {
            return i;
        }
        uint8_t decoded[2];
        if (!hexchr2bin(encoded[0], &decoded[0]) || !hexchr2bin(encoded[1], &decoded[1])) {
            return i;
        }
        buffer[i] = (decoded[0] << 4) | decoded[1];
    }
    return size;
}

Base64Writer::Base64Writer(Writer *target) : target_(target) {
}

int32_t Base64Writer::write(uint8_t const *buffer, size_t size) {
    constexpr static char Hex[] = "0123456789ABCDEF";

    for (size_t i = 0; i < size; i++) {
        char encoded[2];
        encoded[0] = Hex[buffer[i] >> 4];
        encoded[1] = Hex[buffer[i] & 0x0F];
        if (target_->write((uint8_t const *)encoded, sizeof(encoded)) != sizeof(encoded)) {
            return i;
        }
    }
    return size;
}

}
