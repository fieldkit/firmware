#include "tasks/tasks.h"

#include "config.h"

namespace fk {

void task_handler_idle(void *params) {
    while (true) {
        fk_delay(FiveSecondsMs);
    }
}

}
