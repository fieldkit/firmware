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

ExportDataWorker::ExportDataWorker(DataMemory *data_memory) : data_memory_(data_memory) {
    info_ = { "CSV", 0.0f, true };
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
    if (!sd->mkdir(formatted.cstr())) {
        logerror("error making directory '%s'", formatted.cstr());
        return;
    }

    auto meta_file = storage.file(Storage::Meta);
    auto reading = storage.file(Storage::Data);
    if (!reading.seek_end()) {
        logerror("seek end failed");
        return;
    }

    auto total_bytes = reading.size();
    if (total_bytes == 0) {
        loginfo("no data");
        return;
    }

    if (!reading.seek_beginning()) {
        logerror("seek beginning failed");
        return;
    }

    StandardPool loop_pool{ "decode" };

    auto bytes_read = 0u;
    auto nrecords = 0u;
    while (bytes_read < total_bytes) {
        auto record = fk_data_record_decoding_new(loop_pool);
        auto record_read = reading.read(&record, fk_data_DataRecord_fields);
        if (record_read == 0) {
            loginfo("done");
            break;
        }

        if (!lookup_meta(record.readings.meta, meta_file, loop_pool)) {
            logerror("error looking up meta (%" PRIu32 ")", record.readings.meta);
            continue;
        }

        if (writing_ == nullptr) {
            auto path = pool.sprintf("/%s/d_%06" PRIu32 ".csv", formatted.cstr(), meta_record_number_);
            writing_ = sd->open(path, true, pool);
            if (writing_ == nullptr || !writing_) {
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

        switch (write_row(record)) {
        case Success: {
            break;
        }
        case Debug: {
            if (!write_debug(record.readings.meta, meta_file, record.readings.reading, reading, loop_pool)) {
                logerror("error writing diagnostics");
                return;
            }
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

bool ExportDataWorker::lookup_meta(uint32_t meta_record_number, File &meta_file, Pool &pool) {
    if (meta_record_number_ == meta_record_number) {
        return true;
    }

    loginfo("reading meta %" PRIu32, meta_record_number);

    if (!meta_file.seek(meta_record_number)) {
        logerror("error seeking meta record");
        return false;
    }

    meta_pool_.clear();

    SignedRecordLog srl{ meta_file };
    if (!srl.decode(&meta_record_.for_decoding(meta_pool_), fk_data_DataRecord_fields, meta_pool_)) {
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
    auto modules_array = reinterpret_cast<pb_array_t *>(meta_record_.record().modules.arg);
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
    auto modules_array = reinterpret_cast<pb_array_t *>(meta_record_.record().modules.arg);
    auto sensor_groups_array = reinterpret_cast<pb_array_t *>(record.readings.sensorGroups.arg);

    auto modules = reinterpret_cast<fk_data_ModuleInfo *>(modules_array->buffer);
    auto sensor_groups = reinterpret_cast<fk_data_SensorGroup *>(sensor_groups_array->buffer);

    StackBufferedWriter<StackBufferSize> writer{ writing_ };

    writer.write("%" PRIu64 ",%" PRIu32 ",%" PRIu32 ",%" PRIu32 ",%" PRIu32 ",%f,%f,%f,%" PRIu32 ",",
                 record.readings.time, record.readings.reading,
                 record.readings.meta, record.readings.uptime,
                 record.readings.location.fix,
                 record.readings.location.latitude,
                 record.readings.location.longitude,
                 record.readings.location.altitude,
                 record.readings.location.time);

    if (modules_array->length != sensor_groups_array->length) {
        writer.write("modules-mismatch\n");
        return WriteStatus::Debug;
    } else {
        writer.write(",");
    }

    for (auto i = 0u; i < sensor_groups_array->length; ++i) {
        auto &sensor_group = sensor_groups[i];
        auto sensor_values_array = reinterpret_cast<pb_array_t *>(sensor_group.readings.arg);
        auto sensor_values = reinterpret_cast<fk_data_SensorAndValue*>(sensor_values_array->buffer);

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

static bool dump_record(uint8_t *buffer, char *line, File &file) {
    auto record_size = file.record_size();
    auto bytes_read = 0u;
    while (bytes_read < record_size) {
        auto reading = std::min<int32_t>(record_size - bytes_read, 1024);
        auto nread = file.read(buffer, reading);
        if (nread != reading) {
            logerror("error reading");
            return false;
        }

        auto ptr = buffer;
        auto wrote = 0;
        while (wrote < nread) {
            auto line_size = std::min<int32_t>(nread - wrote, 32);

            if (line_size > 0) {
                auto s = line;
                for (auto i = 0; i < line_size; ++i) {
                    tiny_snprintf(s, 4, "%02x ", ptr[i]);
                    s += 3;
                }
                s[-1] = 0;
            }

            logdebug("[%4d/%4" PRIu32 "] %s", wrote, record_size, line);

            wrote += line_size;
            ptr += line_size;
        }
        bytes_read += nread;
    }

    return true;
}

bool ExportDataWorker::write_debug(uint32_t meta_record_number, File &meta_file, uint32_t data_record_number, File &data_file, Pool &pool) {
    auto buffer = reinterpret_cast<uint8_t*>(pool.malloc(1024));
    auto line = reinterpret_cast<char*>(pool.malloc(32 * 3 + 1));

    if (!meta_file.rewind()) {
        logerror("error seeking meta record");
        return false;
    }

    if (!dump_record(buffer, line, meta_file)) {
        return false;
    }

    meta_file.skip();

    if (!data_file.rewind()) {
        logerror("error seeking data record");
        return false;
    }

    if (!dump_record(buffer, line, data_file)) {
        return false;
    }

    data_file.skip();

    return true;
}

} // namespace fk
