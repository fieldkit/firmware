#include "export_data_worker.h"

#include "hal/hal.h"
#include "hal/memory.h"
#include "hal/sd_card.h"

#include "clock.h"
#include "records.h"

#include "storage/signed_log.h"

namespace fk {

FK_DECLARE_LOGGER("expdata");

ExportDataWorker::ExportDataWorker() : ExportDataWorker(MemoryFactory::get_data_memory()) {
}

ExportDataWorker::ExportDataWorker(DataMemory *data_memory) : data_memory_(data_memory), info_{ "CSV", 0.0f, true } {
}

void ExportDataWorker::run(Pool &pool) {
    auto lock = storage_mutex.acquire(UINT32_MAX);

    StatisticsMemory memory{ data_memory_ };
    Storage storage{ &memory, pool };
    if (!storage.begin()) {
        logerror("error opening storage");
        return;
    }

    auto sd = get_sd_card();
    if (!sd->begin()) {
        logerror("error opening sd card");
        return;
    }

    FormattedTime formatted{ get_clock_now(), TimeFormatMachine };
    loginfo("mkdir %s", formatted.cstr());
    if (!sd->mkdir(formatted.cstr())) {
        logerror("error making directory '%s'", formatted.cstr());
        return;
    }

    auto meta_file = storage.file_reader(Storage::Meta, pool);

    auto data_file = storage.file_reader(Storage::Data, pool);

    loginfo("calculating file size");

    size_t total_bytes = 0u;
    if (data_file->get_file_size(total_bytes) < 0) {
        return;
    }

    if (total_bytes == 0) {
        logerror("no data");
        return;
    }

    loginfo("seeking beginnings");

    StandardPool loop_pool{ "decode-loop" };

    // TODO Make this seek the first data record.
    auto seek_meta_err = meta_file->seek_record(0, loop_pool);
    if (!seek_meta_err) {
        return;
    }

    // Why is this suddenly necessary?
    // TODO Make this seek the first data record.
    auto seek_data_err = data_file->seek_record(0, loop_pool);
    if (!seek_data_err) {
        return;
    }

    loginfo("exporting");

    auto bytes_read = 0u;
    auto nrecords = 0u;
    while (bytes_read < total_bytes) {
        auto record = loop_pool.malloc<fk_data_DataRecord>();
        fk_data_record_decoding_new(record, loop_pool);

        auto record_read = data_file->read(record, fk_data_DataRecord_fields);
        FK_ASSERT(record_read >= 0);
        if (record_read < 0) {
            logerror("error");
            break;
        }
        if (record_read == 0) {
            loginfo("done");
            break;
        }

        if (!record->has_readings) {
            loginfo("skip meta record");
            continue;
        }

        if (!lookup_meta(record->readings.meta, meta_file, loop_pool)) {
            logerror("error looking up meta (%" PRIu64 ")", record->readings.meta);
            break;
        }

        if (writing_ == nullptr) {
            auto path = pool.sprintf("/%s/d_%06" PRIu32 ".csv", formatted.cstr(), meta_record_number_);
            writing_ = sd->open(path, OpenFlags::Append, pool);
            if (writing_ == nullptr || !writing_->is_open()) {
                logerror("unable to open '%s'", path);
                return;
            }

            if (!write_header()) {
                return;
            }
        }

        info_.progress = (float)bytes_read / total_bytes;
        bytes_read += record_read;
        nrecords++;

        switch (write_row(*record)) {
        case Success: {
            break;
        }
        case Debug: {
            break;
        }
        case Fatal: {
            logerror("error writing row");
            return;
        }
        }

        loop_pool.clear();
    }

    if (writing_ != nullptr) {
        if (!writing_->close()) {
            logerror("error closing");
            return;
        }
    }
}

bool ExportDataWorker::lookup_meta(uint32_t meta_record_number, FileReader *meta_file, Pool &pool) {
    if (meta_record_number_ == meta_record_number) {
        return true;
    }

    loginfo("reading meta %" PRIu32, meta_record_number);

    if (!meta_file->seek_record(meta_record_number, pool)) {
        logerror("error seeking meta record");
        return false;
    }

    meta_pool_.clear();

    meta_record_ = MetaRecord{ meta_pool_ };

    if (!meta_file->decode_signed(meta_record_.for_decoding(), fk_data_DataRecord_fields, meta_pool_)) {
        logerror("error reading meta record");
        return false;
    }

    meta_record_number_ = meta_record_number;

    if (writing_ != nullptr) {
        writing_->close();
        writing_ = nullptr;
    }

    return true;
}

bool ExportDataWorker::write_header() {
    auto modules_array = reinterpret_cast<pb_array_t *>(meta_record_.record()->modules.arg);
    auto modules = reinterpret_cast<fk_data_ModuleInfo *>(modules_array->buffer);

    StackBufferedWriter<StackBufferSize> writer{ writing_ };

    writer.write("time,data_record,meta_record,uptime,gps,latitude,longitude,altitude,gps_time,note");

    for (auto i = 0u; i < modules_array->length; ++i) {
        auto &module = modules[i];
        auto sensors_array = reinterpret_cast<pb_array_t *>(module.sensors.arg);
        auto sensors = reinterpret_cast<fk_data_SensorInfo *>(sensors_array->buffer);

        writer.write(",module_index,module_position,module_name");

        for (auto j = 0u; j < sensors_array->length; ++j) {
            writer.write(",%s", (const char *)sensors[j].name.arg);
        }
    }

    writer.write('\n');
    writer.flush();

    return true;
}

ExportDataWorker::WriteStatus ExportDataWorker::write_row(fk_data_DataRecord &record) {
    auto modules_array = reinterpret_cast<pb_array_t *>(meta_record_.record()->modules.arg);
    auto sensor_groups_array = reinterpret_cast<pb_array_t *>(record.readings.sensorGroups.arg);

    auto modules = reinterpret_cast<fk_data_ModuleInfo *>(modules_array->buffer);
    auto sensor_groups = reinterpret_cast<fk_data_SensorGroup *>(sensor_groups_array->buffer);

    StackBufferedWriter<StackBufferSize> writer{ writing_ };

    writer.write("%" PRIu64 ",%" PRIu32 ",%" PRIu32 ",%" PRIu32 ",%" PRIu32 ",%f,%f,%f,%" PRIu32 ",", record.readings.time,
                 record.readings.reading, record.readings.meta, record.readings.uptime, record.readings.location.fix,
                 record.readings.location.latitude, record.readings.location.longitude, record.readings.location.altitude,
                 record.readings.location.time);

    if (modules_array->length != sensor_groups_array->length) {
        writer.write("modules-mismatch\n");
        return WriteStatus::Debug;
    }

    for (auto i = 0u; i < sensor_groups_array->length; ++i) {
        auto &sensor_group = sensor_groups[i];
        auto sensor_values_array = reinterpret_cast<pb_array_t *>(sensor_group.readings.arg);
        auto sensor_values = reinterpret_cast<fk_data_SensorAndValue *>(sensor_values_array->buffer);

        auto &module = modules[i];

        writer.write(",%d,%d,%s", i, sensor_group.module, (const char *)module.name.arg);

        for (auto j = 0u; j < sensor_values_array->length; ++j) {
            writer.write(",%f", sensor_values[j].value);
        }
    }

    writer.write('\n');
    writer.flush();

    return WriteStatus::Success;
}

} // namespace fk
