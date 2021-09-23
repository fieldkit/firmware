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

bool TheThingsLoraNetwork::begin(lora_frequency_t frequency_band) {
    if (frequency_band_ != frequency_band) {
        if (powered_) {
            power(false);
            fk_delay(50);
        }
        status_ = Availability::Unknown;
    }

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

        if (pool_ == nullptr) {
            pool_ = create_standard_pool_inside("lora-ttn");
        } else {
            pool_->clear();
        }

        ttn_ = new (pool_) TheThingsNetwork{ stream_, debug_, (ttn_fp_t)frequency_band_ };

#if defined(FK_LORA_SET_UPLINK_COUNTER)
        Rn2903 rn2903;
        const char *line = nullptr;
        if (!rn2903.simple_query("mac get upctr", &line, 1000)) {
            return nullptr;
        }
        auto value = atoi(line);
        if (value < FK_LORA_SET_UP_COUNTER) {
            if (!rn2903.simple_query("mac set upctr %d", &line, 1000, FK_LORA_SET_UP_COUNTER)) {
                return false;
            }

            save_state();
        }
#endif
    }

    status_ = Availability::Available;

    return true;
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
        awake_ = true;
    } else {
        logdebug("power off");
        get_board()->disable_lora();
        powered_ = false;
        awake_ = false;
    }

    return true;
}

bool TheThingsLoraNetwork::sleep(uint32_t ms) {
    FK_ASSERT(ttn_ != nullptr);
    if (!powered_) {
        logwarn("unpowered sleep");
        return true;
    }
    ttn_->sleep(ms);
    awake_ = false;
    return true;
}

bool TheThingsLoraNetwork::wake() {
    FK_ASSERT(ttn_ != nullptr);
    if (!powered_) {
        logwarn("emergency power-on");
        return power(true);
    }
    ttn_->wake();
    awake_ = true;
    return true;
}

bool TheThingsLoraNetwork::factory_reset() {
    FK_ASSERT(ttn_ != nullptr);
    ttn_->reset(false);
    return true;
}

bool TheThingsLoraNetwork::send_bytes(uint8_t port, uint8_t const *data, size_t size, bool confirmed) {
    FK_ASSERT(ttn_ != nullptr);

    error_ = LoraErrorCode::None;

    switch (ttn_->sendBytes(data, size, port, confirmed)) {
    case TTN_ERROR_SEND_COMMAND_FAILED: {
        error_ = LoraErrorCode::ModuleIO;
        return false;
    }
    case TTN_ERROR_UNEXPECTED_RESPONSE: {
        error_ = LoraErrorCode::ModuleIO;
        return false;
    }
    case TTN_ERROR_MAC: {
        error_ = LoraErrorCode::Mac;
        return true;
    }
    case TTN_UNSUCESSFUL_RECEIVE: {
        return true;
    }
    case TTN_SUCCESSFUL_TRANSMISSION: {
        if (confirmed) {
            return save_state();
        }
        return true;
    }
    case TTN_SUCCESSFUL_RECEIVE: {
        if (confirmed) {
            return save_state();
        }
        return true;
    }
    }

    logwarn("ignoring unknown error");
    return true;
}

bool TheThingsLoraNetwork::join(LoraOtaaJoin &otaa, int32_t retries, uint32_t retry_delay) {
    FK_ASSERT(ttn_ != nullptr);

    if (!ttn_->join(otaa.join_eui, otaa.app_key, retries, retry_delay)) {
        return false;
    }

    save_state();

    return true;
}

bool TheThingsLoraNetwork::join_resume() {
    FK_ASSERT(ttn_ != nullptr);
    return ttn_->join_resume();
}

bool TheThingsLoraNetwork::save_state() {
    FK_ASSERT(ttn_ != nullptr);
    ttn_->saveState();
    return true;
}

bool TheThingsLoraNetwork::get_state(Rn2903State *state) {
    const char *line = nullptr;

    loginfo("module: getting state");

    Rn2903 rn2903;

    if (!rn2903.simple_query("sys get vdd", &line, 1000)) {
        return false;
    }

    if (!rn2903.simple_query("mac get status", &line, 1000)) {
        return nullptr;
    }

    if (!rn2903.simple_query("mac get dr", &line, 1000)) {
        return false;
    }

    if (!rn2903.simple_query("mac get adr", &line, 1000)) {
        return false;
    }

    if (!rn2903.simple_query("mac get rxdelay1", &line, 1000)) {
        return false;
    }

    if (!rn2903.simple_query("mac get rxdelay2", &line, 1000)) {
        return false;
    }

    if (!rn2903.simple_query("sys get hweui", &line, 1000)) {
        return false;
    }

    if (!rn2903.simple_query("mac get deveui", &line, 1000)) {
        return false;
    }
    FK_ASSERT(hex_string_to_bytes(state->device_eui, sizeof(state->device_eui), line) == sizeof(state->device_eui));

    if (!rn2903.simple_query("mac get appeui", &line, 1000)) {
        return false;
    }
    FK_ASSERT(hex_string_to_bytes(state->join_eui, sizeof(state->join_eui), line) == sizeof(state->join_eui));

    if (!rn2903.simple_query("mac get devaddr", &line, 1000)) {
        return false;
    }
    if (strlen(line) != sizeof(state->device_address) * 2) {
        loginfo("module: invalid devaddr, too long");
        bzero(state->device_address, sizeof(state->device_address));
    } else {
        FK_ASSERT(hex_string_to_bytes(state->device_address, sizeof(state->device_address), line) == sizeof(state->device_address));
    }

    if (!rn2903.simple_query("mac get upctr", &line, 1000)) {
        return false;
    }
    state->uplink_counter = atoi(line);

    if (!rn2903.simple_query("mac get dnctr", &line, 1000)) {
        return false;
    }
    state->downlink_counter = atoi(line);

    if (!rn2903.simple_query("mac get pwridx", &line, 1000)) {
        return nullptr;
    }
    state->power_index = atoi(line);

    return true;
}

Rn2903State *TheThingsLoraNetwork::get_state(Pool &pool) {
    FK_ASSERT(ttn_ != nullptr);
    auto state = new (pool) Rn2903State();
    if (!get_state(state)) {
        return nullptr;
    }
    return state;
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

bool Rn2903LoraNetwork::save_state() {
    return rn2903_.save_state();
}

bool Rn2903LoraNetwork::power(bool on) {
    if (on) {
        logdebug("power on");
        get_board()->enable_lora();
        powered_ = true;
        awake_ = true;
    } else {
        logdebug("power off");
        get_board()->disable_lora();
        powered_ = false;
        awake_ = false;
    }

    return true;
}

bool Rn2903LoraNetwork::sleep(uint32_t ms) {
    if (!powered_) {
        logwarn("powerless sleep");
        return true;
    }
    if (awake_) {
        awake_ = false;
        return rn2903_.sleep(ms);
    }
    return true;
}

bool Rn2903LoraNetwork::wake() {
    if (!powered_) {
        logwarn("emergency power-on");
        return power(true);
    }
    if (!awake_) {
        awake_ = true;
        return rn2903_.wake();
    }
    return true;
}

bool Rn2903LoraNetwork::factory_reset() {
    return rn2903_.factory_reset();
}

bool Rn2903LoraNetwork::begin(lora_frequency_t frequency_band) {
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

bool Rn2903LoraNetwork::send_bytes(uint8_t port, uint8_t const *data, size_t size, bool confirmed) {
    // TODO This needs to handle errors like the TTN implementation.
    FK_ASSERT(0);
    return false;
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
