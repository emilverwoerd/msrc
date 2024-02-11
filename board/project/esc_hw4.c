#include "esc_hw4.h"

static void process(esc_hw4_parameters_t *parameter, float *current_offset);
float get_voltage(uint16_t voltage_raw, esc_hw4_parameters_t *parameter);
float get_temperature(uint16_t temperature_raw);
float get_current(uint16_t current_raw, float current_offset, esc_hw4_parameters_t *parameter);
float get_pwm_percentage(uint16_t pwm_raw);

void esc_hw4_task(void *parameters)
{
    esc_hw4_parameters_t parameter = *(esc_hw4_parameters_t *)parameters;
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
    TaskHandle_t task_handle;
    uint cell_count_delay = 15000;
    // cell_count_parameters_t cell_count_parameters = {cell_count_delay, parameter.voltage, parameter.cell_count};
    // xTaskCreate(cell_count_task, "cell_count_task", STACK_CELL_COUNT, (void *)&cell_count_parameters, 1, &task_handle);
    // xQueueSendToBack(tasks_queue_handle, task_handle, 0);

    // float current_offset = 0;
    // uint current_delay = 15000;
    // auto_offset_parameters_t current_offset_parameters = {current_delay, parameter.current, &current_offset};
    // xTaskCreate(auto_offset_task, "esc_hw4_current_offset_task", STACK_AUTO_OFFSET, (void *)&current_offset_parameters, 1, &task_handle);
    // xQueueSendToBack(tasks_queue_handle, task_handle, 0);

    //// uart_pio_begin(19200, 5, ESC_HW4_TIMEOUT_US, pio0, PIO0_IRQ_1);
    // uart1_begin(19200, UART1_TX_GPIO, UART_ESC_RX, ESC_HW4_TIMEOUT_US, 8, 1, UART_PARITY_NONE, false);

    while (1)
    {
        sleep_ms(1000);
        *parameter.consumption += 1;

        // ulTaskNotifyTakeIndexed(1, pdTRUE, portMAX_DELAY);
        // process(&parameter, &current_offset);
    }
}

static void process(esc_hw4_parameters_t *parameter, float *current_offset)
{
    uint16_t pwm, throttle;
    static uint32_t timestamp = 0;

    uint8_t lenght = uart1_available();
    //uint8_t lenght = uart_pio_available();
    if (lenght == ESC_HW4_PACKET_LENGHT || lenght == ESC_HW4_PACKET_LENGHT + 1)
    {
        uint8_t data[ESC_HW4_PACKET_LENGHT];
        uart1_read_bytes(data, ESC_HW4_PACKET_LENGHT);
        //uart_pio_read_bytes(data, ESC_HW4_PACKET_LENGHT);
        throttle = (uint16_t)data[4] << 8 | data[5]; // 0-1024
        pwm = (uint16_t)data[6] << 8 | data[7];      // 0-1024
        float rpm = (uint32_t)data[8] << 16 | (uint16_t)data[9] << 8 | data[10];
        // try to filter invalid data frames
        if (throttle < 1024 &&
            pwm < 1024 &&
            rpm < 200000 &&
            data[11] < 0xF &&
            data[13] < 0xF &&
            data[15] < 0xF &&
            data[17] < 0xF)
        {
            uint16_t current_raw = (uint16_t)data[13] << 8 | data[14];
            float voltage = get_voltage((uint16_t)data[11] << 8 | data[12], parameter);
            float current = 0;
            if (throttle > parameter->current_thresold / 100.0 * 1024)
                current = get_current(current_raw, *current_offset, parameter);
            if (current > parameter->current_max)
                current = 0;
            float temperature_fet = get_temperature((uint16_t)data[15] << 8 | data[16]);
            float temperature_bec = get_temperature((uint16_t)data[17] << 8 | data[18]);
            rpm *= parameter->rpm_multiplier;
            if (parameter->pwm_out)
                xTaskNotifyGive(pwm_out_task_handle);
            *parameter->rpm = get_average(parameter->alpha_rpm, *parameter->rpm, rpm);
            *parameter->consumption += get_consumption(*parameter->current, parameter->current_max, &timestamp);
            *parameter->voltage = get_average(parameter->alpha_voltage, *parameter->voltage, voltage);
            *parameter->current = get_average(parameter->alpha_current, *parameter->current, current);
            *parameter->temperature_fet = get_average(parameter->alpha_temperature, *parameter->temperature_fet, temperature_fet);
            *parameter->temperature_bec = get_average(parameter->alpha_temperature, *parameter->temperature_bec, temperature_bec);
            *parameter->cell_voltage = *parameter->voltage / *parameter->cell_count;
            *parameter->pwm_percentage = get_pwm_percentage(pwm);
            if (debug)
            {
                uint32_t packet = (uint32_t)data[1] << 16 | (uint16_t)data[2] << 8 | data[3];
                printf("\nEsc HW4 (%u) < Packet: %i Rpm: %.0f Volt: %0.2f Curr: %.2f TempFet: %.0f TempBec: %.0f Cons: %.0f CellV: %.2f", uxTaskGetStackHighWaterMark(NULL), packet, *parameter->rpm, *parameter->voltage, *parameter->current, *parameter->temperature_fet, *parameter->temperature_bec, *parameter->consumption, *parameter->cell_voltage);
            }
        }
    }
}

float get_voltage(uint16_t voltage_raw, esc_hw4_parameters_t *parameter)
{
    return ESC_HW4_V_REF * voltage_raw / ESC_HW4_ADC_RES * parameter->divisor;
}

float get_temperature(uint16_t temperature_raw)
{
    float voltage = temperature_raw * ESC_HW4_V_REF / ESC_HW4_ADC_RES;
    float ntcR_Rref = (voltage * ESC_HW4_NTC_R1 / (ESC_HW4_V_REF - voltage)) / ESC_HW4_NTC_R_REF;
    if (ntcR_Rref < 0.001)
        return 0;
    float temperature = 1 / (log(ntcR_Rref) / ESC_HW4_NTC_BETA + 1 / 298.15) - 273.15;
    if (temperature < 0)
        return 0;
    return temperature;
}

float get_current(uint16_t current_raw, float current_offset, esc_hw4_parameters_t *parameter)
{
    float current = current_raw * ESC_HW4_V_REF / (parameter->ampgain * ESC_HW4_DIFFAMP_SHUNT * ESC_HW4_ADC_RES) - current_offset;
    if (current < 0)
        return 0;
    return current;
}

float get_pwm_percentage(uint16_t pwm_raw)
{
    float pwm = (pwm_raw / 1024.0) * 100.0;
    return pwm;
}