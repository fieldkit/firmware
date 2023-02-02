#pragma once

#include "worker.h"
#include "lora_manager.h"
#include "state_ref.h"

namespace fk {

enum class LoraWorkOperation {
    FactoryReset,
    Configure,
    Readings,
    Location,
    Status,
};

struct LoraWork {
    LoraWorkOperation op;
};

class LoraWorker : public Worker {
private:
    LoraWork work_;

public:
    explicit LoraWorker(LoraWork work);

public:
    void run(Pool &pool) override;

    uint8_t priority() const override {
        return FK_PRIORITY_NORMAL;
    }

    const char *name() const override {
        return "lora";
    }

private:
    bool factory_reset(LoraManager &lora, Pool &pool);
    bool configure(LoraManager &lora, Pool &pool);
    bool readings(LoraManager &lora, Pool &pool);
    bool location(LoraManager &lora, Pool &pool);
    bool status(LoraManager &lora, Pool &pool);

private:
    struct OutgoingPackets {
        EncodedMessage *packets;
    };

    template <typename PacketizerType> OutgoingPackets packetize(Pool &pool) {
        auto gs = get_global_state_ro();
        PacketizerType packetizer;
        auto packets = packetizer.packetize(gs.get(), pool);
        if (!packets) {
            return OutgoingPackets{ nullptr };
        }
        return OutgoingPackets{ *packets };
    }

    template <typename PacketizerType> bool packets(LoraManager &lora, uint8_t port, Pool &pool) {
        auto outgoing = packetize<PacketizerType>(pool);
        auto iterator = outgoing.packets;
        if (iterator == nullptr) {
            return true;
        }

        return packets(lora, port, iterator, pool);
    }

    bool packets(LoraManager &lora, uint8_t port, EncodedMessage *iterator, Pool &pool);
};

FK_ENABLE_TYPE_NAME(LoraWorker);

} // namespace fk
