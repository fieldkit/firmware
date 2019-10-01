#pragma once

#include <modules.h>
#include <weather.h>

#define UNWRITTEN_QUEUE_SIZE                     (22)

typedef struct unwritten_readings_t {
    fk_weather_t readings[UNWRITTEN_QUEUE_SIZE];
    uint8_t head;
    uint8_t tail;
} unwritten_readings_t;

int32_t unwritten_readings_initialize(unwritten_readings_t *ur);

int32_t unwritten_readings_push(unwritten_readings_t *ur, fk_weather_t *w);

int32_t unwritten_readings_pop(unwritten_readings_t *ur, fk_weather_t **w);

int32_t unwritten_readings_peek(unwritten_readings_t *ur, fk_weather_t **w);

int32_t unwritten_readings_push_error(unwritten_readings_t *ur, uint32_t error, uint32_t memory_failures, uint32_t reading_failures);

int32_t unwritten_readings_get_size(unwritten_readings_t *ur);
