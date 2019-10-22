#include "storage/progress_tracker.h"
#include "platform.h"
#include "config.h"

namespace fk {

ProgressTracker::ProgressTracker(ProgressCallbacks *callbacks, Operation op, const char *facility, const char *prefix, uint32_t total)
    : callbacks_(callbacks), op_(op), facility_(facility), prefix_(prefix), total_(total) {
}

void ProgressTracker::update(int32_t bytes) {
    update(bytes, total_);
}

void ProgressTracker::update(int32_t bytes, uint32_t total) {
    auto now = fk_uptime();

    total_ = total;

    if (started_ == 0) {
        started_ = now;
        status_ = now + ProgressIntervalMs;
        callbacks_->progress(op_, 0.0f);
    }
    bytes_ += bytes;

    if (now >= status_ || done()) {
        auto speed = ((bytes_ / 1024.0f) / (elapsed() / 1000.0f));
        auto progress = (bytes_ / (float)total_) * 100.0f;
        alogf(LogLevels::INFO, facility_, "%s%" PRIu32 "/%" PRIu32 " bytes (%.2f kbps) %.2f%%", prefix_, bytes_, total_, speed, progress);
        status_ = now + ProgressIntervalMs;
        callbacks_->progress(op_, progress);
    }
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
