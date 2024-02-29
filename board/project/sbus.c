#include "sbus.h"

static sensor_sbus_t *sbus_sensor[32] = {NULL};
static uint packet_id;

static void process();
static int64_t send_slot_callback(alarm_id_t id, void *parameters);
static inline void send_slot(uint8_t slot);
static uint16_t format(uint8_t data_id, float value);
static void add_sensor(uint8_t slot, sensor_sbus_t *new_sensor);
static void set_config();
static uint8_t get_slot_id(uint8_t slot);

void sbus_task(void *parameters)
{
    led_cycle_duration = 6;
    led_cycles = 1;
    led_color = GREEN;
    led_repeat = true;

    set_config();
    uart0_begin(100000, UART_RECEIVER_TX, UART_RECEIVER_RX, SBUS_TIMEOUT_US, 8, 2, UART_PARITY_EVEN, true);
    if (debug)
        printf("\nSbus init");
    while (1)
    {
        ulTaskNotifyTakeIndexed(1, pdTRUE, portMAX_DELAY);
        process();
    }
}

static void process()
{
    if (uart0_available() == SBUS_PACKET_LENGHT)
    {
        uint8_t data[SBUS_PACKET_LENGHT];
        uart0_read_bytes(data, SBUS_PACKET_LENGHT);
        if (debug)
        {
            printf("\nSbus (%u) < ", uxTaskGetStackHighWaterMark(NULL));
            for (uint8_t i = 0; i < SBUS_PACKET_LENGHT; i++)
            {
                printf("%X ", data[i]);
            }
        }
        if (data[0] == 0x0F)
        {
            if (data[24] == 0x04 || data[24] == 0x14 || data[24] == 0x24 || data[24] == 0x34)
            {
                packet_id = data[24] >> 4;
                add_alarm_in_us(SBUS_SLOT_0_DELAY /*- uart0_get_time_elapsed()*/, send_slot_callback, NULL, true);

                /* added for visual */
                vTaskResume(led_task_handle);
                if (debug)
                    printf("\nSbus (%u) > ", uxTaskGetStackHighWaterMark(NULL));
            }
        }
    }
}

static int64_t send_slot_callback(alarm_id_t id, void *parameters)
{
    static uint8_t index = 0;
    uint8_t slot = index + packet_id * 8;
    send_slot(slot);
    if (index < 7)
    {
        index++;
        return SBUS_INTER_SLOT_DELAY;
    }
    index = 0;
    return 0;
}

static inline void send_slot(uint8_t slot)
{
    if (debug == 2)
    {
        static uint32_t timestamp;
        if (slot == 0 || slot == 8 || slot == 16 || slot == 24)
            printf(" %u", uart0_get_time_elapsed());
        else
            printf(" %u", time_us_32() - timestamp);
        timestamp = time_us_32();
    }
    if (debug)
        printf(" (%u)", slot);
    uint16_t value = 0;
    if (sbus_sensor[slot])
    {
        if (sbus_sensor[slot]->value)
            value = format(sbus_sensor[slot]->data_id, *sbus_sensor[slot]->value);
        uint8_t data[3];
        data[0] = get_slot_id(slot);
        data[1] = value;
        data[2] = value >> 8;

        uart0_write_bytes(data, 3);
        if (debug)
            printf("%X:%X:%X ", data[0], data[1], data[2]);
    }
}

