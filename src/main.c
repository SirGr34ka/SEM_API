#include "sem_uart_init.h"
#include "sem_uart_io.h"

#include <stdio.h>  // printf()
#include <unistd.h> // sleep()

#define SERIAL_PORT_PATH "/dev/ttyUL0"

int file_descriptor;

static void sem_switch_to_idle(void)
{
    recieve_data(file_descriptor);

    send_command(file_descriptor, MOVE_TO_IDLE, 0, 0, 0);

    sleep(1);
    recieve_data(file_descriptor);

    send_command(file_descriptor, DO_QUARY, 50000, 46, 5);

    sleep(1);
    recieve_data(file_descriptor);

    send_command(file_descriptor, DO_INJECTION, 50000, 46, 5);

    sleep(1);
    recieve_data(file_descriptor);

    send_command(file_descriptor, DO_QUARY, 50000, 46, 5);

    sleep(1);
    recieve_data(file_descriptor);

    send_command(file_descriptor, MOVE_TO_OBSERVATION, 0, 0, 0);

    sleep(1);
    recieve_data(file_descriptor);

    send_command(file_descriptor, MOVE_TO_IDLE, 0, 0, 0);

    sleep(1);
    recieve_data(file_descriptor);

    send_command(file_descriptor, DO_QUARY, 50000, 46, 5);

    sleep(1);
    recieve_data(file_descriptor);
}

int main(void)
{
    printf("Starting the sem_api application...\r\n");

    sem_uart_t uart;

    sem_uart_init_default(SERIAL_PORT_PATH, &uart);

    file_descriptor = uart.fd;

    sem_switch_to_idle();

    sem_uart_close(&uart);

    return 0;
}
