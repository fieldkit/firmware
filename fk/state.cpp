#include "state.h"
#include "state_ref.h"
#include "storage/meta_ops.h"

#if defined(__SAMD51__)
#include "hal/metal/metal_ipc.h"
#else
#include "hal/linux/linux_ipc.h"
#endif

namespace fk {

FK_DECLARE_LOGGER("gs");

static GlobalState gs;

void Schedule::recreate() {
    auto has_intervals = false;
    auto cs = lwcron::CronSpec::interval(0);

    for (auto i = 0u; i < MaximumScheduleIntervals; ++i) {
        auto &ival = intervals[i];
        if (ival.interval > 0) {
            for (auto s = ival.start; s <= ival.end; s += ival.interval) {
                lwcron::TimeOfDay tod{ s };
                cs.set(tod);
            }
            has_intervals = true;
        }
    }

    if (!has_intervals && interval > 0) {
        cs = lwcron::CronSpec::interval(std::max(interval, OneMinuteSeconds));
    }

    cron = cs;
}

void Schedule::simple(uint32_t interval) {
    repeated = 0;
    duration = 0;
    jitter = 0;
    interval = interval;
    cron = lwcron::CronSpec::interval(std::max(interval, OneMinuteSeconds));
}

Schedule& Schedule::operator=(const fk_app_Schedule &s) {
    memzero(intervals, sizeof(intervals));
    if (s.intervals.arg != nullptr) {
        auto intervals_array = reinterpret_cast<pb_array_t*>(s.intervals.arg);
        auto intervals_source = reinterpret_cast<fk_app_Interval*>(intervals_array->buffer);
        for (auto i = 0u; i < std::min(intervals_array->length, MaximumScheduleIntervals); ++i) {
            if (intervals_source[i].interval > 0 && intervals_source[i].start != intervals_source[i].end) {
                intervals[i].start = intervals_source[i].start;
                intervals[i].end = intervals_source[i].end;
                intervals[i].interval = intervals_source[i].interval;
            }
        }
    }

    interval = s.interval;
    repeated = s.repeated;
    duration = s.duration;
    jitter = s.jitter;

    recreate();

    return *this;
}

GlobalStateRef<const GlobalState*> get_global_state_ro() {
    auto lock = data_lock.acquire_read(UINT32_MAX);
    FK_ASSERT(lock);
    return { std::move(lock), true, &gs };
}

GlobalStateRef<GlobalState*> get_global_state_rw() {
    auto lock = data_lock.acquire_write(UINT32_MAX);
    FK_ASSERT(lock);
    return { std::move(lock), false, &gs };
}

GlobalStateRef<GlobalState const*> try_get_global_state_ro() {
    auto lock = data_lock.acquire_read(0);
    if (!lock) {
        return { std::move(lock), false, nullptr };
    }
    return { std::move(lock), true, &gs };
}

GlobalState::GlobalState() : version(0) {
}

void GlobalState::update_data_stream(File const &file) {
    storage.data.size = file.size();
    storage.data.block = file.end_record();
    readings.number = file.end_record();
}

void GlobalState::update_meta_stream(File const &file) {
    storage.meta.size = file.size();
    storage.meta.block = file.end_record();
}

void GlobalState::update_physical_modules(ConstructedModulesCollection const &modules) {
    for (auto &status : physical_modules) {
        status.meta = nullptr;
        status.header = { };
        status.status = ModuleStatus::Empty;
    }

    for (auto &m : modules) {
        if (m.found.physical()) {
            auto index = m.found.position.integer();
            FK_ASSERT(index < MaximumNumberOfPhysicalModules);
            auto &status = physical_modules[index];
            status.header = m.found.header;
            status.meta = m.meta;
            status.status = m.status;
        }
    }
}

void GlobalState::released(uint32_t locked) const {
    auto elapsed = fk_uptime() - locked;
    if (elapsed > 100) {
        logwarn("read (%" PRIu32 "ms)", elapsed);
    }
}

void GlobalState::released(uint32_t locked) {
    logverbose("modified (%" PRIu32 "ms)", fk_uptime() - locked);
    version++;
}

bool GlobalState::flush(Pool &pool) {
    Storage storage{ MemoryFactory::get_data_memory(), pool, false };
    if (!storage.begin()) {
        return false;
    }

    MetaOps ops{ storage };
    if (!ops.write_state(this, pool)) {
        return false;
    }

    return true;
}

GpsState *GpsState::clone(Pool &pool) const {
    auto clone = new (pool) GpsState();
    memcpy(clone, this, sizeof(GpsState));
    return clone;
}

GpsState const *GlobalState::location(Pool &pool) const {
    return gps.clone(pool);
}

}
