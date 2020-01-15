#pragma once

namespace fk {

class Activity {
private:
    uint32_t created_;

public:
    Activity() {
    }

    Activity(uint32_t created) : created_(created) {
    }

public:
    virtual void consumed() {
        created_ = 0;
    }

    bool valid() const {
        return created_ > 0;
    }

};

}
