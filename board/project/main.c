#include "main.h"

TaskHandle_t pwm_out_task_handle, uart0_notify_task_handle = NULL, uart1_notify_task_handle = NULL, uart_pio_notify_task_handle = NULL, receiver_task_handle = NULL, led_task_handle = NULL, usb_task_handle = NULL;
QueueHandle_t uart0_queue_handle = NULL, uart1_queue_handle = NULL, uart_pio_queue_handle = NULL, tasks_queue_handle = NULL, sensors_queue_handle = NULL;
alarm_pool_t *uart_alarm_pool = NULL;
uint8_t debug;

void vApplicationStackOverflowHook( TaskHandle_t xTask, char * pcTaskName )
{
	printf("stack overflow %x %s\r\n", xTask, (portCHAR *)pcTaskName);
	
	for (;;) {
	}
}

int main()
{
    stdio_init_all();

    sleep_ms(1000);

    if (CONFIG_FORZE_WRITE)
    {
        config_forze_write();
    }
    config_t *config = config_read();
    debug = config->debug;
    if (debug)
        printf("\n\nMSRC init");

    tasks_queue_handle = xQueueCreate(64, sizeof(QueueHandle_t));

    led_cycle_duration = 1000;
    led_cycles = 10;
    led_color = GREEN;

    xTaskCreate(led_task, "led_task", STACK_LED, NULL, 1, &led_task_handle);
    xTaskCreate(usb_task, "usb_task", STACK_USB, NULL, 1, &usb_task_handle);

  
    /* Create SBUS task only receiver protocol left */
    xTaskCreate(sbus_task, "sbus_task", STACK_RX_SBUS, NULL, 3, &receiver_task_handle);
    uart0_notify_task_handle = receiver_task_handle;
    xQueueSendToBack(tasks_queue_handle, receiver_task_handle, 0);
  

    vTaskStartScheduler();

    vTaskResume(led_task_handle);
    while (1)
    {
    };
}
