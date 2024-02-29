#include "esc_hw5.h"

static void process(esc_hw5_parameters_t *parameter);

#define TELEMETRY_BUFFER_SIZE    40

static uint8_t buffer[TELEMETRY_BUFFER_SIZE] = { 0, };
static uint8_t  readBytes = 0;
static uint32_t syncCount = 0;

static uint32_t dataUpdateUs = 0;
static uint32_t consumptionUpdateUs = 0;

static float consumptionDelta = 0.0f;
static float totalConsumption = 0.0f;


void esc_hw5_task(void *parameters)
{
    esc_hw5_parameters_t parameter = *(esc_hw5_parameters_t *)parameters;
    *parameter.rpm = 0;
    *parameter.voltage = 0;
    *parameter.current = 0;
    *parameter.temperature_fet = 0;
    *parameter.temperature_bec = 0;
    *parameter.cell_voltage = 0;
    *parameter.consumption = 0;
    *parameter.cell_count = 1;
    *parameter.pwm_percentage = 0;
    xTaskNotifyGive(receiver_task_handle);
#ifdef SIM_SENSORS
    *parameter.rpm = 22345.67;
    *parameter.consumption = 12.4;
    *parameter.voltage = 42.34;
    *parameter.current = 126.6;
    *parameter.temperature_fet = 11.34;
    *parameter.temperature_bec = 23.45;
    *parameter.cell_voltage = 3.75;
    *parameter.pwm_percentage = 94.1;
#endif
    
    // TaskHandle_t task_handle;
    // uint cell_count_delay = 15000;
    // cell_count_parameters_t cell_count_parameters = {cell_count_delay, parameter.voltage, parameter.cell_count};
    // xTaskCreate(cell_count_task, "cell_count_task", STACK_CELL_COUNT, (void *)&cell_count_parameters, 1, &task_handle);
    // xQueueSendToBack(tasks_queue_handle, task_handle, 0);

    // float current_offset = 0;
    // uint current_delay = 15000;
    // auto_offset_parameters_t current_offset_parameters = {current_delay, parameter.current, &current_offset};
    // xTaskCreate(auto_offset_task, "esc_hw4_current_offset_task", STACK_AUTO_OFFSET, (void *)&current_offset_parameters, 1, &task_handle);
    // xQueueSendToBack(tasks_queue_handle, task_handle, 0);
   
    uart1_begin(115200, UART1_TX_GPIO, UART_ESC_RX, ESC_HW5_TIMEOUT_US, 8, 1, UART_PARITY_NONE, false);

    while (1)
    {
        ulTaskNotifyTakeIndexed(1, pdTRUE, portMAX_DELAY);
        process(&parameter);
    }
}

static uint16_t calculateCRC16_MODBUS(const uint8_t *ptr, size_t len)
{
    uint16_t crc = ~0;

    while (len--) {
        crc ^= *ptr++;
        for (int i = 0; i < 8; i++)
            crc = (crc & 1) ? (crc >> 1) ^ 0xA001 : (crc >> 1);
    }

    return crc;
}

static void frameSyncError(void)
{
    readBytes = 0;
}

static bool processHW5TelemetryStream(uint8_t dataByte)
{
    buffer[readBytes++] = dataByte;

    if (readBytes == 1) {
        if (dataByte != 0xFE)
            frameSyncError();
    }
    else if (readBytes == 2) {
        if (dataByte != 0x01)
            frameSyncError();
    }
    else if (readBytes == 3) {
        if (dataByte != 0x00)
            frameSyncError();
    }
    else if (readBytes == 4) {
        if (dataByte != 0x03)
            frameSyncError();
    }
    else if (readBytes == 5) {
        if (dataByte != 0x30)
            frameSyncError();
    }
    else if (readBytes == 6) {
        if (dataByte != 0x5C)
            frameSyncError();
    }
    else if (readBytes == 7) {
        if (dataByte != 0x17)
            frameSyncError();
    }
    else if (readBytes == 32) {
        readBytes = 0;
        return true;
    }

    return false;
}

static int32_t cmpTimeUs(uint32_t a, uint32_t b) 
{ 
    return (int32_t)(a - b); 
}

// Only for non-BLHeli32 protocols with single ESC support
static void checkFrameTimeout(uint32_t currentTimeUs, int32_t timeout)
{
    // Increment data age counter if no updates
    if (cmpTimeUs(currentTimeUs, dataUpdateUs) > timeout) {
        dataUpdateUs = currentTimeUs;

        led_color = BLUE;
    }
}

static void setConsumptionCurrent(float current)
{
    // Pre-convert Aµs to mAh
    consumptionDelta = current * (1000.0f / 3600e6f);
}

static void updateConsumption(uint32_t currentTimeUs, float alphaCurrent)
{
    // Increment consumption
    totalConsumption += (cmpTimeUs(currentTimeUs, consumptionUpdateUs) * consumptionDelta) * alphaCurrent;

    // Save update time
    consumptionUpdateUs = currentTimeUs;
}


static void process(esc_hw5_parameters_t *parameter)
{
    uint32_t currentTimeUs = time_us_32();

    // check for any available bytes in the rx buffer
    while (uart1_available()) {
        if (processHW5TelemetryStream(uart1_read())) {
            uint16_t crc = buffer[31] << 8 | buffer[30];

            if (calculateCRC16_MODBUS(buffer, 30) == crc) {
                uint32_t rpm = buffer[14] << 8 | buffer[13];
                uint16_t power = buffer[9];
                uint16_t voltage = buffer[16] << 8 | buffer[15];
                uint16_t current = buffer[18] << 8 | buffer[17];
                uint16_t tempFET = buffer[19];
                uint16_t tempBEC = buffer[20];                               

                // When throttle changes to zero, the last current reading is
                // repeated until the motor has totally stopped.
                if (power == 0) {
                    current = 0;
                }

                setConsumptionCurrent(current * 0.1f);

                *parameter->rpm = (rpm * 10.0) / parameter->pairOfPoles;
                *parameter->consumption = totalConsumption;
                *parameter->voltage = voltage * 1.0;
                *parameter->current = current * 1.0;;
                *parameter->temperature_fet = tempFET * 1.0;
                *parameter->temperature_bec = 0;
                *parameter->cell_voltage = *parameter->voltage / *parameter->cell_count;
                *parameter->pwm_percentage = power * 1.0;

                dataUpdateUs = currentTimeUs;

                led_color = VIOLET;
                led_repeat = false;
            }
        }
    }

    // Update consumption on every cycle
    updateConsumption(currentTimeUs, parameter->alpha_current);

    // Maximum frame spacing 400ms
    checkFrameTimeout(currentTimeUs, 500000);
}