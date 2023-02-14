#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sdkconfig.h"
#include "myUart.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"

#define MASTER 2
#define SLAVE 1
#define MODE SLAVE

#define TXD_PIN (GPIO_NUM_17)
#define RXD_PIN (GPIO_NUM_16)
#define LED (GPIO_NUM_2)

#define BAUD_RATE 115200

void app_main()
{
    gpio_reset_pin(LED);
    gpio_set_direction(LED, GPIO_MODE_OUTPUT);
    uartInit1();
    uartInit(0, BAUD_RATE, 8, eParityDis, eStop, UART_TX_PIN0, UART_RX_PIN0); // uart_num, baudrate,  size,  parity, stop,  txPin,  rxPin)

    uint8_t rx_buffer[3];
    while (1)
    {
        int len = uart_read_bytes(UART_NUM_1, rx_buffer, 3, pdMS_TO_TICKS(100));
        if (len == 2 && rx_buffer[0] == 'O' && rx_buffer[1] == 'N')
        {
            gpio_set_level(LED, 1);
            uartClrScr(0);
            uartPuts(0, "Prendido");
        }
        else if (len == 3 && rx_buffer[0] == 'O' && rx_buffer[1] == 'F' && rx_buffer[2] == 'F')
        {
            gpio_set_level(LED, 0); // apaga el LED
            uartClrScr(0);
            uartPuts(0, "Apagado");
        }
    }
}
