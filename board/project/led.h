#ifndef LED_H
#define LED_H

#include <FreeRTOS.h>
#include <task.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "config.h"
#include "ws2812.h"


/* Led states returned by eLedColor. */
typedef enum
{
    GREEN = 1,      /* Device is On. */
    RED,       /* There is a failure. */
    BLUE,     /* The Receiver is sending data. */
    VIOLET   /* The Esc is sending data. */
    
} eLedColor;

extern uint16_t led_cycle_duration;
extern uint8_t led_cycles;
extern eLedColor led_color;

extern uint8_t debug;

void led_task();

#endif