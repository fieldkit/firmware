#include "hal/metal/metal_lora.h"
#include "hal/board.h"
#include "hal/metal/sc16is740.h"
#include "utilities.h"

#include <Arduino.h>

namespace fk {

constexpr uint32_t DefaultTimeout = 10000;

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
        if (!power(false)) {
            return false;
        }

        fk_delay(50);

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
        stream_.setTimeout(1000);
        stream_.readBytesUntil('\n', buffer, sizeof(buffer));
        stream_.setTimeout(DefaultTimeout);

        loginfo("version: %s", buffer);

        if (pool_ == nullptr) {
            pool_ = create_standard_pool_inside("lora-ttn");
        } else {
            pool_->clear();
        }

        frequency_band_ = frequency_band;

        auto ttnfp = TTN_FP_US915;
        switch (frequency_band_) {
        case lora_frequency_t::Us915:
            ttnfp = TTN_FP_US915;
            loginfo("frequency-band: Us915");
            break;
        case lora_frequency_t::Eu868:
            ttnfp = TTN_FP_EU868;
            loginfo("frequency-band: Eu868");
            break;
        default:
            FK_ASSERT(false);
            break;
        }

        ttn_ = new (pool_) TheThingsNetwork{ stream_, debug_, ttnfp };

#if defined(FK_LORA_SET_UPLINK_COUNTER)
        Rn2903 rn2903;
        const char *line = nullptr;
        if (!rn2903.simple_query("mac get upctr", &line, DefaultTimeout)) {
            return nullptr;
        }
        auto value = atoi(line);
        if (value < FK_LORA_SET_UPLINK_COUNTER) {
            if (!rn2903.simple_query("mac set upctr %d", &line, DefaultTimeout, FK_LORA_SET_UPLINK_COUNTER)) {
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
    if (status_ == Availability::Available) {
        power(false);
    }

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
    if (!powered_) {
        logwarn("unpowered sleep");
        return true;
    }

    Rn2903 rn2903;
    rn2903.sleep(ms);

    awake_ = false;
    return true;
}

bool TheThingsLoraNetwork::wake() {
    FK_ASSERT(ttn_ != nullptr);
    if (!powered_) {
        logwarn("emergency power-on");
        return power(true);
    }

    Rn2903 rn2903;
    rn2903.wake();

    awake_ = true;
    return true;
}

bool TheThingsLoraNetwork::factory_reset() {
    FK_ASSERT(ttn_ != nullptr);
    Rn2903 rn2903;
    return rn2903.factory_reset();
}

bool TheThingsLoraNetwork::send_bytes(uint8_t port, uint8_t const *data, size_t size, bool confirmed) {
    FK_ASSERT(ttn_ != nullptr);

    error_ = LoraErrorCode::None;

#if defined(FK_LORA_TTN_SEND)
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
        // Our return value indicates a critical failure of some kind and
        // usually means we kill the active task. This is failed confirmation
        // and we can respond to that.
        error_ = LoraErrorCode::Mac;
        return true;
    }
    case TTN_UNSUCESSFUL_RECEIVE: {
        return true;
    }
    case TTN_SUCCESSFUL_TRANSMISSION: {
        return true;
    }
    case TTN_SUCCESSFUL_RECEIVE: {
        return true;
    }
    }

    logwarn("ignoring unknown error");
    return true;
#else
    Rn2903 rn2903;
    auto send = rn2903.send_bytes(data, size, port, confirmed);
    error_ = rn2903.error();
    return send;
#endif
}

