#include "hal/metal/metal_lora.h"
#include "hal/board.h"
#include "hal/metal/sc16is740.h"
#include "utilities.h"

#include <Arduino.h>

namespace fk {

FK_DECLARE_LOGGER("lora");

TwoWireExtenderStream::TwoWireExtenderStream(Sc16is740 &bridge) : bridge_(&bridge) {
}

int TwoWireExtenderStream::available() {
    return bridge_->available_for_read();
}

int TwoWireExtenderStream::read() {
    if (peek_ >= 0) {
        auto c = peek_;
        peek_ = -1;
        return c;
    }

    uint8_t value{ 0 };
    if (bridge_->read(&value, sizeof(value)) != 1) {
        return -1;
    }

    return value;
}

int TwoWireExtenderStream::peek() {
    if (peek_ >= 0) {
        return peek_;
    }

    if (bridge_->available_for_read() <= 0) {
        logwarn("peek: eos");
        return -1;
    }

    uint8_t value{ 0 };
    if (bridge_->read(&value, sizeof(value)) != 1) {
        return -1;
    }

    peek_ = value;

    return peek_;
}

size_t TwoWireExtenderStream::write(uint8_t byte) {
    if (!bridge_->write(byte)) {
        return 0;
    }
    return 1;
}

DebugStream::DebugStream() {
    position_ = 0;
    buffer_[0] = 0;
}

size_t DebugStream::write(uint8_t byte) {
    FK_ASSERT(position_ < sizeof(buffer_) - 1);
    if (byte == '\r' || byte == '\n') {
        if (position_ > 0) {
            buffer_[position_] = 0;
            loginfo("debug: %s", buffer_);
            position_ = 0;
            buffer_[position_] = 0;
        }
    } else {
        buffer_[position_] = byte;
        position_++;
    }
    return 1;
}

TheThingsLoraNetwork::TheThingsLoraNetwork() : bridge_(get_board()->acquire_i2c_radio()) {
}

bool TheThingsLoraNetwork::begin() {
    if (status_ == Availability::Available && powered_) {
        return true;
    }

    status_ = Availability::Unavailable;

    if (!powered_) {
        if (!power(true)) {
            return false;
        }

        fk_delay(50);

        if (!bridge_.begin(57600)) {
            logwarn("bridge begin");
            return false;
        }

        fk_delay(200);

        uint8_t buffer[32];
        stream_.readBytesUntil('\n', buffer, sizeof(buffer));

        loginfo("version: %s", buffer);
    }

    ttn_.reset(false);

    ttn_.showStatus();

    const char *join_eui = "0000000000000000";
    const char *app_key = "00000000000000000000000000000000";

    ttn_.join(join_eui, app_key, 1);

    status_ = Availability::Available;

    return false;
}

bool TheThingsLoraNetwork::stop() {
    power(false);

    return true;
}

bool TheThingsLoraNetwork::power(bool on) {
    if (on) {
        logdebug("power on");
        get_board()->enable_lora();
        powered_ = true;
    } else {
        logdebug("power off");
        get_board()->disable_lora();
        powered_ = false;
    }

    return true;
}

bool TheThingsLoraNetwork::sleep(uint32_t ms) {
    return false;
}

bool TheThingsLoraNetwork::wake() {
    return false;
}

bool TheThingsLoraNetwork::factory_reset() {
    return false;
}

bool TheThingsLoraNetwork::configure_tx(uint8_t power_index, uint8_t data_rate) {
    return false;
}

bool TheThingsLoraNetwork::send_bytes(uint8_t port, uint8_t const *data, size_t size, bool confirmed) {
    return false;
}

bool TheThingsLoraNetwork::join(LoraOtaaJoin &otaa, int32_t retries, uint32_t retry_delay) {
    return false;
}

bool TheThingsLoraNetwork::join_resume() {
    return false;
}

bool TheThingsLoraNetwork::resume_previous_session() {
    return false;
}

bool TheThingsLoraNetwork::save_state() {
    return false;
}

Rn2903LoraNetwork::Rn2903LoraNetwork() {
}

bool Rn2903LoraNetwork::join(LoraOtaaJoin &otaa, int32_t retries, uint32_t retry_delay) {
    if (!rn2903_.join(otaa, retries, retry_delay)) {
        return false;
    }

    if (!rn2903_.disable_adr()) {
        return false;
    }

    return true;
}

bool Rn2903LoraNetwork::join_resume() {
    if (!rn2903_.join("abp")) {
        return false;
    }

    if (!rn2903_.disable_adr()) {
        return false;
    }

    return true;
}

bool Rn2903LoraNetwork::resume_previous_session() {
    if (!rn2903_.join("abp")) {
        return false;
    }

    return true;
}

bool Rn2903LoraNetwork::save_state() {
    return rn2903_.save_state();
}

bool Rn2903LoraNetwork::power(bool on) {
    if (on) {
        logdebug("power on");
        get_board()->enable_lora();
        powered_ = true;
    } else {
        logdebug("power off");
        get_board()->disable_lora();
        powered_ = false;
    }

    return true;
}

bool Rn2903LoraNetwork::sleep(uint32_t ms) {
    return rn2903_.sleep(ms);
}

bool Rn2903LoraNetwork::wake() {
    return rn2903_.wake();
}

bool Rn2903LoraNetwork::factory_reset() {
    return rn2903_.factory_reset();
}

bool Rn2903LoraNetwork::begin() {
    if (status_ == Availability::Available && powered_) {
        return true;
    }

    status_ = Availability::Unavailable;

    if (!power(true)) {
        return false;
    }

    fk_delay(100);

    if (!rn2903_.begin()) {
        return false;
    }

    const char *line = nullptr;
    if (!rn2903_.read_line_sync(&line, 5000)) {
        return false;
    }

    status_ = Availability::Available;

    return true;
}

bool Rn2903LoraNetwork::stop() {
    power(false);

    return true;
}

bool Rn2903LoraNetwork::configure_tx(uint8_t power_index, uint8_t data_rate) {
    if (!rn2903_.simple_query("mac set pwridx %d", 1000, power_index)) {
        return false;
    }

    if (!rn2903_.simple_query("mac set dr %d", 1000, data_rate)) {
        return false;
    }

    return true;
}

bool Rn2903LoraNetwork::send_bytes(uint8_t port, uint8_t const *data, size_t size, bool confirmed) {
    if (!rn2903_.send_bytes(data, size, port, confirmed)) {
        return false;
    }

    return true;
}

Rn2903State *Rn2903LoraNetwork::get_state(Pool &pool) {
    const char *line = nullptr;

    auto state = new (pool) Rn2903State();

    loginfo("module: getting state");

    if (!rn2903_.simple_query("sys get vdd", &line, 1000)) {
        return false;
    }

    if (!rn2903_.simple_query("mac get status", &line, 1000)) {
        return nullptr;
    }

    if (!rn2903_.simple_query("mac get dr", &line, 1000)) {
        return false;
    }

    if (!rn2903_.simple_query("mac get adr", &line, 1000)) {
        return false;
    }

    if (!rn2903_.simple_query("mac get rxdelay1", &line, 1000)) {
        return false;
    }

    if (!rn2903_.simple_query("mac get rxdelay2", &line, 1000)) {
        return false;
    }

    if (!rn2903_.simple_query("sys get hweui", &line, 1000)) {
        return false;
    }

    if (!rn2903_.simple_query("mac get deveui", &line, 1000)) {
        return nullptr;
    }
    FK_ASSERT(hex_string_to_bytes(state->device_eui, sizeof(state->device_eui), line) == sizeof(state->device_eui));

    if (!rn2903_.simple_query("mac get appeui", &line, 1000)) {
        return nullptr;
    }
    FK_ASSERT(hex_string_to_bytes(state->join_eui, sizeof(state->join_eui), line) == sizeof(state->join_eui));

    if (!rn2903_.simple_query("mac get devaddr", &line, 1000)) {
        return nullptr;
    }
    FK_ASSERT(hex_string_to_bytes(state->device_address, sizeof(state->device_address), line) == sizeof(state->device_address));

    if (!rn2903_.simple_query("mac get upctr", &line, 1000)) {
        return nullptr;
    }
    state->uplink_counter = atoi(line);

    if (!rn2903_.simple_query("mac get dnctr", &line, 1000)) {
        return nullptr;
    }
    state->downlink_counter = atoi(line);

    if (!rn2903_.simple_query("mac get pwridx", &line, 1000)) {
        return nullptr;
    }
    state->power_index = atoi(line);

    return state;
}

} // namespace fk
