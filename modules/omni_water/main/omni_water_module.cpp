#include "omni_water_module.h"
#include "omni_water_api.h"

#include "modules/eeprom.h"
#include "platform.h"
#include "state_ref.h"
#include "mpl3115a2.h"

namespace fk {

FK_DECLARE_LOGGER("omniwater");

#define FK_MCP2803_ADDRESS 0x22
#define FK_ADS1219_ADDRESS 0x45

#define FK_MCP2803_IODIR 0b00000010
#define FK_MCP2803_GPPU  0b00000010

#define FK_MCP2803_GPIO_ON  0b00000001
#define FK_MCP2803_GPIO_OFF 0b00000000

#define FK_MCP2803_GPIO_EXCITE_ON  0b00000101
#define FK_MCP2803_GPIO_EXCITE_OFF 0b00000001

OmniWaterModule::OmniWaterModule(Pool &pool) : pool_(pool.subpool("omniwater", MaximumConfigurationSize)) {
}

OmniWaterModule::~OmniWaterModule() {
}

ModuleReturn OmniWaterModule::initialize(ModuleContext mc, Pool &pool) {
    loginfo("initialize");

    /*
    if (!load_configuration(mc, pool)) {
        return { ModuleStatus::Fatal };
    }

    auto &bus = mc.module_bus();

    Mcp2803 mcp{ bus, FK_MCP2803_ADDRESS };
    NoopReadyChecker unused_ready_checker;
    Ads1219 ads{ bus, FK_ADS1219_ADDRESS, &unused_ready_checker };

    if (!initialize(mcp, ads)) {
        return { ModuleStatus::Fatal };
    }
    */

    return { ModuleStatus::Ok };
}

bool OmniWaterModule::initialize(Mcp2803 &mcp, Ads1219 &ads) {
    /*
    if (!mcp.configure(FK_MCP2803_IODIR, FK_MCP2803_GPPU, FK_MCP2803_GPIO_OFF)) {
        logerror("mcp2803::begin");
        return false;
    }

    if (!mcp.configure(FK_MCP2803_IODIR, FK_MCP2803_GPPU, FK_MCP2803_GPIO_ON)) {
        logerror("mcp2803::begin");
        return false;
    }

    fk_delay(100);

    if (!ads.begin()) {
        logerror("ads1219::begin");
        return false;
    }

    if (!ads.configure(Ads1219VoltageReference::Internal, Ads1219Channel::Diff_0_1, Ads1219Gain::One, Ads1219DataRate::DataRate_1000)) {
        logerror("ads1219::configure");
        return false;
    }
    */

    return true;
}

bool OmniWaterModule::load_configuration(ModuleContext mc, Pool &pool) {
    ModuleEeprom eeprom{ mc.module_bus() };

    logdebug("reading header");

    // We need the header to know the kind of module we are so if that
    // fails then we're in pretty bad shape.
    bzero(&header_, sizeof(ModuleHeader));
    if (!eeprom.read_header(header_)) {
        logwarn("error reading header");
        return false;
    }

    if (!fk_module_header_valid(&header_)) {
        logwarn("invalid header");
        return false;
    }

    loginfo("have header: mk=%02" PRIx32 "%02" PRIx32, header_.manufacturer, header_.kind);

    /*
    cfg_message_ = nullptr;
    cfg_ = nullptr;
    pool_->clear();

    size_t size = 0;
    auto buffer = (uint8_t *)pool.malloc(MaximumConfigurationSize);
    bzero(buffer, MaximumConfigurationSize);
    if (!eeprom.read_configuration(buffer, size, MaximumConfigurationSize)) {
        logwarn("mod-cfg: reading");
    } else if (size > 0) {
        auto cfg = fk_module_configuration_decoding_new(pool_);
        auto stream = pb_istream_from_buffer(buffer, size);
        if (!pb_decode_delimited(&stream, fk_data_ModuleConfiguration_fields, cfg)) {
            // Some modules consider this an error. We continue along uncalibrated.
            logwarn("mod-cfg: decoding");
        } else {
            loginfo("mod-cfg: decoded");
            cfg_message_ = pool_->wrap_copy(buffer, size);
            cfg_ = cfg;
        }
    }
    */

    return true;
}

ModuleReturn OmniWaterModule::api(ModuleContext mc, HttpServerConnection *connection, Pool &pool) {
    OmniWaterApi api;
    if (!api.handle(mc, connection, pool)) {
        logerror("api (ms::fatal)");
        return { ModuleStatus::Fatal };
    }

    load_configuration(mc, pool);

    return { ModuleStatus::Ok };
}

ModuleReturn OmniWaterModule::service(ModuleContext mc, Pool &pool) {
    return { ModuleStatus::Ok };
}

ModuleSensors const *OmniWaterModule::get_sensors(Pool &pool) {
    SensorMetadata *sensors = nullptr;

    /*
    switch (header_.kind) {
    case FK_MODULES_KIND_WATER_PH:
        sensors = pool.malloc_with<SensorMetadata>({
            .name = "ph",
            .unitOfMeasure = "pH",
            .flags = 0,
        });
        break;
    case FK_MODULES_KIND_WATER_EC:
        sensors = pool.malloc_with<SensorMetadata>({
            .name = "ec",
            .unitOfMeasure = "µS/cm",
            .flags = 0,
        });
        break;
    case FK_MODULES_KIND_WATER_DO:
        return &fk_module_water_do_sensors;
    case FK_MODULES_KIND_WATER_TEMP:
        sensors = pool.malloc_with<SensorMetadata>({
            .name = "temp",
            .unitOfMeasure = "°C",
            .flags = 0,
        });
        break;
    case FK_MODULES_KIND_WATER_ORP:
        sensors = pool.malloc_with<SensorMetadata>({
            .name = "orp",
            .unitOfMeasure = "mV",
            .flags = 0,
        });
        break;
    default:
        logwarn("unknown water module kind: %d", header_.kind);
        return nullptr;
    };
    */

    return pool.malloc_with<ModuleSensors>({
        .nsensors = 0,
        .sensors = sensors,
    });
}

ModuleConfiguration const OmniWaterModule::get_configuration(Pool &pool) {
    return ModuleConfiguration{ "modules.water.omni", ModulePower::ReadingsOnly, cfg_message_, DefaultModuleOrder };
}

bool OmniWaterModule::can_enable() {
    return true;
}

ModuleReadings *OmniWaterModule::take_readings(ReadingsContext mc, Pool &pool) {
    return nullptr;
}

} // namespace fk