static uint16_t format(uint8_t data_id, float value)
{

// SCORPION SENSOR
    if (data_id == FASST_SCOR_VOLT)
    {
        return __builtin_bswap16((uint16_t)round(value * 100) | 0x8000);
    }
    if (data_id == FASST_SCOR_CURR)
    {
        return __builtin_bswap16((uint16_t)round(value * 100));
    }
    if (data_id == FASST_SCOR_CONS)
    {
        return __builtin_bswap16((uint16_t)round(value * 10));
    }
    if (data_id == FASST_SCOR_RPM)
    {
        return __builtin_bswap16((uint16_t)round(value / 6));
    }
    if (data_id == FASST_SCOR_TEMP)
    {
        return __builtin_bswap16((uint16_t)round(value));
    }
    if (data_id == FASST_SCOR_PWM)
    {
        return __builtin_bswap16((uint16_t)round(value));
    }

// Hobbywing SENSOR
    if (data_id == FASST_HW_CURR)
    {
        return __builtin_bswap16((uint16_t)round(value * 10));
    }
    if (data_id == FASST_HW_VOLT)
    {
        return __builtin_bswap16((uint16_t)round(value * 10));
    }
    if (data_id == FASST_HW_CONS)
    {
        return __builtin_bswap16((uint16_t)round(value));
    }
    if (data_id == FASST_HW_RPM)
    {
        return __builtin_bswap16((uint16_t)round(value / 6));
    }
    if (data_id == FASST_HW_TEMP)
    {
        return __builtin_bswap16((uint16_t)round(value) | 0x4000);
    }
    if (data_id == FASST_HW_TEMP2)
    {
        return __builtin_bswap16((uint16_t)round(value));
    }
    if (data_id == FASST_HW_PWM)
    {
        return __builtin_bswap16((uint16_t)round(value));
    }
    if (data_id == FASST_HW_STATUS)
    {
        return __builtin_bswap16((uint16_t)round(1));
    }



// Legacy SENSOR

    if (data_id == FASST_RPM || data_id == FASST_PWM)
    {
        return (uint16_t)round(value / 6);
    }
    if (data_id == FASST_TEMP)
    {
        return (uint16_t)round(value + 100) | 0X8000;
    }
    if (data_id == FASST_VOLT_V1)
    {
        return __builtin_bswap16((uint16_t)round(value * 10) | 0x8000);
    }
    if (data_id == FASST_VOLT_V2)
    {
        return __builtin_bswap16((uint16_t)round(value * 10));
    }
    if (data_id == FASST_VARIO_SPEED)
    {
        return __builtin_bswap16((int16_t)round(value * 100));
    }
    if (data_id == FASST_VARIO_ALT)
    {
        return __builtin_bswap16((int16_t)round(value) | 0x4000);
    }
    if (data_id == FASST_POWER_CURR)
    {
        return __builtin_bswap16((uint16_t)round(value * 100) | 0x4000);
    }
    if (data_id == FASST_POWER_VOLT)
    {
        return __builtin_bswap16((uint16_t)round((value)*100));
    }
    if (data_id == FASST_AIR_SPEED)
    {
        return __builtin_bswap16((uint16_t)round(value) | 0x4000);
    }
    if (data_id == FASST_GPS_SPEED)
    {
        return __builtin_bswap16((uint16_t)round(value * 1.852) | 0x4000);
    }
    if (data_id == FASST_GPS_VARIO_SPEED)
    {
        return __builtin_bswap16((int16_t)round(value * 10));
    }
    if (data_id == FASST_GPS_ALTITUDE)
    {
        return __builtin_bswap16((int16_t)round(value) | 0x4000);
    }
    if (data_id == FASST_GPS_LATITUDE1 || data_id == FASST_GPS_LONGITUDE1)
    {
        // FFFF = (deg,deg,S/W,min) -> min *10000 (prec 4)
        uint16_t lat;
        if (value < 0)
        {
            lat = 1 << FASST_SOUTH_WEST_BIT;
            value *= -1;
        }
        uint8_t degrees = value / 60;
        lat |= degrees << 8;
        uint32_t minutes = fmod(value, 60) * 10000; // minutes precision 4
        lat |= minutes >> 16;
        return __builtin_bswap16(lat);
    }
    if (data_id == FASST_GPS_LATITUDE2 || data_id == FASST_GPS_LONGITUDE2)
    {
        // FFFF = (min) -> min *10000 (prec 4)
        if (value < 0)
        {
            value *= -1;
        }
        uint32_t minutes = fmod(value, 60) * 10000; // minutes precision 4
        return __builtin_bswap16(minutes);
    }
    if (data_id == FASST_GPS_TIME)
    {
        if (value > 120000)
            value -= 120000;
        uint8_t hours = value / 10000;
        uint8_t minutes = (uint8_t)(value / 100) - hours * 100;
        uint8_t seconds = value - hours * 10000 - minutes * 100;
        return __builtin_bswap16(hours * 3600 + minutes * 60 + seconds);
    }
    return __builtin_bswap16(round(value));
}

