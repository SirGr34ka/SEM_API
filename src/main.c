#include "sem_uart_init.h" // sem_uart_init(), sem_uart_close()
#include "sem_test.h"      // sem_test_inj_and_correct()

#include <stdio.h>  // printf()

#define SERIAL_PORT_PATH "/dev/ttyUL0"

sem_uart_t uart;

int main(void)
{
    printf("Starting the sem_api application...\r\n");

    sem_uart_cfg_t cfg;

    cfg.baudrate = 50;
    cfg.parity = 0;
    cfg.stop_bit = 1;
    cfg.word_size = 5;

    uart.cfg = &cfg;

    sem_uart_init(SERIAL_PORT_PATH, &uart);
    sem_test_inj_and_correct(&uart, NULL);
    sem_uart_close(&uart);

    return 0;
}
