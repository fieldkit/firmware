#include "readings.h"
#include "clock.h"
#include "records.h"
#include "platform.h"
#include "modules/bridge/modules.h"

namespace fk {

FK_DECLARE_LOGGER("readings");

class EnableModulePower {
private:
    bool enabled_;
    ModulePower power_;
    ModulePosition position_;

public:
    bool enabled_once() {
        return enabled_ && position_ != ModulePosition::Virtual && power_ == ModulePower::ReadingsOnly;
    }

    bool always_enabled() {
        return power_ == ModulePower::Always;
    }

    EnableModulePower(bool enabled, ModulePower power, ModulePosition position) : enabled_(enabled), power_(power), position_(position) {
    }

    virtual ~EnableModulePower() {
        if (enabled_once()) {
            logverbose("[%d] powering off", position_.integer());
            if (!get_modmux()->disable_module(position_)) {
                loginfo("[%d] disabling module failed", position_.integer());
            }
        }
    }

public:
    bool enable() {
        if (enabled_once() || always_enabled()) {
            logverbose("[%d] powering on", position_.integer());
            if (!get_modmux()->enable_module(position_)) {
                loginfo("[%d] enabling module failed", position_.integer());
                return false;
            }
        }
        return true;
    }

};

Readings::Readings(ModMux *mm) : mm_(mm) {
}

void Readings::link(uint32_t meta_record, uint32_t reading_number) {
    record_.readings.reading = reading_number;
    record_.readings.meta = meta_record;
}

tl::expected<ModuleReadingsCollection, Error> Readings::take_readings(ScanningContext &ctx, ConstructedModulesCollection const &modules, Pool &pool) {
    ModuleReadingsCollection all_readings{ pool };

    auto now = get_clock_now();
    auto gps = ctx.gps();

    record_ = fk_data_record_encoding_new();
    record_.has_readings = true;
    record_.readings.time = now;
    // These get set via Readings::link.
    record_.readings.reading = 0;
    record_.readings.meta = 0;
    record_.readings.uptime = fk_uptime();
    record_.readings.flags = fk_data_DownloadFlags_READING_FLAGS_NONE;
    record_.readings.has_location = true;
    record_.readings.location.time = gps->time;
    record_.readings.location.fix = gps->fix;
    record_.readings.location.longitude = gps->longitude;
    record_.readings.location.latitude = gps->latitude;
    record_.readings.location.altitude = gps->altitude;
    record_.readings.location.satellites = gps->satellites;
    record_.readings.location.hdop = gps->hdop;

    if (modules.size() == 0) {
        return std::move(all_readings);
    }

    auto groups = pool.malloc<fk_data_SensorGroup>(modules.size());
    auto group_number = 0;

    bzero(groups, sizeof(fk_data_SensorGroup) * modules.size());

    for (auto pair : modules) {
        auto meta = pair.meta;
        auto module = pair.module;
        auto i = pair.found.position;

        if (module == nullptr) {
            continue;
        }

        EnableModulePower module_power{ true, pair.configuration.power, pair.found.position };
        if (!module_power.enable()) {
            logerror("[%d] error powering module", i.integer());
            continue;
        }
        if (module_power.enabled_once()) {
            fk_delay(pair.configuration.wake_delay);
        }

        auto mc = ctx.readings(i, all_readings, pool);
        if (!mc.open()) {
            logerror("[%d] error choosing module", i.integer());
            continue;
        }

        loginfo("'%s' mk=%02" PRIx32 "%02" PRIx32 " version=%" PRIu32, pair.configuration.display_name_key, meta->manufacturer, meta->kind, meta->version);

        auto module_status = module->status(mc, pool);
        if (module_status.status != ModuleStatus::Ok) {
            logwarn("'%s' status error", meta->name);
        } else {
            if (module_status.message != nullptr) {
                loginfo("'%s' status ok (%zu bytes)", meta->name, module_status.message->size);
            }
            else {
                loginfo("'%s' status ok", meta->name);
            }
        }

        auto readings = module->take_readings(mc, pool);
        if (readings == nullptr || readings->size() == 0) {
            logwarn("'%s' no readings", meta->name);
            continue;
        }

        loginfo("'%s' %zd readings", meta->name, readings->size());

        auto sensor_metas = module->get_sensors(pool);
        auto sensor_values = pool.malloc<fk_data_SensorAndValue>(readings->size());
        for (auto i = 0u; i < readings->size(); ++i) {
            auto value = readings->get(i);
            loginfo("[%2d] '%s.%s' = %f", i, meta->name, sensor_metas->sensors[i].name, value);
            sensor_values[i] = { i, value };
        }

        auto readings_array = pool.malloc_with<pb_array_t>({
            .length = (size_t)readings->size(),
            .itemSize = sizeof(fk_data_SensorAndValue),
            .buffer = sensor_values,
            .fields = fk_data_SensorAndValue_fields,
        });

        auto &group = groups[group_number];
        group.module = i.integer();
        group.readings.funcs.encode = pb_encode_array;
        group.readings.arg = readings_array;
        group_number++;

        all_readings.emplace(ModuleMetaAndReadings{
            .position = pair.found.position,
            .id = (fk_uuid_t *)pool.copy(&pair.found.header.id, sizeof(pair.found.header.id)),
            .meta = meta,
            .status_message = module_status.message,
            .sensors = module->get_sensors(pool),
            .readings = readings,
            .configuration = pair.configuration,
        });
    }

    auto sensor_groups_array = pool.malloc_with<pb_array_t>({
        .length = (size_t)modules.size(),
        .itemSize = sizeof(fk_data_SensorGroup),
        .buffer = groups,
        .fields = fk_data_SensorGroup_fields,
    });

    record_.readings.sensorGroups.arg = sensor_groups_array;

    if (!mm_->choose_nothing()) {
        logerror("[-] error deselecting");
        return std::move(all_readings);
    }

    return std::move(all_readings);
}

fk_data_DataRecord const &Readings::record() {
    return record_;
}

}
