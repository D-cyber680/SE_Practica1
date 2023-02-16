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

void app_main()
{
    char secs[20];
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
        uartPuts(0, secs);
        uartClrScr(0);
        uartGotoxy(0, 5, 5);
        uartPuts(0, "Sended: 0x10");
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        uart_write_bytes(UART_NUM_1, "11", 2); // envía el comando "OFF" por UART
        uartClrScr(0);
        uartPuts(0, "Sended: 0x11");
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        uart_write_bytes(UART_NUM_1, "12", 2);
        uartClrScr(0);
        uartPuts(0, "Sended: 0x12");
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        uart_write_bytes(UART_NUM_1, "13", 2);
        uartClrScr(0);
        uartPuts(0, "Sended: 0x13");
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}