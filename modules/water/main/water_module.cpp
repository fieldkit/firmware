#include "water_module.h"
#include "platform.h"
#include "atlas_protocol.h"

using namespace fk;

FK_DECLARE_LOGGER("water");

class AtlasApiReply {
private:
    Pool *pool_;
    fk_atlas_WireAtlasReply reply_;

public:
    AtlasApiReply(Pool &pool);

public:
    fk_atlas_WireAtlasReply const *reply() {
        return fk_atlas_reply_prepare_encoding(&reply_, pool_);
    }

};

AtlasApiReply::AtlasApiReply(Pool &pool) : pool_(&pool) {
    reply_ = fk_atlas_WireAtlasReply_init_default;
}

bool WaterModule::initialize(ModuleContext mc, Pool &pool) {
    auto atlas = OemAtlas{ mc.module_bus() };
    if (!atlas.find()) {
        return false;
    }

    type_ = atlas.type();
    address_ = atlas.address();

    return true;
}

bool WaterModule::api(HttpServerConnection *connection, Pool &pool) {
    Reader *reader = connection;

    auto query = fk_atlas_query_prepare_decoding(pool.malloc<fk_atlas_WireAtlasQuery>(), &pool);
    auto stream = pb_istream_from_readable(reader);
    if (!pb_decode_delimited(&stream, fk_app_HttpQuery_fields, query)) {
        logwarn("error parsing query (%" PRIu32 ")", connection->length());
        connection->error("error parsing query");
        return true;
    }

    AtlasApiReply reply{ pool };

    connection->write(200, "ok", reply.reply(), fk_atlas_WireAtlasReply_fields);

    connection->close();

    return true;
}

bool WaterModule::service(ModuleContext mc, Pool &pool) {
    return true;
}

ModuleSensors const *WaterModule::get_sensors(Pool &pool) {
    switch (type_) {
    case AtlasSensorType::Ec: {
        auto meta = pool.malloc_with<SensorMetadata, 3>({
            {
                .name = "ec",
                .unitOfMeasure = "µS/cm",
                .flags = 0,
            },
            {
                .name = "tds",
                .unitOfMeasure = "ppm",
                .flags = 0,
            },
            {
                .name = "salinity",
                .unitOfMeasure = "",
                .flags = 0,
            }
        });
        return pool.malloc_with<ModuleSensors>({
            .nsensors = 3,
            .sensors = meta,
        });
    }
    case AtlasSensorType::Ph: {
        auto meta = pool.malloc_with<SensorMetadata>({
            .name = "ph",
            .unitOfMeasure = "",
            .flags = 0,
        });
        return pool.malloc_with<ModuleSensors>({
            .nsensors = 1,
            .sensors = meta,
        });
    }
    case AtlasSensorType::Do: {
        auto meta = pool.malloc_with<SensorMetadata>({
            .name = "do",
            .unitOfMeasure = "mg/L",
            .flags = 0,
        });
        return pool.malloc_with<ModuleSensors>({
            .nsensors = 1,
            .sensors = meta,
        });
    }
    case AtlasSensorType::Temp: {
        auto meta = pool.malloc_with<SensorMetadata>({
            .name = "temp",
            .unitOfMeasure = "C",
            .flags = 0,
        });
        return pool.malloc_with<ModuleSensors>({
            .nsensors = 1,
            .sensors = meta,
        });
    }
    case AtlasSensorType::Orp: {
        auto meta = pool.malloc_with<SensorMetadata>({
            .name = "orp",
            .unitOfMeasure = "mV",
            .flags = 0,
        });
        return pool.malloc_with<ModuleSensors>({
            .nsensors = 1,
            .sensors = meta,
        });
    }
    default: {
        break;
    }
    }

    logerror("unknown atlas module!");

    return nullptr;
}

ModuleConfiguration WaterModule::get_configuration(Pool &pool) {
    return { };
}

ModuleReadings *WaterModule::take_readings(ModuleContext mc, Pool &pool) {
    if (type_ == AtlasSensorType::Unknown) {
        return nullptr;
    }

    auto atlas = OemAtlas{ mc.module_bus(), address_, type_ };
    if (!atlas.wake()) {
        logerror("wake failed");
        return nullptr;
    }

    size_t number_of_values = 0;
    float values[ATLAS_MAXIMUM_VALUES];
    if (!atlas.read(values, number_of_values)) {
        atlas.leds(true);
        atlas.hibernate();
        logerror("readings failed");
        return nullptr;
    }

    if (!atlas.leds(true)) {
        logerror("leds failed");
        return nullptr;
    }

    if (!atlas.hibernate()) {
        logerror("hibernate failed");
        return nullptr;
    }

    auto mr = new(pool) NModuleReadings<ATLAS_MAXIMUM_VALUES>(number_of_values);
    for (size_t i = 0; i < mr->size(); ++i) {
        mr->set(i, values[i]);
    }

    return mr;
}