bool TheThingsLoraNetwork::join(LoraOtaaJoin &otaa, int32_t retries, uint32_t retry_delay) {
    FK_ASSERT(ttn_ != nullptr);

    Rn2903 rn2903;

    // NOTE: This is required anytime you save parameters. Per the RN2903
    // manual, If this parameter was previously saved to user EEPROM by issuing
    // the mac save command, after modifying its value, the mac save command
    // should be called again.
    // https://www.loraserver.io/lora-app-server/use/devices/#to-set-the-appeui-and-appkey

    if (!rn2903.simple_query("mac set devaddr %s", DefaultTimeout, "00000000")) {
        return false;
    }
    if (!rn2903.simple_query("mac set nwkskey %s", DefaultTimeout, "00000000000000000000000000000000")) {
        return false;
    }
    if (!rn2903.simple_query("mac set appskey %s", DefaultTimeout, "00000000000000000000000000000000")) {
        return false;
    }
    if (!rn2903.save_state()) {
        return false;
    }

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

    if (!rn2903.simple_query("sys get vdd", &line, DefaultTimeout)) {
        return false;
    }

    if (!rn2903.simple_query("mac get status", &line, DefaultTimeout)) {
        return nullptr;
    }

    if (!rn2903.simple_query("mac get dr", &line, DefaultTimeout)) {
        return false;
    }

    if (!rn2903.simple_query("mac get adr", &line, DefaultTimeout)) {
        return false;
    }

    if (!rn2903.simple_query("mac get rxdelay1", &line, DefaultTimeout)) {
        return false;
    }
    state->rx_delay_1 = atoi(line);

    if (!rn2903.simple_query("mac get rxdelay2", &line, DefaultTimeout)) {
        return false;
    }
    state->rx_delay_2 = atoi(line);

    if (!rn2903.simple_query("sys get hweui", &line, DefaultTimeout)) {
        return false;
    }

    if (!rn2903.simple_query("mac get deveui", &line, DefaultTimeout)) {
        return false;
    }
    FK_ASSERT(hex_string_to_bytes(state->device_eui, sizeof(state->device_eui), line) == sizeof(state->device_eui));

    if (!rn2903.simple_query("mac get appeui", &line, DefaultTimeout)) {
        return false;
    }
    FK_ASSERT(hex_string_to_bytes(state->join_eui, sizeof(state->join_eui), line) == sizeof(state->join_eui));

    if (!rn2903.simple_query("mac get devaddr", &line, DefaultTimeout)) {
        return false;
    }
    if (strlen(line) != sizeof(state->device_address) * 2) {
        loginfo("module: invalid devaddr, too long");
        bzero(state->device_address, sizeof(state->device_address));
    } else {
        FK_ASSERT(hex_string_to_bytes(state->device_address, sizeof(state->device_address), line) == sizeof(state->device_address));
    }

    if (!rn2903.simple_query("mac get upctr", &line, DefaultTimeout)) {
        return false;
    }
    state->uplink_counter = atoi(line);

    if (!rn2903.simple_query("mac get dnctr", &line, DefaultTimeout)) {
        return false;
    }
    state->downlink_counter = atoi(line);

    if (!rn2903.simple_query("mac get pwridx", &line, DefaultTimeout)) {
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
        awake_ = false;
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
        auto powered = power(true);
        fk_delay(200);
        return powered;
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

    if (!rn2903_.simple_query("sys get vdd", &line, DefaultTimeout)) {
        return false;
    }

    if (!rn2903_.simple_query("mac get status", &line, DefaultTimeout)) {
        return nullptr;
    }

    if (!rn2903_.simple_query("mac get dr", &line, DefaultTimeout)) {
        return false;
    }

    if (!rn2903_.simple_query("mac get adr", &line, DefaultTimeout)) {
        return false;
    }

    if (!rn2903_.simple_query("mac get rxdelay1", &line, DefaultTimeout)) {
        return false;
    }
    state->rx_delay_1 = atoi(line);

    if (!rn2903_.simple_query("mac get rxdelay2", &line, DefaultTimeout)) {
        return false;
    }
    state->rx_delay_2 = atoi(line);

    if (!rn2903_.simple_query("sys get hweui", &line, DefaultTimeout)) {
        return false;
    }

    if (!rn2903_.simple_query("mac get deveui", &line, DefaultTimeout)) {
        return nullptr;
    }
    FK_ASSERT(hex_string_to_bytes(state->device_eui, sizeof(state->device_eui), line) == sizeof(state->device_eui));

    if (!rn2903_.simple_query("mac get appeui", &line, DefaultTimeout)) {
        return nullptr;
    }
    FK_ASSERT(hex_string_to_bytes(state->join_eui, sizeof(state->join_eui), line) == sizeof(state->join_eui));

    if (!rn2903_.simple_query("mac get devaddr", &line, DefaultTimeout)) {
        return nullptr;
    }
    FK_ASSERT(hex_string_to_bytes(state->device_address, sizeof(state->device_address), line) == sizeof(state->device_address));

    if (!rn2903_.simple_query("mac get upctr", &line, DefaultTimeout)) {
        return nullptr;
    }
    state->uplink_counter = atoi(line);

    if (!rn2903_.simple_query("mac get dnctr", &line, DefaultTimeout)) {
        return nullptr;
    }
    state->downlink_counter = atoi(line);

    if (!rn2903_.simple_query("mac get pwridx", &line, DefaultTimeout)) {
        return nullptr;
    }
    state->power_index = atoi(line);

    return state;
}

} // namespace fk
