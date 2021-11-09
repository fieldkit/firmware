#pragma once

#include "common.h"
#include "hal/board.h"

namespace fk {

struct Mpl3115a2Reading {
    float pressure{ 0 };
    float temperature{ 0 };
};

class Mpl3115a2 {
private:
    TwoWireWrapper &bus_;

public:
    Mpl3115a2(TwoWireWrapper &bus);
    virtual ~Mpl3115a2();

public:
    bool begin();
    bool get(Mpl3115a2Reading *reading);
};

} // namespace fk