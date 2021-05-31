#pragma once

#include <fk-data-protocol.h>

#include "common.h"
#include "encoded_message.h"
#include "containers.h"
#include "state.h"

namespace fk {

class LoraPacketizer {
private:
    // TODO Calculate this from spreading factor.
    size_t maximum_packet_size_{ 22 };

public:
    LoraPacketizer();
    virtual ~LoraPacketizer();

    struct TakenReadingsModule {
        ModulePosition position;
        ModuleReadings *readings;
    };

    using ModuleReadingsCollection = collection<TakenReadingsModule>;

    struct TakenReadings {
        uint32_t time;
        uint32_t number;
        ModuleReadingsCollection readings;

        TakenReadings() {
        }

        TakenReadings(uint32_t time, uint32_t number, ModuleReadingsCollection readings)
            : time(time), number(number), readings(std::move(readings)) {
        }
    };

public:
    tl::expected<EncodedMessage *, Error> packetize(TakenReadings const &taken, Pool &pool);

};

} // namespace fk
