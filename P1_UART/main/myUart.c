#include "myUart.h"

void uartInit(uart_port_t uart_num, uint32_t baudrate, uint8_t size, uint8_t parity, uint8_t stop, uint8_t txPin, uint8_t rxPin)
{
    /* Configure parameters of an UART driver,
     * communication pins and install the driver */
    uart_config_t uart_config = {
        .baud_rate = baudrate,
        .data_bits = size - 5,
        .parity = eParityOdd ? UART_PARITY_ODD : eParityEven ? UART_PARITY_EVEN
                                                             : UART_PARITY_DISABLE,
        .stop_bits = eStop2 ? UART_STOP_BITS_2 : UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_APB,
    };

    ESP_ERROR_CHECK(uart_driver_install(uart_num, READ_BUF_SIZE, READ_BUF_SIZE, 0, NULL, 0));
    ESP_ERROR_CHECK(uart_param_config(uart_num, &uart_config));
    ESP_ERROR_CHECK(uart_set_pin(uart_num, txPin, rxPin,
                                 UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
}

void delayMs(uint16_t ms)
{
    vTaskDelay(ms / portTICK_PERIOD_MS);
}

void uartClrScr(uart_port_t uart_num)
{
    // uartPuts(uart_num, "\e[2J"); // para limpiar la plantalla
    // uartPuts(uart_num, "\e[H");  // hace que el cursor vuelva al inicio
    //  Uso "const" para sugerir que el contenido del arreglo lo coloque en Flash y no en RAM
    const char caClearScr[] = "\e[2J";
    const char regresarInicio[] = "\e[H";
    uart_write_bytes(uart_num, caClearScr, sizeof(caClearScr));
    uart_write_bytes(uart_num, regresarInicio, sizeof(regresarInicio));
}
void uartGoto11(uart_port_t uart_num)
{
    // Limpie un poco el arreglo de caracteres, los siguientes tres son equivalentes:
    // "\e[1;1H" == "\x1B[1;1H" == {27,'[','1',';','1','H'}
    const char caGoto11[] = "\e[1;1H";
    uart_write_bytes(uart_num, caGoto11, sizeof(caGoto11));
}

bool uartKbhit(uart_port_t uart_num)
{
    uint8_t length;
    uart_get_buffered_data_len(uart_num, (size_t *)&length);
    return (length > 0);
}

void uartPutchar(uart_port_t uart_num, char c)
{
    uart_write_bytes(uart_num, &c, sizeof(c));
}

void uartPuts(uart_port_t uart_num, char *str)
{
    while (*str)
    {
        uartPutchar(uart_num, *(str++));
        //*(str++);
    }
}

char uartGetchar(uart_port_t uart_num)
{
    char c;
    // Wait for a received byte
    while (!uartKbhit(uart_num))
    {
        delayMs(10);
    }
    // read byte, no wait
    uart_read_bytes(UART_NUM0, &c, sizeof(c), 0);

    return c;
}

void uartGets(uart_port_t uart_num, char *str)
{
    uint8_t count = 0, flag = 0, count_no_num = 0;
    char c;
    while (1)
    {
        c = uartGetchar(uart_num);
        if (c != 13) // Si no se le pone enter
        {
            if (c == 8)
            {                   // Si se da un backspace
                if (count != 0) // si no es cero el cont
                {
                    count--;
                    str[count] = '\0';
                    uartPuts(uart_num, "\e[1D"); // se mueve un caracter hacia atras \e[1D
                    uartPuts(uart_num, "\e[0K"); // Borra la linea del cursor hasta el final
                }
            }
            else
            {
                uartPutchar(uart_num, c);
                if (flag != 1)
                {
                    str[count] = c;
                    count++;
                }
            }
        }
        else
        {
            str[count] = '\0';
            break;
        }
    }
}

void uartSetColor(uart_port_t uart_num, uint8_t color)
{
    char y[] = "\033[0;1;33m", g[] = "\033[0;0;32m", b[] = "\033[0;0;34m"; //\x1B[38;5;{33}m	0;49;93
    char cadena[15];
    switch (color)
    {
    case YELLOW:
        memcpy(cadena, y, strlen(y) + 1); // str="\[\033[1;33m\]";
        break;
    case GREEN:
        memcpy(cadena, g, strlen(g) + 1);
        break;
    case BLUE:
        memcpy(cadena, b, strlen(b) + 1);
        break;
    default:
        break;
    }
    uartPuts(uart_num, cadena);
}

void uartGotoxy(uart_port_t uart_num, uint8_t x, uint8_t y)
{
    char cad[20];
    sprintf(cad, "\e[%d;%dH", x, y);
    uartPuts(uart_num, cad);
}

void myItoa(uint16_t number, char *str, uint8_t base)
{
    int n = 0, i = 0, length = 0;
    char temp;
    while (number != 0)
    {
        n = n * 10;
        n = number % base;
        str[i] = n >= 10 && n <= 15 ? n + 55 : n + 48;
        number /= base;
        i++;
    }
    str[i] = '\0';
    length = i;
    for (i = 0; i < length / 2; i++)
    {
        temp = str[i];
        str[i] = str[length - i - 1];
        str[length - i - 1] = temp;
    }
}

uint16_t myAtoi(char *str)
{ // convertir una cadena en un entero
    uint16_t num = 0, num_from_str = 0;

    while (*str)
    { // mientras no se termine la cadena
        // num=num*10; //si la cadena no se ha terminado, multiplicar por 10
        num_from_str = *str - '0'; // le restamos '0' para tener el valor crudo, no ascii
        if (num_from_str >= 0 && num_from_str <= 9)
        {
            num += num_from_str;
            str++;
            if (*str >= '0' && *str <= '9')
            {
                num = num * 10;
            }
        }
        else
            break;
    }
    return num;
}