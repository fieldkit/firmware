#pragma once

extern "C" {

#if defined(__SAMD51__)

void fk_dyn_run(void *got, void *entry);

#else

void fk_dyn_run(void *got, void *entry) {
}

#endif

}
