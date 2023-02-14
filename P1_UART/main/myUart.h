#ifndef _MY_UART_H_
#define _MY_UART_H_
 
#include <inttypes.h>
#include <string.h>

#include "driver/uart.h"

#define UART_NUM0 (0)
#define UART_RX_PIN0 (3)
#define UART_TX_PIN0 (1)

#define UART_NUM1 (1)
#define UART_RX_PIN1 (18) //(9)
#define UART_TX_PIN1 (19) //(10)

#define UART_NUM2 (2)
#define UART_RX_PIN2 (16)
#define UART_TX_PIN2 (17)

#define UARTS_BAUD_RATE (115200)
#define TASK_STACK_SIZE (1048)
#define READ_BUF_SIZE (1024)

void uartInit(uart_port_t uart_num, uint32_t baudrate, uint8_t size, uint8_t parity, uint8_t stop, uint8_t txPin, uint8_t rxPin);
void uartInit1();
void delayMs(uint16_t ms);
void uartClrScr(uart_port_t uart_num);
void uartGoto11(uart_port_t uart_num);
bool uartKbhit(uart_port_t uart_num);
void uartPutchar(uart_port_t uart_num, char c);
void uartPuts(uart_port_t uart_num, char *str);
char uartGetchar(uart_port_t uart_num);
void uartGets(uart_port_t uart_num, char *str);
void uartSetColor(uart_port_t uart_num, uint8_t color);
void uartGotoxy(uart_port_t uart_num, uint8_t x, uint8_t y);
void myItoa(uint16_t number, char *str, uint8_t base);
uint16_t myAtoi(char *str);

#define YELLOW 0 // Fixme
#define GREEN 1  // Fixme
#define BLUE 2   // Fixme

enum
{
    eParityDis = 0,
    eParityOdd = 1,
    eParityEven = 2
};

enum
{
    eStop = 1,
    eStop2 = 3
};


#endif
