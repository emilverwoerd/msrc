add_executable(${PROJECT_NAME}
        main.c
        uart.c
        common.c
        led.c
        esc_hw5.c
        config.c
        cell_count.c
        auto_offset.c
        sbus.c       
        usb.c
        ws2812.c
)

pico_generate_pio_header(${PROJECT_NAME} ${CMAKE_CURRENT_LIST_DIR}/ws2812.pio)

target_link_libraries(${PROJECT_NAME}
        pico_stdlib
        freertos
        hardware_uart
        hardware_irq
        hardware_pwm
        hardware_clocks
        hardware_adc
        hardware_pio
        hardware_i2c
        hardware_flash
)

pico_add_extra_outputs(${PROJECT_NAME})

pico_enable_stdio_usb(${PROJECT_NAME} 1) # 1 normal, 0 probe debug 
pico_enable_stdio_uart(${PROJECT_NAME} 0) # 0 normal, 1 probe debug
