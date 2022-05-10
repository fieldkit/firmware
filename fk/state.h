#pragma once

#include "state/power_state.h"
#include "state/storage_state.h"
#include "state/schedule_state.h"
#include "state/network_state.h"
#include "state/lora_state.h"
#include "state/gps_state.h"
#include "state/dynamic.h"

#include "common.h"
#include "config.h"
#include "pool.h"

namespace fk {

struct RuntimeState {
    uint32_t startup_time{ 0 };
    uint32_t readings{ 0 };
};

struct GeneralState {
    char name[MaximumNameLength];
    uint8_t generation[GenerationLength];
    uint32_t recording{ 0 };
};

struct ProgressState {
    const char *operation;
    float progress;
};

struct NotificationState {
    uint32_t created{ 0 };
    const char *message{ nullptr };
    uint32_t delay{ 0 };

    explicit NotificationState() {
    }

    explicit NotificationState(const char *message) : created(fk_uptime()), message(message), delay(0) {
    }
};

struct OpenMenu {
    uint32_t time;
    bool readings;
};

struct DisplayState {
    OpenMenu open_menu;
};

struct ReadingsState {
    uint32_t nreadings{ 0 };
    uint32_t time{ 0 };
};

struct DebuggingState {
    DebuggingUdpTraffic udp_traffic;
    bool unexciting{ false };
};

struct GlobalState {
public:
    uint32_t version{ 0 };
    state::DynamicState dynamic;
    GeneralState general{};
    RuntimeState runtime{};
    PowerState power{};
    GpsState gps{};
    MainNetworkState network{};
    NotificationState notification{};
    DisplayState display{};
    ProgressState progress{};
    StorageState storage{};
    LoraState lora{};
    SchedulerState scheduler{};
    SdCardState sd_card{};
    ReadingsState readings{};
    TransmissionState transmission{};
    DebuggingState debugging{};

public:
    GlobalState();

public:
    void apply(StorageUpdate &update);
    void apply(UpcomingUpdate &update);
    void released(uint32_t locked) const;
    void released(uint32_t locked);
    bool flush(uint32_t timeout, Pool &pool);

public:
    GpsState const *location(Pool &pool) const;
};

} // namespace fk
