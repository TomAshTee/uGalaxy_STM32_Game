#pragma once
#include <stdint.h>
#include <stdbool.h>

typedef struct { int dummy; } ADC_HandleTypeDef;
typedef struct { int dummy; } SPI_HandleTypeDef;
typedef struct { int dummy; } GPIO_TypeDef;

typedef enum {
    GPIO_PIN_RESET = 0,
    GPIO_PIN_SET
} GPIO_PinState;

static inline uint32_t HAL_GetTick(void) { return 0; }