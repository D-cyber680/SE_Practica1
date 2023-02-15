#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sdkconfig.h"
#include "myUart.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "esp_log.h"

#define MASTER 2
#define SLAVE 1
#define MODE SLAVE

#define TXD_PIN (GPIO_NUM_17)
#define RXD_PIN (GPIO_NUM_16)
#define LED (GPIO_NUM_2)

#define BAUD_RATE 115200

// F comando 0x10
uint32_t get_time_in_seconds()
{
    // enviar devuelta
    return xTaskGetTickCount() / configTICK_RATE_HZ;
}

// F. cmd. 0x11
uint8_t send_led_state(uint8_t *led_state)
{
    //enviar devuelta
    return *led_state;
}
//F. comando 0x12
void send_temp(void){
    int num = rand() % 100;
    char cad[4];
    myItoa(num, cad, 10);
    //UartPuts(2, cad);
}

// F comando 0x13
void toggle_led_state(uint8_t *led_state)
{
    *led_state == 1 ? (*led_state = 0) : (*led_state = 1);
    gpio_set_level(LED, *led_state);
}

void app_main()
{
    char feedback[50];
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
            sprintf(feedback, "(Timestamp) Han transcurrido %d segundos", get_time_in_seconds());
            uartClrScr(0);
            uartPuts(0, feedback);
            uart_write_bytes(UART_NUM_1, feedback, strlen(feedback));
        }
        else if (len == 2 && command[0] == '1' && command[1] == '1')
        {
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
            toggle_led_state(&led_state);
            uartClrScr(0);
            uartPuts(0, "Comando: 0x13");
            uartClrScr(0);
        }
        else
        {
            uartClrScr(0);
            // uartPuts(0, "Ese comando no existe");
        }
    }
}
