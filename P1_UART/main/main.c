#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sdkconfig.h"
#include "myUart.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"

#define MASTER 0
#define SLAVE 1
#define MODE SLAVE

#define TXD_PIN (GPIO_NUM_17)
#define RXD_PIN (GPIO_NUM_16)
#define BAUD_RATE 115200

#define INTERVAL 5000

void app_main()
{
    char secs[20];
    char led_state[2];
    char temperature[20];
    uart_config_t uart_config = {
        .baud_rate = BAUD_RATE,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_APB,
    };
    uart_param_config(UART_NUM_1, &uart_config);
    uart_set_pin(UART_NUM_1, TXD_PIN, RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    uart_driver_install(UART_NUM_1, 1024, 0, 0, NULL, 0);

    uartInit(0, 115200, 8, eStop, eParityEven, UART_TX_PIN0, UART_RX_PIN0); // uart_num, baudrate,  size,  parity, stop,  txPin,  rxPin)

    while (1)
    {
        uart_write_bytes(UART_NUM_1, "10", 2); // envía el comando "ON" por UART
        uart_read_bytes(1, secs, (READ_BUF_SIZE), 20 / portTICK_RATE_MS);
        uartClrScr(0);
        uartGotoxy(0, 5, 5);
        uartPuts(0, secs);
        vTaskDelay(pdMS_TO_TICKS(INTERVAL));

        uartClrScr(0);
        uart_write_bytes(UART_NUM_1, "11", 2); // envía el comando "OFF" por UART
        uart_read_bytes(1, led_state, (READ_BUF_SIZE), 20 / portTICK_RATE_MS);
        uartGotoxy(0, 6, 5);
        uartPuts(0, led_state);
        // uartPuts(0, "Sended: 0x11");
        
        vTaskDelay(pdMS_TO_TICKS(INTERVAL));

        uartClrScr(0);
        uart_write_bytes(UART_NUM_1, "12", 2);
        uart_read_bytes(1, temperature, (READ_BUF_SIZE), 20 / portTICK_RATE_MS);
        uartGotoxy(0, 7, 5);
        uartPuts(0, temperature);
        // uartPuts(0, "Sended: 0x12");
        vTaskDelay(pdMS_TO_TICKS(INTERVAL));

        uart_write_bytes(UART_NUM_1, "13", 2);
        uartClrScr(0);
        uartGotoxy(0, 8, 5);
        uartPuts(0, "0x13: Invertimos LED");
        vTaskDelay(pdMS_TO_TICKS(INTERVAL));
    }
}