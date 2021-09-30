#pragma once

#include "common.h"
#include "display_views.h"
#include "scheduling.h"
#include "state_ref.h"

namespace fk {

class ScheduleView : public DisplayView {
private:
    ScheduleType type_{ ScheduleType::Readings };
    ScheduledTime scheduled_;
    int32_t interval_{ 0 };
    uint8_t position_{ 0 };
    uint32_t update_at_{ 0 };

public:
    void tick(ViewController *views, Pool &pool) override;
    void up(ViewController *views) override;
    void down(ViewController *views) override;
    void enter(ViewController *views) override;
    void type(ScheduleType type);

private:
    Schedule get();
    void set(uint32_t interval);
};

} // namespace fk
