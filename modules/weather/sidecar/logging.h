#pragma once

#include "sidecar.h"

#include <SEGGER_RTT.h>

int32_t loginfo(const char *f);
int32_t loginfof(const char *f, ...);
int32_t logerror(const char *f);
int32_t logerrorf(const char *f, ...);
