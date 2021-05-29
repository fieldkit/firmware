#pragma once

#include "modules/scanning.h"
#include "modules/registry.h"

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
    AttachedModule(ModulePosition position, ModuleHeader const &header, ModuleMetadata const *meta, Module *driver, Pool &pool);

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
    fk_uuid_t id() {
        return header_.id;
    }
    uint32_t flags() {
        return meta_->flags;
    }
    Module *get() {
        return driver_;
    }

public:
    int32_t initialize(ModuleContext ctx, Pool *pool);
    int32_t take_readings(ReadingsContext ctx, Pool *pool);
    ModuleConfiguration get_configuration(Pool *pool);
    bool has_id(fk_uuid_t const &id) const;

};

class AttachedModules : public ScanningListener {
private:
    using Modules = collection<AttachedModule>;
    Modules modules_{ pool_ };
    Pool *pool_{ nullptr };
    bool refresh_necessary_{ true };
    bool creating_{ true };

public:
    AttachedModules(Pool &pool);

public:
    int32_t scanned_module(ModulePosition const position, ModuleHeader const &header, Pool *pool) override;

public:
    ModuleStatus get_module_status(ModulePosition const position);
    AttachedModule *get_by_position(ModulePosition const position);
    AttachedModule *get_by_id(fk_uuid_t const &id);
    size_t number_of_sensors();

    struct ModuleAndSensor{
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
    int32_t take_readings(Pool &pool);

};

}

}
