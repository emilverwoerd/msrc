#ifndef ESC_HW5_H
#define ESC_HW5_H

#include <FreeRTOS.h>
#include <task.h>
#include <stdio.h>
#include <queue.h>
#include "pico/stdlib.h"

#include "uart.h"
#include "cell_count.h"
#include "auto_offset.h"
#include "common.h"
#include "config.h"

#include "led.h"

#define ESC_HW5_TIMEOUT_US 2000

typedef struct esc_hw5_parameters_t
{
    uint8_t pairOfPoles;
    float alpha_current;
    float *rpm, *voltage, *current, *temperature_fet, *temperature_bec, *cell_voltage, *consumption, *pwm_percentage;
    uint8_t *cell_count;
} esc_hw5_parameters_t;


extern TaskHandle_t pwm_out_task_handle, receiver_task_handle;
extern QueueHandle_t tasks_queue_handle;
extern uint8_t debug;

void esc_hw5_task(void *parameters);

#endif