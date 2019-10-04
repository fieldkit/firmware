#pragma once

#include "pool.h"
#include "storage/storage.h"
#include "storage/signed_log.h"

size_t write_reading(fk::File &file);

void append_metadata_always(fk::SignedRecordLog &srl, uint32_t time, const char *build, const char *git, fk::Pool &pool);

void append_metadata(fk::SignedRecordLog &srl, const char *build, const char *git, fk::Pool &pool);

void append_other_always(fk::SignedRecordLog &srl, const char *build, const char *git, fk::Pool &pool);

void append_other(fk::SignedRecordLog &srl, const char *build, const char *git, fk::Pool &pool);
