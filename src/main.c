#include "sem_uart_structs.h" // sem_uart_t
#include "sem_uart_init.h"    // sem_uart_init(), sem_uart_close()
#include "sem_uart_io.h"      // sem_uart_send(), sem_uart_recieve()

#include <stdio.h>  // printf()
#include <unistd.h> // sleep()

#define SERIAL_PORT_PATH "/dev/ttyUL0"

sem_uart_t uart;

static void sem_injection_scenario(void)
{
    sem_uart_recieve(&uart);

    sem_uart_send(&uart, MOVE_TO_IDLE, NULL);

    sleep(1);
    sem_uart_recieve(&uart);

    sem_addr_t addr;

    addr.lfa = 50000;
    addr.wa = 46;
    addr.ba = 5;

    sem_uart_send(&uart, DO_QUARY, &addr);

    sleep(1);
    sem_uart_recieve(&uart);

    sem_uart_send(&uart, DO_INJECTION, &addr);

    sleep(1);
    sem_uart_recieve(&uart);

    sem_uart_send(&uart, DO_QUARY, &addr);

    sleep(1);
    sem_uart_recieve(&uart);

    sem_uart_send(&uart, MOVE_TO_OBSERVATION, NULL);

    sleep(1);
    sem_uart_recieve(&uart);

    sem_uart_send(&uart, MOVE_TO_IDLE, NULL);

    sleep(1);
    sem_uart_recieve(&uart);

    sem_uart_send(&uart, DO_QUARY, &addr);

    sleep(1);
    sem_uart_recieve(&uart);
}

int main(void)
{
    printf("Starting the sem_api application...\r\n");

    uart.sem_uart_attr = NULL;

    sem_uart_init(SERIAL_PORT_PATH, &uart);
    sem_injection_scenario();
    sem_uart_close(&uart);

    return 0;
}
