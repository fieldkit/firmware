#pragma once

#include <gtest/gtest.h>

#include "common.h"
#include "utilities.h"

namespace fk {

template<size_t N>
class StaticPool : public Pool {
private:
    alignas(sizeof(uint32_t)) typename std::aligned_storage<sizeof(uint8_t), alignof(uint8_t)>::type data[aligned_size(N)];

public:
    StaticPool(const char *name) : Pool(name, aligned_size(N), (void *)data, 0) {
    }

};

}
