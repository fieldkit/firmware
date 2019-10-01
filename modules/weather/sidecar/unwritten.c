#include <string.h>

#include "sidecar.h"
#include "unwritten.h"

uint32_t fk_weather_sign(fk_weather_t const *weather);

int32_t unwritten_readings_initialize(unwritten_readings_t *ur) {
    ur->head = 0;
    ur->tail = 0;
    return FK_SUCCESS;
}

int32_t unwritten_readings_push(unwritten_readings_t *ur, fk_weather_t *w) {
    int8_t n = (ur->head + 1) % UNWRITTEN_QUEUE_SIZE;
    if (n != ur->tail) {
        memcpy(&ur->readings[ur->head], w, sizeof(fk_weather_t));
        ur->head = n;
    }
    return FK_SUCCESS;
}

int32_t unwritten_readings_pop(unwritten_readings_t *ur, fk_weather_t **w) {
    if (ur->head == ur->tail) {
        if (w != NULL) {
            (*w) = NULL;
        }
        return FK_ERROR_EMPTY;
    }
    if (w != NULL) {
        (*w) = &ur->readings[ur->tail];
    }
    ur->tail = (ur->tail + 1) % UNWRITTEN_QUEUE_SIZE;
    return FK_SUCCESS;
}

int32_t unwritten_readings_peek(unwritten_readings_t *ur, fk_weather_t **w) {
    if (ur->head == ur->tail) {
        (*w) = NULL;
        return FK_ERROR_EMPTY;
    }
    (*w) = &ur->readings[ur->tail];
    return FK_SUCCESS;
}

int32_t unwritten_readings_push_error(unwritten_readings_t *ur, uint32_t error, uint32_t memory_failures, uint32_t reading_failures) {
    int8_t n = (ur->head + 1) % UNWRITTEN_QUEUE_SIZE;
    if (n != ur->tail) {
        memset(&ur->readings[ur->head], 0, sizeof(fk_weather_t));
        ur->readings[ur->head].error = error;
        ur->readings[ur->head].reading_failures = reading_failures;
        ur->readings[ur->head].memory_failures = memory_failures;
        ur->readings[ur->head].crc = fk_weather_sign(&ur->readings[ur->head]);
        ur->head = n;
    }
    return FK_SUCCESS;
}

int32_t unwritten_readings_get_size(unwritten_readings_t *ur) {
    int32_t delta = ur->head - ur->tail;

    if (delta < 0) {
        return UNWRITTEN_QUEUE_SIZE + delta;
    }

    return delta;
}
