#ifndef ESC_SCORP_H
#define ESC_SCORP_H

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

#define ESC_SCORP_TIMEOUT_US 2000

typedef struct esc_scorp_parameters_t
{
    float *rpm, *voltage, *current, *temperature, *consumption, *pwm, *throttle;
} esc_scorp_parameters_t;


extern TaskHandle_t pwm_out_task_handle, receiver_task_handle;
extern QueueHandle_t tasks_queue_handle;
extern uint8_t debug;

void esc_scorp_task(void *parameters);

#endif