#include "sem_uart_structs.h" // sem_uart_t
#include "sem_uart_init.h"    // sem_uart_init(), sem_uart_close()
#include "sem_uart_io.h"      // sem_uart_send(), sem_uart_recieve()

#include <stdio.h>  // printf()
#include <unistd.h> // sleep()

#define SERIAL_PORT_PATH "/dev/ttyUL0"

sem_uart_t uart;

void print_recieved_data(const char *buffer, size_t size)
{
    for (size_t i = 0; i < size; ++i) {
        printf("%c", buffer[i]);

        if (buffer[i] == '\r') {
            printf("\n");
        }
    }

    printf("\r\n");
}

static void sem_injection_scenario(void)
{
    const size_t SIZE = 1024;
    char buffer[SIZE];

    sem_uart_recieve(&uart, buffer, SIZE);
    print_recieved_data(buffer, SIZE);

    sem_uart_send(&uart, MOVE_TO_IDLE, NULL);

    sleep(1);
    sem_uart_recieve(&uart, buffer, SIZE);
    print_recieved_data(buffer, SIZE);

    sem_addr_t addr;

    addr.lfa = 50000;
    addr.wa = 46;
    addr.ba = 5;

    sem_uart_send(&uart, DO_QUARY, &addr);

    sleep(1);
    sem_uart_recieve(&uart, buffer, SIZE);
    print_recieved_data(buffer, SIZE);

    sem_uart_send(&uart, DO_INJECTION, &addr);

    sleep(1);
    sem_uart_recieve(&uart, buffer, SIZE);
    print_recieved_data(buffer, SIZE);

    sem_uart_send(&uart, DO_QUARY, &addr);

    sleep(1);
    sem_uart_recieve(&uart, buffer, SIZE);
    print_recieved_data(buffer, SIZE);

    sem_uart_send(&uart, MOVE_TO_OBSERVATION, NULL);

    sleep(1);
    sem_uart_recieve(&uart, buffer, SIZE);
    print_recieved_data(buffer, SIZE);

    sem_uart_send(&uart, MOVE_TO_IDLE, NULL);

    sleep(1);
    sem_uart_recieve(&uart, buffer, SIZE);
    print_recieved_data(buffer, SIZE);

    sem_uart_send(&uart, DO_QUARY, &addr);

    sleep(1);
    sem_uart_recieve(&uart, buffer, SIZE);
    print_recieved_data(buffer, SIZE);
}

int main(void)
{
    printf("Starting the sem_api application...\r\n");

    sem_uart_cfg_t cfg;

    cfg.baudrate = 9600;
    cfg.parity = 2;
    cfg.stop_bit = 1;
    cfg.word_size = 6;

    uart.cfg = &cfg;

    sem_uart_init(SERIAL_PORT_PATH, &uart);
    sem_injection_scenario();
    sem_uart_close(&uart);

    return 0;
}
