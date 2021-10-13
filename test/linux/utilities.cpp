/**
 * This software is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option) any
 * later version.
 *
 * This is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this source code. If not, see <http://www.gnu.org/licenses/>.
 */
#include "utilities.h"

#include <internal.h>

void PrintTo(os_task_t *task, std::ostream *os) {
    if (task == NULL) {
        *os << "<null>";
    } else {
        *os << "T<'" << task->name << "'>";
    }
}

void PrintTo(volatile os_task_t *task, std::ostream *os) {
    if (task == NULL) {
        *os << "<null>";
    } else {
        *os << "T<'" << task->name << "'>";
    }
}

os_task_t *tests_task_switch(void) {
    OS_ASSERT(osg.scheduled != NULL);

    osg.running = osg.scheduled;
    osg.scheduled = NULL;

    return (os_task_t *)osg.running;
}

os_task_t *tests_schedule_task_and_switch() {
    osi_schedule();
    if (osg.scheduled != NULL) {
        return tests_task_switch();
    }
    return (os_task_t *)osg.running;
}

os_task_t *tests_sleep_task(os_task_t &task) {
    osi_task_status_set(&task, OS_TASK_STATUS_WAIT);
    task.delay = os_uptime() + 1000;
    return &task;
}

os_task_t *tests_sleep_running_task() {
    tests_sleep_task(*(os_task_t *)osg.running);
    return tests_schedule_task_and_switch();
}

void tests_dump_runqueue() {
    std::cerr << "osg.rq =";
    for (auto iter = osg.runqueue; iter != NULL; iter = iter->nrp) {
        std::cerr << " T<'" << iter->name << "' " << os_task_status_str(iter->status) << ">";
        if (osg.running == iter)
            std::cerr << "*R*";
        if (osg.scheduled == iter)
            std::cerr << "*S*";
    }
    std::cerr << std::endl;
}

void tests_dump_waitqueue() {
    std::cerr << "osg.wq =";
    for (auto iter = osg.waitqueue; iter != NULL; iter = iter->nrp) {
        std::cerr << " T<'" << iter->name << "' " << os_task_status_str(iter->status) << ">";
        if (osg.running == iter)
            std::cerr << "*R*";
        if (osg.scheduled == iter)
            std::cerr << "*S*";
    }
    std::cerr << std::endl;
}

void task_handler_idle(void *p) {
}

void task_handler_test(void *p) {
}

extern "C" {

void osi_assert(const char *assertion, const char *file, int line) {
    FAIL() << "Assertion \"" << assertion << "\" failed. File: " << file << " Line: " << line;
}
}
