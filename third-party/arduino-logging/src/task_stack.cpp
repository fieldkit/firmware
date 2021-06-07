#include "alogging/sprintf.h"
#include "log_stream.h"
#include "logging.h"
#include "task_stack.h"

static task_stack *nullptr_task_stack() {
    return nullptr;
}

log_message_task_stack_fn_t log_task_stack_fn = nullptr_task_stack;

task_stack *log_task_stack_get() {
    return log_task_stack_fn();
}

void log_configure_task_stack(log_message_task_stack_fn_t stack_fn) {
    log_task_stack_fn = stack_fn;
}

task_stack::task_stack(size_t size) {
    memset(value_, 0, sizeof(value_));
    size_ = size;
    stack_ = (size_t *)malloc(sizeof(size_t) * size_);
}

int32_t task_stack::push(const char *name) {
    if (position_ < size_) {
        auto value_len = strlen(value_);
        auto name_len = strlen(name);
        stack_[position_] = value_len;
        memcpy(value_ + value_len, name, name_len);
        value_[value_len + name_len    ] = ' ';
        value_[value_len + name_len + 1] = 0;
        position_++;
        return 1;
    }
    return 0;
}

void task_stack::pop() {
    position_--;
    auto old_len = stack_[position_];
    memset(value_ + old_len, 0, sizeof(value_) - old_len);
}
