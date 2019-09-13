#include <string.h>

#include "unwritten.h"

int32_t unwritten_readings_initialize(unwritten_readings_t *ur) {
    ur->head = 0;
    ur->tail = 0;
    return 1;
}

int32_t unwritten_readings_push(unwritten_readings_t *ur, fk_weather_t *w) {
    int8_t n = (ur->head + 1) % UNWRITTEN_QUEUE_SIZE;
    if (n != ur->tail) {
        memcpy(&ur->readings[ur->head], w, sizeof(fk_weather_t));
        ur->head = n;
    }
    return 1;
}

int32_t unwritten_readings_pop(unwritten_readings_t *ur, fk_weather_t **w) {
    if (ur->head == ur->tail) {
        if (w != NULL) {
            (*w) = NULL;
        }
        return 0;
    }
    if (w != NULL) {
        (*w) = &ur->readings[ur->tail];
    }
    ur->tail = (ur->tail + 1) % UNWRITTEN_QUEUE_SIZE;
    return 1;
}

int32_t unwritten_readings_peek(unwritten_readings_t *ur, fk_weather_t **w) {
    if (ur->head == ur->tail) {
        (*w) = NULL;
        return 0;
    }
    (*w) = &ur->readings[ur->tail];
    return 1;
}
