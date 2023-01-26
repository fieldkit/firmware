#pragma once

#include <fk-data-protocol.h>

#include "common.h"
#include "containers.h"
#include "encoded_message.h"
#include "state.h"

namespace fk {

class LoraReadingsPacketizer {
public:
    tl::expected<EncodedMessage *, Error> packetize(GlobalState const *gs, Pool &pool);
};

class LoraLocationPacketizer {
public:
    tl::expected<EncodedMessage *, Error> packetize(GlobalState const *gs, Pool &pool);
};

} // namespace fk
