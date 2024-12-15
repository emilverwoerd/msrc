#include "esc_scorp.h"

static void process(esc_scorp_parameters_t *parameter);

#define TELEMETRY_BUFFER_SIZE    40

static uint8_t buffer[TELEMETRY_BUFFER_SIZE] = { 0, };
static uint8_t  readBytes = 0;
static uint32_t syncCount = 0;

static uint32_t dataUpdateUs = 0;

// static uint32_t counter = 0;

void esc_scorp_task(void *parameters)
{
    esc_scorp_parameters_t parameter = *(esc_scorp_parameters_t *)parameters;
    *parameter.rpm = 0;
    *parameter.voltage = 0;
    *parameter.current = 0;
    *parameter.temperature = 0;
    *parameter.consumption = 0;
    *parameter.pwm = 0;
    *parameter.throttle = 0;
    xTaskNotifyGive(receiver_task_handle);
#ifdef SIM_SENSORS
    *parameter.rpm = 1.67;
    *parameter.voltage = 42.34;
    *parameter.consumption = 12.4;    
    *parameter.current = 126.6;
    *parameter.temperature = 3331.34;
    *parameter.throttle = 333.75;
    *parameter.pwm = 1.0;
#endif    
   
    uart1_begin(38400, UART1_TX_GPIO, UART_ESC_RX, ESC_SCORP_TIMEOUT_US, 8, 1, UART_PARITY_NONE, false);

    while (1)
    {
        ulTaskNotifyTakeIndexed(1, pdTRUE, portMAX_DELAY);
        process(&parameter);
        
        // ulTaskNotifyTakeIndexed(1, pdTRUE, 1000);
        // counter += 1;

        // *parameter.rpm += counter;
        // *parameter.pwm += counter;
        
    }
}


static uint16_t calculateCRC16_CCITT(const uint8_t *ptr, size_t len)
{
    uint16_t crc = 0;

    while (len--) {
        crc ^= *ptr++;
        for (int i = 0; i < 8; i++)
            crc = (crc & 1) ? (crc >> 1) ^ 0x8408 : (crc >> 1);
    }

    return crc;
}


static void frameSyncError(void)
{
    readBytes = 0;
}

static bool processUNCTelemetryStream(uint8_t dataByte)
{

    buffer[readBytes++] = dataByte;

    if (readBytes == 1) {
        if (dataByte != 0x55)
            frameSyncError();
    }
    else if (readBytes == 2) {
        if (dataByte != 0x00)  // Proto v0
            frameSyncError();
    }
    else if (readBytes == 3) {
        if (dataByte != 22)  // Message v0 is 22 bytes
            frameSyncError();
        else
            syncCount++;
    }
    else if (readBytes == 22) {
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

static void process(esc_scorp_parameters_t *parameter)
{
    uint32_t currentTimeUs = time_us_32();

    // check for any available bytes in the rx buffer
    while (uart1_available()) {
        if (processUNCTelemetryStream(uart1_read())) {
             uint16_t crc = buffer[21] << 8 | buffer[20];

             if (calculateCRC16_CCITT(buffer, 20) == crc) {
                uint16_t rpm = buffer[18] << 8 | buffer[17];
                uint16_t temp = buffer[14];
                uint16_t throttle = (( 0x00 << 8) | buffer[7]) / 2;
                uint16_t power = (( 0x00 << 8) | buffer[15]) / 2;
                uint16_t voltage = buffer[11] << 8 | buffer[10];
                uint16_t current = buffer[9] << 8 | buffer[8];
                uint16_t capacity = buffer[13] << 8 | buffer[12];
                uint16_t status = buffer[19];

                *parameter->rpm = rpm * 1.0;
                *parameter->voltage = voltage * 0.1;
                *parameter->current = current * 0.1;
                *parameter->consumption = capacity * 0.01;      
                *parameter->temperature = temp * 1.0;  
                
                *parameter->pwm = power  * 1.0;
                *parameter->throttle = throttle  * 1.0;

                dataUpdateUs = currentTimeUs;

                led_color = VIOLET;
                led_repeat = false;
            }
        }
    }

    // Maximum frame spacing 400ms
    checkFrameTimeout(currentTimeUs, 1200000);
}