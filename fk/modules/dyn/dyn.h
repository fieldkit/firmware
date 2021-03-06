#pragma once

#include <loading.h>

namespace fk {

void fkb_log_header(fkb_header_t const *fkbh);

} // namespace fk

extern "C" {

void fk_dyn_run(void *got, void *entry);

}
