#pragma once

#include "modules/enable_module_power.h"
#include "modules/registry.h"
#include "modules/scanning.h"

namespace fk {

namespace state {

class AttachedSensor {
private:
    SensorMetadata const *meta_{ nullptr };
    uint32_t index_{ 0 };
    ModuleReading reading_;

public:
    AttachedSensor(SensorMetadata const *meta, uint32_t index);

public:
    uint32_t index();
    const char *name();
    ModuleReading reading();
    void reading(ModuleReading reading);
    const char *unit_of_measure() {
        return meta_->unitOfMeasure;
    }
    uint32_t flags() {
        return meta_->flags;
    }
};

class AttachedModule;

class ReadingsListener {
public:
    virtual int32_t readings_taken(AttachedModule *attached_module, ModuleReadings *readings, Pool *pool) = 0;
    virtual int32_t sensor_reading(AttachedModule *attached_module, AttachedSensor *sensor, ModuleReading reading,
                                   Pool *pool) = 0;
};

class NoopReadingsListener : public ReadingsListener {
public:
    int32_t readings_taken(AttachedModule *attached_module, ModuleReadings *readings, Pool *pool) override {
        return 0;
    }

    int32_t sensor_reading(AttachedModule *attached_module, AttachedSensor *sensor, ModuleReading reading,
                           Pool *pool) override {
        return 0;
    }
};

class AttachedModule {
private:
    using Sensors = collection<AttachedSensor>;
    ModulePosition position_;
    ModuleHeader header_;
    ModuleMetadata const *meta_{ nullptr };
    Module *driver_{ nullptr };
    Pool *pool_{ nullptr };
    ModuleStatus status_{ ModuleStatus::Found };
    ModuleConfiguration configuration_;
    Sensors sensors_{ pool_ };

public:
    AttachedModule(ModulePosition position, ModuleHeader const &header, ModuleMetadata const *meta, Module *driver,
                   Pool &pool);

public:
    ModulePosition position() const;
    ModuleHeader header() {
        return header_;
    }
    ModuleStatus status() const;
    const char *name() const;
    ModuleMetadata const *meta() const;
    ModuleConfiguration configuration() const;
    Sensors &sensors() {
        return sensors_;
    }
    fk_uuid_t id() const {
        return header_.id;
    }
    uint32_t flags() const {
        return meta_->flags;
    }
    Module *get() {
        return driver_;
    }
    bool is_virtual() const {
        return position() == ModulePosition::Virtual;
    }

public:
    int32_t initialize(ModuleContext ctx, Pool *pool);
    int32_t take_readings(ReadingsContext ctx, ReadingsListener *listener, Pool *pool);
    ModuleConfiguration get_configuration(Pool *pool);
    bool has_id(fk_uuid_t const &id) const;
    EnableModulePower enable();
};

class AttachedModules : public ScanningListener {
private:
    using Modules = collection<AttachedModule>;
    Modules modules_{ pool_ };
    Pool *pool_{ nullptr };

public:
    AttachedModules(Pool &pool);

public:
    int32_t scanned_module(ModulePosition const position, ModuleHeader const &header, Pool *pool) override;

public:
    ModuleStatus get_module_status(ModulePosition const position);
    AttachedModule *get_by_position(ModulePosition const position);
    AttachedModule *get_by_id(fk_uuid_t const &id);
    size_t number_of_sensors() const;
    size_t number_of_physical_sensors() const;

    struct ModuleAndSensor {
        AttachedModule *attached_module;
        AttachedSensor *sensor;
    };

    ModuleAndSensor get_nth_sensor(size_t n);
    Modules &modules() {
        return modules_;
    }

private:
    int32_t scan(Pool &pool);

public:
    int32_t create(Pool &pool);
    int32_t initialize(Pool &pool);
    int32_t take_readings(ReadingsListener *listener, Pool &pool);
};

} // namespace state

} // namespace fk
