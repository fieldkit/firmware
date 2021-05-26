#include "progress_tracker.h"
#include "platform.h"
#include "config.h"
#include "hal/network.h"

namespace fk {

ProgressTracker::ProgressTracker(ProgressCallbacks *callbacks, Operation op, const char *facility, const char *prefix, uint32_t total)
    : callbacks_(callbacks), op_(op), facility_(facility), prefix_(prefix), total_(total) {
}

bool ProgressTracker::update(int32_t bytes) {
    return update(bytes, 0, 0);
}

bool ProgressTracker::update(int32_t bytes, uint32_t total) {
    return update(bytes, 0, 0, total);
}

bool ProgressTracker::update(int32_t bytes, uint32_t read_time, uint32_t write_time) {
    return update(bytes, read_time, write_time, total_);
}

bool ProgressTracker::update(int32_t bytes, uint32_t read_time, uint32_t write_time, uint32_t total) {
    auto now = fk_uptime();

    total_ = total;

    if (started_ == 0) {
        started_ = now;
        status_ = now + ProgressIntervalMs;
        callbacks_->progress(op_, 0.0f);
    }
    bytes_ += bytes;

    if (!done() && now < status_) {
        return false;
    }

    auto elapsed_ms = elapsed();
    auto speed = ((bytes_ / 1024.0f) / (elapsed_ms / 1000.0f));
    auto progress = (bytes_ / (float)total_) * 100.0f;
    alogf(LogLevels::INFO, facility_, "%s%" PRIu32 "/%" PRIu32 " bytes (%.2f kbps) %.2f%% elapsed=%" PRIu32 "ms read-time=%" PRIu32 "ms write-time=%" PRIu32 "ms rssi=%" PRId32,
          prefix_, bytes_, total_, speed, progress, elapsed_ms, read_time, write_time, get_network()->rssi());
    status_ = now + ProgressIntervalMs;
    callbacks_->progress(op_, progress);

    return true;
}

bool ProgressTracker::busy() const {
    return bytes_ < total_;
}

bool ProgressTracker::done() const {
    return !busy();
}

uint32_t ProgressTracker::elapsed() const {
    return fk_uptime() - started_;
}

uint32_t ProgressTracker::bytes() const {
    return bytes_;
}

uint32_t ProgressTracker::remaining_bytes() const {
    return total_ - bytes_;
}

void ProgressTracker::finished() {
    callbacks_->progress(Operation::None, 0.0f);
}

}
