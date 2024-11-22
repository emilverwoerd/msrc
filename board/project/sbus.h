#ifndef SBUS_H
#define SBUS_H

#include <FreeRTOS.h>
#include <task.h>
#include <stdio.h>
#include "pico/stdlib.h"

#include "common.h"
#include "uart.h"
#include "esc_hw5.h"
#include "esc_scorp.h"

/* FASST Sbus Data Id */
#define FASST_NULL 0
#define FASST_TEMP 1
#define FASST_VOLT_V1 2
#define FASST_VOLT_V2 3
#define FASST_RPM 4
#define FASST_POWER_CURR 5
#define FASST_POWER_VOLT 6
#define FASST_POWER_CONS 7
#define FASST_VARIO_ALT 8 // F1672
#define FASST_VARIO_SPEED 9
#define FASST_GPS_SPEED 10 // F1675
#define FASST_GPS_ALTITUDE 11
#define FASST_GPS_TIME 12
#define FASST_GPS_VARIO_SPEED 13
#define FASST_GPS_LATITUDE1 14
#define FASST_GPS_LATITUDE2 15
#define FASST_GPS_LONGITUDE1 16
#define FASST_GPS_LONGITUDE2 17
#define FASST_AIR_SPEED 18
#define FASST_PWM 19
#define FASST_HW_CURR 20
#define FASST_HW_VOLT 21
#define FASST_HW_CONS 22
#define FASST_HW_RPM 23
#define FASST_HW_TEMP 24
#define FASST_HW_TEMP2 25
#define FASST_HW_PWM 26
#define FASST_HW_STATUS 27

#define FASST_SCORP_VOLT 28
#define FASST_SCORP_CONS 29
#define FASST_SCORP_RPM 30
#define FASST_SCORP_CURR 31
#define FASST_SCORP_TEMP 32
#define FASST_SCORP_PWM 33


#define FASST_NEGATIVE_BIT 15
#define FASST_SOUTH_WEST_BIT 4

#define SBUS_WAIT 0
#define SBUS_SEND 1

/**
 * Slots sensor mapping for Futaba transmitters:
 *
 * | Slot   | Sensor                               |
 * | :----: | :----------------------------------: |
 * |0       | RX voltage (reserved)                |
 * |1       | Temperature 1 (SBS-01T/TE)           |
 * |2       | RPM (type magnet)(SBS-01RB/RM/RO)    |
 * |3-4     | Vario-F1672                          |
 * |6-7     | Voltage (SBS-01V)                    |
 * |8-15    | GPS-F1675                            |
 * |16      | Air speed (SBS-01TAS)                |
 * |17-21   | Unused                               |
 * |21-23   | Current 3 (SBS-01C)                  |
 * |24-26   | Current 1 (SBS-01C)                  |
 * |27-29(+)| Current 2 (SBS-01C)                  |
 * |30(+)   | Temperature 2 (SBS-01T/TE)           |
 * |31      | Unused                               |
 *
 * (+) Non default slots
 */

#define SBUS_SLOT_TEMP1 1
#define SBUS_SLOT_RPM 2
#define SBUS_SLOT_VARIO_SPEED 3
#define SBUS_SLOT_VARIO_ALT 4
#define SBUS_SLOT_VARIO_RESSURE 5
#define SBUS_SLOT_VOLT_V1 6
#define SBUS_SLOT_VOLT_V2 7

#define SBUS_SLOT_GPS_SPD 8
#define SBUS_SLOT_GPS_ALT 9
#define SBUS_SLOT_GPS_TIME 10
#define SBUS_SLOT_GPS_VARIO 11
#define SBUS_SLOT_GPS_LAT1 12
#define SBUS_SLOT_GPS_LAT2 13
#define SBUS_SLOT_GPS_LON1 14
#define SBUS_SLOT_GPS_LON2 15

#define SBUS_SLOT_AIR_SPEED 16

#define SBUS_SLOT_POWER_CURR3 21
#define SBUS_SLOT_POWER_VOLT3 22
#define SBUS_SLOT_POWER_CONS3 23
#define SBUS_SLOT_POWER_CURR1 24
#define SBUS_SLOT_POWER_VOLT1 25
#define SBUS_SLOT_POWER_CONS1 26
#define SBUS_SLOT_POWER_CURR2 27
#define SBUS_SLOT_POWER_VOLT2 28
#define SBUS_SLOT_POWER_CONS2 29
#define SBUS_SLOT_TEMP2 30

#define SBUS_SLOT_PWM 16

#define SBUS_SLOT_HW_POWER_CURR 8
#define SBUS_SLOT_HW_POWER_VOLT 9
#define SBUS_SLOT_HW_POWER_CONS 10
#define SBUS_SLOT_HW_RPM 11
#define SBUS_SLOT_HW_TEMP1 12
#define SBUS_SLOT_HW_TEMP2 13
#define SBUS_SLOT_HW_PWM 14
#define SBUS_SLOT_HW_STATUS 15

#define SBUS_SLOT_SCORP_POWER_VOLT 8
#define SBUS_SLOT_SCORP_POWER_CONS 9
#define SBUS_SLOT_SCORP_RPM 10
#define SBUS_SLOT_SCORP_POWER_CURR 11
#define SBUS_SLOT_SCORP_TEMP1 12
#define SBUS_SLOT_SCORP_TEMP2 13
#define SBUS_SLOT_SCORP_BEC_CURR 14
#define SBUS_SLOT_SCORP_PWM 15


#define SBUS_TIMEOUT_US 500
#define SBUS_SLOT_0_DELAY 2000
#define SBUS_INTER_SLOT_DELAY 660
#define SBUS_PACKET_LENGHT 25

typedef struct sensor_sbus_t
{
    uint8_t data_id;
    float *value;
} sensor_sbus_t;

extern QueueHandle_t sensors_queue_handle, tasks_queue_handle;
extern TaskHandle_t pwm_out_task_handle, led_task_handle;
extern uint16_t led_cycle_duration;
extern uint8_t led_cycles;

void sbus_task(void *parameters);

#endif