static uint8_t get_slot_id(uint8_t slot)
{
    uint8_t slot_id[32] = {0x03, 0x83, 0x43, 0xC3, 0x23, 0xA3, 0x63, 0xE3,
                           0x13, 0x93, 0x53, 0xD3, 0x33, 0xB3, 0x73, 0xF3,
                           0x0B, 0x8B, 0x4B, 0xCB, 0x2B, 0xAB, 0x6B, 0xEB,
                           0x1B, 0x9B, 0x5B, 0xDB, 0x3B, 0xBB, 0x7B, 0xFB};
    return slot_id[slot];
}

static void add_sensor(uint8_t slot, sensor_sbus_t *new_sensor)
{
    sbus_sensor[slot] = new_sensor;
}

static void set_config(sensor_sbus_t *sensor[])
{
    config_t *config = config_read();
    TaskHandle_t task_handle;
    sensor_sbus_t *new_sensor;
 
   
    if (config->esc_protocol == ESC_HW5)
    {
        esc_hw5_parameters_t parameter = {config->pairOfPoles, config->alpha_current,
                                          malloc(sizeof(float)), malloc(sizeof(float)), malloc(sizeof(float)), malloc(sizeof(float)), malloc(sizeof(float)), malloc(sizeof(float)), malloc(sizeof(float)), malloc(sizeof(float)), malloc(sizeof(uint8_t))};
        xTaskCreate(esc_hw5_task, "esc_hw5_task", STACK_ESC_HW5, (void *)&parameter, 2, &task_handle);
        uart1_notify_task_handle = task_handle;
        xQueueSendToBack(tasks_queue_handle, task_handle, 0);
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);


#ifdef SBUS_HOBBYWING_SENSOR
        // HOBBYWING SENSOR
        new_sensor = malloc(sizeof(sensor_sbus_t));
        *new_sensor = (sensor_sbus_t){FASST_HW_CURR, parameter.current};
        add_sensor(SBUS_SLOT_HW_POWER_CURR, new_sensor);

        new_sensor = malloc(sizeof(sensor_sbus_t));
        *new_sensor = (sensor_sbus_t){FASST_HW_VOLT, parameter.voltage};
        add_sensor(SBUS_SLOT_HW_POWER_VOLT, new_sensor);

        new_sensor = malloc(sizeof(sensor_sbus_t));
        *new_sensor = (sensor_sbus_t){FASST_HW_CONS, parameter.consumption};
        add_sensor(SBUS_SLOT_HW_POWER_CONS, new_sensor);

        new_sensor = malloc(sizeof(sensor_sbus_t));
        *new_sensor = (sensor_sbus_t){FASST_HW_RPM, parameter.rpm};
        add_sensor(SBUS_SLOT_HW_RPM, new_sensor);

        new_sensor = malloc(sizeof(sensor_sbus_t));
        *new_sensor = (sensor_sbus_t){FASST_HW_TEMP, parameter.temperature_fet};
        add_sensor(SBUS_SLOT_HW_TEMP1, new_sensor);

        new_sensor = malloc(sizeof(sensor_sbus_t));
        *new_sensor = (sensor_sbus_t){FASST_HW_TEMP2, parameter.temperature_bec};
        add_sensor(SBUS_SLOT_HW_TEMP2, new_sensor);

        new_sensor = malloc(sizeof(sensor_sbus_t));
        *new_sensor = (sensor_sbus_t){FASST_HW_PWM, parameter.pwm_percentage};
        add_sensor(SBUS_SLOT_HW_PWM, new_sensor);

        new_sensor = malloc(sizeof(sensor_sbus_t));
        *new_sensor = (sensor_sbus_t){FASST_HW_STATUS, 0};
        add_sensor(SBUS_SLOT_HW_STATUS, new_sensor);

        // SCORPION SENSOR
        // new_sensor = malloc(sizeof(sensor_sbus_t));
        // *new_sensor = (sensor_sbus_t){FASST_SCOR_VOLT, parameter.voltage};
        // add_sensor(SBUS_SLOT_SCOR_POWER_VOLT, new_sensor);

        // new_sensor = malloc(sizeof(sensor_sbus_t));
        // *new_sensor = (sensor_sbus_t){FASST_SCOR_CONS, parameter.consumption};
        // add_sensor(SBUS_SLOT_SCOR_POWER_CONS, new_sensor);

        // new_sensor = malloc(sizeof(sensor_sbus_t));
        // *new_sensor = (sensor_sbus_t){FASST_SCOR_RPM, parameter.rpm};
        // add_sensor(SBUS_SLOT_SCOR_RPM, new_sensor);

        // new_sensor = malloc(sizeof(sensor_sbus_t));
        // *new_sensor = (sensor_sbus_t){FASST_SCOR_CURR, parameter.current};
        // add_sensor(SBUS_SLOT_SCOR_POWER_CURR, new_sensor);

        // new_sensor = malloc(sizeof(sensor_sbus_t));
        // *new_sensor = (sensor_sbus_t){FASST_SCOR_TEMP, parameter.temperature_fet};
        // add_sensor(SBUS_SLOT_SCOR_TEMP1, new_sensor);

        // new_sensor = malloc(sizeof(sensor_sbus_t));
        // *new_sensor = (sensor_sbus_t){FASST_SCOR_TEMP, parameter.temperature_bec};
        // add_sensor(SBUS_SLOT_SCOR_TEMP2, new_sensor);

        // new_sensor = malloc(sizeof(sensor_sbus_t));
        // *new_sensor = (sensor_sbus_t){FASST_SCOR_CURR, 0};
        // add_sensor(SBUS_SLOT_SCOR_BEC_CURR, new_sensor);

        // new_sensor = malloc(sizeof(sensor_sbus_t));
        // *new_sensor = (sensor_sbus_t){FASST_SCOR_PWM, parameter.pwm_percentage};
        // add_sensor(SBUS_SLOT_SCOR_PWM, new_sensor);

        // // PWM SENSOR
        // new_sensor = malloc(sizeof(sensor_sbus_t));
        // *new_sensor = (sensor_sbus_t){FASST_PWM, parameter.pwm_percentage};
        // add_sensor(SBUS_SLOT_PWM, new_sensor);
#endif

#ifndef SBUS_HOBBYWING_SENSOR
        // LEGACY SENSOR
        new_sensor = malloc(sizeof(sensor_sbus_t));
        *new_sensor = (sensor_sbus_t){FASST_RPM, parameter.rpm};
        add_sensor(SBUS_SLOT_RPM, new_sensor);
        new_sensor = malloc(sizeof(sensor_sbus_t));
        *new_sensor = (sensor_sbus_t){FASST_POWER_VOLT, parameter.voltage};
        add_sensor(SBUS_SLOT_POWER_VOLT1, new_sensor);
        new_sensor = malloc(sizeof(sensor_sbus_t));
        *new_sensor = (sensor_sbus_t){FASST_POWER_CURR, parameter.current};
        add_sensor(SBUS_SLOT_POWER_CURR1, new_sensor);
        new_sensor = malloc(sizeof(sensor_sbus_t));
        *new_sensor = (sensor_sbus_t){FASST_POWER_CONS, parameter.consumption};
        add_sensor(SBUS_SLOT_POWER_CONS1, new_sensor);
        new_sensor = malloc(sizeof(sensor_sbus_t));
        *new_sensor = (sensor_sbus_t){FASST_TEMP, parameter.temperature_fet};
        add_sensor(SBUS_SLOT_TEMP1, new_sensor);
        new_sensor = malloc(sizeof(sensor_sbus_t));
        *new_sensor = (sensor_sbus_t){FASST_TEMP, parameter.temperature_bec};
        add_sensor(SBUS_SLOT_TEMP2, new_sensor);

        // PWM SENSOR
        new_sensor = malloc(sizeof(sensor_sbus_t));
        *new_sensor = (sensor_sbus_t){FASST_PWM, parameter.pwm_percentage};
        add_sensor(SBUS_SLOT_PWM, new_sensor);
       
        // new_sensor = malloc(sizeof(sensor_sbus_t));
        //*new_sensor = (sensor_sbus_t){AFHDS2A_ID_CELL_VOLTAGE, parameter.cell_voltage};
        // add_sensor(new_sensor);
#endif

    }    
}
