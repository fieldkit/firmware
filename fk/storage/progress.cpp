#include "storage/progress.h"
#include "platform.h"
#include "config.h"

namespace fk {

ProgressTracker::ProgressTracker(const char *facility, const char *prefix, uint32_t total) : facility_(facility), prefix_(prefix), total_(total) {
}

void ProgressTracker::update(int32_t bytes) {
    auto now = fk_uptime();

    if (started_ == 0) {
        started_ = now;
        status_ = now + ProgressIntervalMs;
    }
    bytes_ += bytes;

    if (now >= status_ || done()) {
        auto speed = ((bytes_ / 1024.0f) / (elapsed() / 1000.0f));
        auto progress = (bytes_ / (float)total_) * 100.0f;
        alogf(LogLevels::INFO, facility_, "%s %" PRIu32 "/%" PRIu32 " bytes (%.2f kbps) %.2f%%", prefix_, bytes_, total_, speed, progress);
        status_ = now + ProgressIntervalMs;
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

}
