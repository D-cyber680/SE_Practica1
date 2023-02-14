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
    uint8_t led_state = 0;
    gpio_reset_pin(LED);
    gpio_set_direction(LED, GPIO_MODE_OUTPUT);
    uartInit1();
    uartInit(0, BAUD_RATE, 8, eParityDis, eStop, UART_TX_PIN0, UART_RX_PIN0); // uart_num, baudrate,  size,  parity, stop,  txPin,  rxPin)

    uint8_t command[3];
    while (1)
    {
        int len = uart_read_bytes(UART_NUM_1, command, 3, pdMS_TO_TICKS(100));
        if (len == 2 && command[0] == '1' && command[1] == '0')
        {
            gpio_set_level(LED, 1);
            uartClrScr(0);
            uartPuts(0, "Comando: 0x10");
        }
        else if (len == 2 && command[0] == '1' && command[1] == '1')
        {
            gpio_set_level(LED, 0); // apaga el LED
            uartClrScr(0);
            uartPuts(0, "Comando: 0x11");
        }
        else if (len == 2 && command[0] == '1' && command[1] == '2')
        {
            uartClrScr(0);
            uartPuts(0, "Comando: 0x12");
        }
        else if (len == 2 && command[0] == '1' && command[1] == '3')
        {
            uartClrScr(0);
            uartPuts(0, "Comando: 0x13");
        }
        else
        {
            uartClrScr(0);
            uartPuts(0, "Ese comando no existe");
        }
    }
}
