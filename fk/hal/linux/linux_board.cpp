#include "hal/board.h"
#include "platform.h"

#if defined(linux)

namespace fk {

FK_DECLARE_LOGGER("board");

bool Board::initialize() {
    disable_everything();

    return true;
}

void Board::disable_everything() {
    spi_flash().end();
    spi_radio().end();
    spi_module().end();
    i2c_core().end();
    i2c_radio().end();
    i2c_module().end();
    disable_gps();
}

void Board::enable_everything() {
    enable_gps();
}

void Board::disable_gps() {
}

void Board::enable_gps() {
}

SpiWrapper Board::spi_flash() {
    return { "spi-flash", nullptr };
}

SpiWrapper Board::spi_sd() {
    return { "spi-sd", nullptr };
}

SpiWrapper Board::spi_radio() {
    return { "spi-radio", nullptr };
}

SpiWrapper Board::spi_module() {
    return { "spi-mod", nullptr };
}

TwoWireWrapper Board::i2c_core() {
    return TwoWireWrapper{ nullptr, "i2c-core", nullptr };
}

TwoWireWrapper Board::i2c_radio() {
    return TwoWireWrapper{ nullptr, "i2c-radio", nullptr };
}

TwoWireWrapper Board::i2c_module() {
    return TwoWireWrapper{ nullptr, "i2c-mod", nullptr };
}

SerialWrapper Board::gps_serial() {
    return { "uart-gps", nullptr };
}

SpiWrapper::SpiWrapper(const char *name, void *ptr) : name_(name), ptr_(ptr) {
    logverbose("acquire %s", name_);
}

SpiWrapper::~SpiWrapper() {
    logverbose("release %s", name_);
}

void SpiWrapper::begin() {
}

void SpiWrapper::end() {
}

TwoWireWrapper::TwoWireWrapper(Mutex *lock, const char *name, void *ptr) : lock_(lock), name_(name), ptr_(ptr) {
    logverbose("acquire %s", name_);
}

TwoWireWrapper::~TwoWireWrapper() {
    logverbose("release %s", name_);
}

void TwoWireWrapper::begin() {
}

int32_t TwoWireWrapper::read(uint8_t address, void *data, int32_t size, TwoWireFlags flags) {
    return -1;
}

int32_t TwoWireWrapper::write(uint8_t address, const void *data, int32_t size, TwoWireFlags flags) {
    return -1;
}

void TwoWireWrapper::end() {
}

int32_t TwoWireWrapper::recover() {
    return -1;
}

SerialWrapper::SerialWrapper(const char *name, void *ptr) : name_(name), ptr_(ptr) {
    logverbose("acquire %s", name_);
}

SerialWrapper::~SerialWrapper() {
    logverbose("release %s", name_);
}

bool SerialWrapper::begin(uint32_t baud) {
    return true;
}

bool SerialWrapper::end() {
    return true;
}

int32_t SerialWrapper::available() {
    return 0;
}

int8_t SerialWrapper::read() {
    return -1;
}

} // namespace fk

#endif
