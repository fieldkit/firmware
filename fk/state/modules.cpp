#include "state/modules.h"
#include "modules/registry.h"
#include "modules/scanning.h"
#include "state.h"
#include "task_stack.h"

namespace fk {

FK_DECLARE_LOGGER("state");

namespace state {

AttachedSensor::AttachedSensor(SensorMetadata const *meta, uint32_t index) : meta_(meta), index_(index) {
}

AttachedSensor::AttachedSensor(SensorMetadata const *meta, uint32_t index, ModuleReading reading) : meta_(meta), index_(index), reading_(reading) {
}

uint32_t AttachedSensor::index() {
    return index_;
}

const char *AttachedSensor::name() {
    return meta_->name;
}

ModuleReading AttachedSensor::reading() {
    return reading_;
}

void AttachedSensor::reading(ModuleReading reading) {
    reading_ = reading;
}

AttachedModule::AttachedModule(ModulePosition position, ModuleHeader const &header, ModuleMetadata const *meta,
                               Module *driver, Pool &pool)
    : position_(position), header_(header), meta_(meta), driver_(driver), pool_(&pool) {
}

ModulePosition AttachedModule::position() const {
    return position_;
}

ModuleStatus AttachedModule::status() const {
    return status_;
}

const char *AttachedModule::name() const {
    if (configuration_.display_name_key != nullptr) {
        return configuration_.display_name_key;
    }
    if (meta_ != nullptr) {
        return meta_->name;
    }
    return "unknown";
}

ModuleMetadata const *AttachedModule::meta() const {
    return meta_;
}

ModuleConfiguration AttachedModule::configuration() const {
    return configuration_;
}

int32_t AttachedModule::initialize(ModuleContext ctx, Pool *pool) {
    auto mr = driver_->initialize(ctx, *pool_);
    if (mr.status != ModuleStatus::Ok) {
        logerror("[%d] error initializing module", position_.integer());
    }

    status_ = mr.status;

    configuration_ = driver_->get_configuration(*pool_);

    auto sensor_metas = driver_->get_sensors(*pool_);

    auto status_str = get_module_status_string(status_);

    loginfo("[%d] '%s' status=%s", position_.integer(), configuration_.display_name_key, status_str);

    if (sensor_metas != nullptr) {
        for (auto i = 0u; i < sensor_metas->nsensors; ++i) {
            auto &s = sensor_metas->sensors[i];

            loginfo("[%d] sensor[%2d] name='%s.%s'", position_.integer(), i, meta_->name, s.name);

            sensors_.emplace(&s, i);
        }
    } else {
        logwarn("[%d] no sensors", position_.integer());
    }

    return 0;
}

int32_t AttachedModule::take_readings(ReadingsContext ctx, ReadingsListener *listener, Pool *pool) {
    loginfo("[%d] '%s' mk=%02" PRIx32 "%02" PRIx32 " version=%" PRIu32, position_.integer(),
            configuration_.display_name_key, meta_->manufacturer, meta_->kind, meta_->version);

    configuration_ = driver_->get_configuration(*pool_);

    auto sensor_metas = driver_->get_sensors(*pool);
    if (sensor_metas == nullptr) {
        logwarn("[%d] sensorless module", position_.integer());
        return 0;
    }

    if (sensor_metas->nsensors != sensors_.size()) {
        logwarn("[%d] sensors change");
    }

    auto module_readings = driver_->take_readings(ctx, *pool);
    if (module_readings == nullptr) {
        status_ = ModuleStatus::Fatal;
        logwarn("no readings ms=fatal");
        return 0;
    }

    auto nreadings = module_readings->size();

    loginfo("[%d] %d readings", position_.integer(), nreadings);

    auto err = listener->readings_taken(this, module_readings, pool);
    if (err < 0) {
        logerror("readings listener");
        return err;
    }

    for (auto &sensor : sensors_) {
        auto i = sensor.index();
        if (i < nreadings) {
            auto reading = module_readings->get(i);

            loginfo("[%d] sensor[%2d] name='%s.%s' reading=%f (%f)", position_.integer(), sensor.index(), meta_->name,
                    sensor.name(), reading.calibrated, reading.uncalibrated);

            auto err = listener->sensor_reading(this, &sensor, reading, pool);
            if (err < 0) {
                logerror("reading listener");
                return err;
            }
        } else {
            logwarn("[%d] sensor[%2d] name='%s.%s' no-reading", position_.integer(), sensor.index(), meta_->name,
                    sensor.name());
        }
    }

    return 0;
}

bool AttachedModule::has_id(fk_uuid_t const &id) const {
    return memcmp(&header_.id, &id, sizeof(fk_uuid_t)) == 0;
}

ModuleConfiguration AttachedModule::get_configuration(Pool *pool) {
    configuration_ = driver_->get_configuration(*pool);

    return configuration_;
}

EnableModulePower AttachedModule::enable() {
    return EnableModulePower{ position_, configuration_.power, configuration_.wake_delay };
}

AttachedModules::AttachedModules(AttachedModules::Modules modules, Pool &pool) : modules_(modules), pool_(&pool) {
}

AttachedModules::AttachedModules(Pool &pool) : pool_(&pool) {
    modules_ = Modules{ pool_ };
}

int32_t AttachedModules::scanned_module(ModulePosition const position, ModuleHeader const &header, Pool *pool) {
    auto attached = get_by_id(header.id);
    if (attached == nullptr) {
        loginfo("[%d] initializing module", position.integer());

        auto gps = GpsState{};
        auto bus = get_board()->i2c_module();
        ScanningContext ctx{ get_modmux(), &gps, bus, *pool };

        ModuleRegistry registry;
        auto meta = registry.resolve(header);
        FK_ASSERT(meta != nullptr);

        auto driver = meta->ctor(*pool_);
        FK_ASSERT(driver != nullptr);

        modules_.emplace(position, header, meta, driver, *pool_);

        attached = get_by_position(position);
        FK_ASSERT(attached != nullptr);

        auto sub = ctx.open_module(position, *pool);
        if (!sub.open()) {
            logerror("[%d] choosing module", position.integer());
        } else {
            auto err = attached->initialize(sub, pool);
            if (err < 0) {
                return err;
            }
        }
    } else {
        loginfo("[%d] keeping module", position.integer());
    }

    return 0;
}

ModuleStatus AttachedModules::get_module_status(ModulePosition const position) {
    auto attached = get_by_position(position);
    if (attached != nullptr) {
        return attached->status();
    }
    return ModuleStatus::Empty;
}

AttachedModule *AttachedModules::get_by_position(ModulePosition const position) {
    for (auto &am : modules_) {
        if (am.position() == position) {
            return &am;
        }
    }
    return nullptr;
}

AttachedModule *AttachedModules::get_by_id(fk_uuid_t const &id) {
    for (auto &am : modules_) {
        if (am.has_id(id)) {
            return &am;
        }
    }
    return nullptr;
}

size_t AttachedModules::number_of_sensors() const {
    size_t total = 0;
    for (auto &am : modules_) {
        total += am.sensors().size();
    }
    return total;
}

size_t AttachedModules::number_of_physical_sensors() const {
    size_t total = 0;
    for (auto &am : modules_) {
        if (!am.is_virtual()) {
            total += am.sensors().size();
        }
    }
    return total;
}

AttachedModules::ModuleAndSensor AttachedModules::get_nth_sensor(size_t n) {
    for (auto &am : modules_) {
        for (auto &as : am.sensors()) {
            if (n-- == 0) {
                return { &am, &as };
            }
        }
    }
    return { nullptr, nullptr };
}

int32_t AttachedModules::scan(Pool &pool) {
    auto mm = get_modmux();

    mm->enable_all_modules();

    fk_delay(10); // TODO Maybe remove

    ModuleScanning scanning{ mm };
    auto err = scanning.scan(this, pool);
    if (err < 0) {
        logerror("scanning");
        return err;
    }

    if (!mm->choose_nothing()) {
        logerror("[-] deselecting");
    }

    return 0;
}

int32_t AttachedModules::create(Pool &pool) {
    modules_ = Modules{ pool_ };

    auto err = scan(pool);
    if (err < 0) {
        return err;
    }

    return 0;
}

int32_t AttachedModules::initialize(Pool &pool) {
    auto started = fk_uptime();

    loginfo("initialize begin");

    auto mm = get_modmux();
    auto gps = GpsState{};
    auto bus = get_board()->i2c_module();
    ScanningContext ctx{ mm, &gps, bus, pool };

    for (auto &attached : modules_) {
        auto position = attached.position();
        logged_task lt{ pool.sprintf("module[%d]", position.integer()) };

        auto module_power = attached.enable();
        if (!module_power.enable()) {
            logerror("[%d] error powering module", position.integer());
            return -1;
        }

        auto sub = ctx.open_module(position, pool);
        if (!sub.open()) {
            logerror("[%d] choosing module", position.integer());
        } else {
            auto err = attached.initialize(sub, &pool);
            if (err < 0) {
                return err;
            }
        }
    }

    if (!mm->choose_nothing()) {
        logerror("[-] deselecting");
    }

    auto elapsed = fk_uptime() - started;

    loginfo("initialize elapsed=%" PRIu32 "ms", elapsed);

    return 0;
}

int32_t AttachedModules::take_readings(ReadingsListener *listener, Pool &pool) {
    auto started = fk_uptime();

    loginfo("take-readings begin");

    auto mm = get_modmux();
    auto gps = GpsState{};
    auto bus = get_board()->i2c_module();
    ScanningContext ctx{ mm, &gps, bus, pool };

    for (auto &attached : modules_) {
        auto position = attached.position();

        logged_task lt{ pool.sprintf("module[%d]", position.integer()) };

        auto sub = ctx.open_readings(position, pool);
        if (!sub.open()) {
            logerror("[%d] choosing module", position.integer());
        } else {
            auto err = attached.take_readings(sub, listener, &pool);
            if (err < 0) {
                return err;
            }
        }
    }

    if (!mm->choose_nothing()) {
        logerror("[-] deselecting");
    }

    auto elapsed = fk_uptime() - started;

    loginfo("take-readings elapsed=%" PRIu32 "ms", elapsed);

    return 0;
}

} // namespace state

bool fk_state_test() {
    StandardPool pool{ "state-test" };

    auto mm = get_modmux();

    mm->enable_all_modules();

    state::DynamicState dynamic;

    if (dynamic.attached()->create(pool) < 0) {
        logerror("scanning");
    }

    auto counter = 0;

    while (true) {
        if (pool.used() > 0) {
            pool.clear();
        }

        state::NoopReadingsListener readings_listener;
        if (dynamic.attached()->take_readings(&readings_listener, pool) < 0) {
            logerror("scanning");
        }

        fk_delay(2000);

        if (++counter % 5 == 0) {
            loginfo("recreating!");

            state::DynamicState replacing;

            if (replacing.attached()->create(pool) < 0) {
                logerror("scanning");
            }

            dynamic = std::move(replacing);
        }
    }

    return true;
}

} // namespace fk
