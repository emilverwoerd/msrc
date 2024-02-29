#ifndef CONFIG_H
#define CONFIG_H

#include "pico/stdlib.h"
#include <hardware/flash.h>
#include <hardware/sync.h>
#include <string.h>
#include <stdio.h>

#include <FreeRTOS.h>
#include <task.h>
#include <stdio.h>
#include <queue.h>
#include "config.h"

#include "constants.h"

#define CONFIG_FORZE_WRITE false
#define CONFIG_FLASH_TARGET_OFFSET (512 * 1024)
#define CONFIG_VERSION 2

/* Receiver protocol */
#define RX_PROTOCOL RX_SMARTPORT // RX_SMARTPORT, RX_XBUS, RX_SRXL, RX_FRSKY_D, RX_IBUS, RX_SBUS, RX_MULTIPLEX, RX_JETIEX, RX_HITEC
/* Sensors */
#define ESC_PROTOCOL ESC_NONE // ESC_NONE, ESC_HW3, ESC_HW4, ESC_PWM, ESC_CASTLE, ESC_KONTRONIK, ESC_APD_F, ESC_APD_HV

/* Averaging elements (1 = no averaging) */
// #define AVERAGING_ELEMENTS_RPM 1.0
// #define AVERAGING_ELEMENTS_VOLTAGE 1.0
#define AVERAGING_ELEMENTS_CURRENT 1.0


/* Analog voltage sensors */
// #define ANALOG_VOLTAGE_MULTIPLIER 7.8 // 7.8 if using 68k and 10k as proposed in the docs
// /* Analog current sensor */
// #define ANALOG_CURRENT_MULTIPLIER 1 // current_multiplier = 1000 / sensitivity(mV/A)
// /* Zero current output voltage offset or quiescent voltage (voltage offset when I = 0, Viout)
//  - All hall effect core sensors (Amploc) are bidirectional. Viout=Vs/2
//  - Hall effect coreless sensors (IC) (ACS758) can be bidirectional or directional. Recommended to use directional for higher sensitivity. Viout defined in datasheet
//  - If CURRENT_AUTO_OFFSET is true, then after 5 seconds, voltage read is set as offset. It is recommended to use auto offset
// */
// #define ANALOG_CURRENT_OFFSET 0
// #define ANALOG_CURRENT_AUTO_OFFSET true
// #define ANALOG_CURRENT_SENSITIVITY 1000

/* RPM multipliers (optional, this may be done in transmitter*/
#define RPM_PAIR_OF_POLES 1
#define RPM_PINION_TEETH 1 // For helis
#define RPM_MAIN_TEETH 1   // For helis


/* 
   Debug
   Disconnect Vcc from the RC model to the board before connecting USB
   Telemetry may not work properly in debug mode
*/

#define MSRC_DEBUG 0  // 0 = no debug, 1 = debug level 1, 2 = debug level 2

/* Simulate Sensor / ESC */
// #define SIM_SENSORS

//#define SBUS_HOBBYWING_SENSOR
#define SBUS_HOBBYWING_SENSOR


// typedef enum rx_protocol_t
// {
//     RX_SMARTPORT,
//     RX_FRSKY_D,
//     RX_XBUS,
//     RX_SRXL,
//     RX_IBUS,
//     RX_SBUS,
//     RX_MULTIPLEX,
//     RX_JETIEX,
//     RX_HITEC
// } rx_protocol_t;

typedef enum esc_protocol_t
{
    ESC_NONE,
    ESC_HW5,
} esc_protocol_t;


typedef struct config_t
{
    uint16_t version;
    enum esc_protocol_t esc_protocol;
 
    float alpha_current;
    uint8_t pairOfPoles;
    uint8_t debug;
    uint32_t spare1;
    uint32_t spare2;
    uint32_t spare3;
    uint32_t spare4;
    uint32_t spare5;
} config_t;

config_t *config_read();
void config_write(config_t *config);
void config_forze_write();
void config_get(config_t *config);

#endif