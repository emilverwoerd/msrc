#ifndef CONSTANTS_H
#define CONSTANTS_H

/* Version */
#define VERSION_MAJOR 0
#define VERSION_MINOR 11
#define VERSION_PATCH 0

/* Pins */
#define UART0_TX_GPIO 0       // receiver rx
#define UART0_RX_GPIO 1       // receiver tx
#define UART1_TX_GPIO 4       // not used
#define UART1_RX_GPIO 5       // esc tx

/* UARTS */
#define UART_RECEIVER uart0
#define UART_RECEIVER_RX UART0_RX_GPIO
#define UART_RECEIVER_TX UART0_TX_GPIO
#define UART_ESC uart1
#define UART_ESC_RX UART1_RX_GPIO
#define UART_ESC_TX UART1_TX_GPIO

// set receiver to uart1 when debugging with probe
//#define UART_RECEIVER uart1
//#define UART_RECEIVER_RX UART1_RX_GPIO
//#define UART_RECEIVER_TX UART1_TX_GPIO

/* Stack */
#define STACK_EXTRA 100

/* RECEIVERS */
#define STACK_RX_SBUS (220 + STACK_EXTRA)


#define STACK_ESC_HW5 (224 + STACK_EXTRA)
// #define STACK_ESC_HW4 (224 + STACK_EXTRA)
#define STACK_CELL_COUNT (180 + STACK_EXTRA)
#define STACK_AUTO_OFFSET (140 + STACK_EXTRA)

#define STACK_USB (180 + STACK_EXTRA)
#define STACK_LED (186 + STACK_EXTRA)

/* RPM multiplier */
// #define RPM_MULTIPLIER (RPM_PINION_TEETH / (1.0 * RPM_MAIN_TEETH * RPM_PAIR_OF_POLES))

#endif
