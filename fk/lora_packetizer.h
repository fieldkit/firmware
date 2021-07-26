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

public:
    tl::expected<EncodedMessage *, Error> packetize(GlobalState const *gs, Pool &pool);

};

} // namespace fk
