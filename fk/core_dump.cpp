#include <tiny_printf.h>
#include <loading.h>

#include "core_dump.h"
#include "common.h"
#include "hal/hal.h"
#include "tasks/tasks.h"

#if defined(__SAMD51__)

extern "C" {

extern uint32_t __cm_ram_origin__;

extern uint32_t __cm_ram_end__;
}

extern const struct fkb_header_t fkb_header;

namespace fk {

FK_DECLARE_LOGGER("coredump");

#define FK_CORE_DUMP_MAGIC         (0x7dc48f3c)
#define FK_CORE_DUMP_MAGIC_ADDRESS (0x20040000 - sizeof(magic_struct_t))

typedef struct {
    uint32_t key;
    uint32_t address;
} magic_struct_t;

/*
static int32_t generate_path(char *buffer, size_t buffer_size, const char *pattern, size_t max) {
    auto sd_card = get_sd_card();

    for (auto counter = 0u; counter < max; ++counter) {
        tiny_snprintf(buffer, buffer_size, pattern, counter);
        if (!sd_card->is_file(buffer)) {
            loginfo("picked file name %s", buffer);
            return counter;
        }
    }

    return -1;
}
*/

static int32_t delete_if_there(const char *path) {
    auto sd_card = get_sd_card();

    if (sd_card->is_file(path)) {
        if (!sd_card->unlink(path)) {
            logerror("unlink %s", path);
            return -1;
        }
    }

    return 0;
}

static void panic() {
    while (true) {
        logerror("terrible things are happening");
        fk_delay(1000);
    }
}

static magic_struct_t *get_magic() {
    return reinterpret_cast<magic_struct_t *>((uint32_t)FK_CORE_DUMP_MAGIC_ADDRESS);
}

int32_t fk_core_dump_check() {
    auto magic = get_magic();

    if (magic->key != FK_CORE_DUMP_MAGIC) {
        loginfo("core-dump: silenced");

        return 0;
    }

    loginfo("core-dump: magic!");

    magic->key = 0;
    return fk_core_dump(nullptr);
}

int32_t fk_core_dump_trigger() {
    auto magic = get_magic();

    loginfo("core-dump: triggered!");

    magic->key = FK_CORE_DUMP_MAGIC;

    return 0;
}

int32_t fk_core_dump_task(os_task_t *task) {
    auto running = os_task_is_running(task);
    auto queue = task->queue != nullptr ? task->queue->def->name : "<none>";
    auto mutex = task->mutex != nullptr ? task->mutex->def->name : "<none>";
    auto stack_base = task->stack;
    auto stack_size = task->stack_size;

    loginfo("core-dump: task '%s' running=%s sp=0x%x", task->name, running ? "yes" : "no", task->sp);

    loginfo("core-dump: task started=%" PRIu32 " delay=%" PRIu32 " queue=%s mutex=%s", task->started, task->delay, queue, mutex);

    loginfo("core-dump: stack: 0x%x -> 0x%x (%d)", stack_base, (uint32_t)stack_base + stack_size, stack_size);

    uint32_t *stack = (uint32_t *)stack_base;

    fk_logs_dump_memory_u32_filtered("stack: ", OSH_STACK_MAGIC_WORD, stack, stack_size / sizeof(uint32_t));

    return 0;
}

int32_t fk_core_dump_tasks() {
    loginfo("core-dump: tasks");

    os_task_t **all_tasks = fk_tasks_all();
    for (auto iter = all_tasks; *iter != nullptr; iter++) {
        int32_t err = fk_core_dump_task(*iter);
        if (err != 0) {
            return err;
        }
    }

    fk_standard_page_log();

    loginfo("core-dump: done");

    return 0;
}

int32_t fk_core_dump(const char *base_path) {
    StandardPool pool{ __FUNCTION__ };
    char bin_path[32];
    char inf_path[32];

    loginfo("core-dump: starting");

    int32_t err = fk_core_dump_tasks();
    if (err != 0) {
        return err;
    }

    loginfo("core-dump: checking sd");

    auto sd_card = get_sd_card();
    if (!sd_card->begin()) {
        logerror("sd card");
        return -1;
    }

    if (base_path == nullptr) {
        for (auto counter = 0u;; ++counter) {
            tiny_snprintf(bin_path, sizeof(bin_path), "core-%03d.bin", counter);
            tiny_snprintf(inf_path, sizeof(inf_path), "core-%03d.inf", counter);
            if (!sd_card->is_file(bin_path) && !sd_card->is_file(inf_path)) {
                loginfo("picked %s / %s", bin_path, inf_path);
                break;
            }

            if (counter == 1000) {
                panic();
            }
        }
    } else {
        tiny_snprintf(bin_path, sizeof(bin_path), "%s.bin", base_path);
        tiny_snprintf(inf_path, sizeof(inf_path), "%s.inf", base_path);

        if (delete_if_there(bin_path) < 0) {
            panic();
        }

        if (delete_if_there(inf_path) < 0) {
            panic();
        }
    }

    loginfo("core-dump: collecting 0x%x - 0x%x", &__cm_ram_origin__, &__cm_ram_end__);

    auto bin_file = sd_card->open(bin_path, OpenFlags::Write, pool);

    for (uint32_t *ptr = &__cm_ram_origin__; ptr < &__cm_ram_end__;) {
        loginfo("%s: writing 4k of ram to sd", bin_path);

        auto wrote = bin_file->write((uint8_t *)ptr, 4096);
        if (wrote != 4096) {
            logerror("%d != 4096", wrote);
            break;
        }

        ptr += 4096 / sizeof(uint32_t);
    }

    bin_file->close();

    auto inf_file = sd_card->open(inf_path, OpenFlags::Write, pool);

    loginfo("%s: writing", inf_path);

    auto header_bytes = inf_file->write((uint8_t *)&fkb_header, sizeof(fkb_header_t));
    if (header_bytes != sizeof(fkb_header_t)) {
        logerror("%d != sizeof(fkb_header_t)", header_bytes);
    }

    inf_file->close();

    loginfo("core-dump: done");

    return 0;
}

} // namespace fk

#else

namespace fk {

int32_t fk_core_dump(const char *base_path) {
    return 0;
}

int32_t fk_core_dump_tasks() {
    return 0;
}

} // namespace fk

#endif
