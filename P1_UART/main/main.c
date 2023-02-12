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

// ACTIVAR COMANDOS CON BOTONES:
#define CMD_0 GPIO_NUM_12
#define CMD_1 GPIO_NUM_14
#define CMD_2 GPIO_NUM_27
#define CMD_3 GPIO_NUM_26

#define LED GPIO_NUM_0

uint32_t get_time_in_seconds()
{
    return xTaskGetTickCount() / configTICK_RATE_HZ;
}

void init_master_inputs()
{
    gpio_reset_pin(CMD_0);
    gpio_reset_pin(CMD_1);
    gpio_reset_pin(CMD_2);
    gpio_reset_pin(CMD_3);

    gpio_set_direction(CMD_0, GPIO_MODE_INPUT);
    gpio_set_direction(CMD_1, GPIO_MODE_INPUT);
    gpio_set_direction(CMD_2, GPIO_MODE_INPUT);
    gpio_set_direction(CMD_3, GPIO_MODE_INPUT);

    gpio_set_pull_mode(CMD_0, GPIO_PULLUP_ONLY);
    gpio_set_pull_mode(CMD_1, GPIO_PULLUP_ONLY);
    gpio_set_pull_mode(CMD_2, GPIO_PULLUP_ONLY);
    gpio_set_pull_mode(CMD_3, GPIO_PULLUP_ONLY);
}

void init_slave_outputs()
{
    gpio_reset_pin(LED);
    gpio_set_direction(LED, GPIO_MODE_OUTPUT);
}

void app_main(void)
{
    uartInit(0, 115200, 8, eParityEven, eStop, 1, 3);
    // uartInit(1, 115200, 8, eParityEven, eStop, 17, 16);
    uart_flush(0);
    // uart_flush(1); // limpiamos buffer de recibir

    while (1)
    {
#if MODE == SLAVE
        uint8_t command[1];
        int len = uart_read_bytes(0, command, 1, portMAX_DELAY);
        char *tag = "info";
        init_slave_outputs();

        if (command[0] == 0x10)
        {
            ESP_LOGI(tag, "Timestamp = %d", get_time_in_seconds());
        }
        else if (command[0] == 0x11)
        {
            ESP_LOGI(tag, "Estado del led = ");
        }
        else if (command[0] == 0x12)
        {
            ESP_LOGI(tag, "Temperatura = 45");
        }
        else if (command[0] == 0x13)
        {
            ESP_LOGI(tag, "Invertimos el LED");
        }

#elif MODE == MASTER
        if (gpio_get_level(CMD_0) == 0)
        {
            uartPutchar(0, 0x10);
        }
        else if (gpio_get_level(CMD_1) == 0)
        {
            uartPutchar(0, 0x11);
        }
        else if (gpio_get_level(CMD_2) == 0)
        {
            uartPutchar(0, 0x12);
        }
        else if (gpio_get_level(CMD_3) == 0)
        {
            uartPutchar(0, 0x13);
        }
#endif
        delayMs(5); // Necesario para el watchdog
    }
}
