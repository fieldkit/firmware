#pragma once

namespace fk {

enum class Error : int32_t {
    None = 0,
    General = -1,
    Bus = -2,
    TooSoon = -3,
    IO = -4,
    EoF = -5,
};

}
