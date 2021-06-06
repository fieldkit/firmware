#pragma once

namespace fk {

class Runnable {
public:
    virtual bool is_running() const = 0;
};

} // namespace fk
