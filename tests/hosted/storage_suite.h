#pragma once

#include "tests.h"
#include "pool.h"
#include "hal/linux/linux.h"
#include "storage/storage.h"
#include "patterns.h"
#include "protobuf.h"

namespace fk {

class StorageSuite : public ::testing::Test {
protected:
    StandardPool pool_{ "storage" };
    LinuxDataMemory *banks_[MemoryFactory::NumberOfDataMemoryBanks] = { nullptr };
    DataMemory *data_memory_{ nullptr };
    StatisticsMemory statistics_memory_{ data_memory_ };
    MemoryStatistics statistics_;
    DataMemory *memory_{ &statistics_memory_ };
    FlashGeometry g_;

protected:
    void SetUp() override {
        auto opaque = MemoryFactory::get_data_memory_banks();
        for (size_t i = 0; i < MemoryFactory::NumberOfDataMemoryBanks; ++i) {
            banks_[i] = reinterpret_cast<LinuxDataMemory*>(opaque[i]);
        }

        data_memory_ = MemoryFactory::get_data_memory();
        statistics_memory_ = StatisticsMemory{ data_memory_ };

        memory_->begin();

        g_ = memory_->geometry();
        pool_.clear();

        for (size_t i = 0; i < MemoryFactory::NumberOfDataMemoryBanks; ++i) {
            banks_[i]->erase_all();
        }
    }

    void TearDown() override {
        auto erases = 0;
        auto reads = 0;
        auto writes = 0;

        for (size_t i = 0; i < MemoryFactory::NumberOfDataMemoryBanks; ++i) {
            auto &log = banks_[i]->log();

            erases += log.number_of(OperationType::EraseBlock);
            reads += log.number_of(OperationType::Read);
            writes += log.number_of(OperationType::Write);

            banks_[i]->clear_bad_regions();
        }

        clear_statistics();

        statistics_.log();
    }

protected:
    void enable_info() {
        log_configure_level(LogLevels::INFO);
    }

    void enable_debug() {
        log_configure_level(LogLevels::DEBUG);
    }

    void enable_trace() {
        log_configure_level(LogLevels::TRACE);
    }

    void enable_verbose() {
        log_configure_level(LogLevels::VERBOSE);
    }

    void clear_logs() {
        for (size_t i = 0; i < MemoryFactory::NumberOfDataMemoryBanks; ++i) {
            auto &log = banks_[i]->log();
            log.clear();
        }
    }

    void clear_statistics() {
        statistics_.add(statistics_memory_.statistics());
        statistics_memory_.statistics() = { };
    }

    LinuxDataMemory &bank(int32_t i) {
        return *banks_[i];
    }

};

}
