#include "uart_config.h"
#include "uart_io.h"

#include <stdio.h>
#include <unistd.h>

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

    sem_uart_init(SERIAL_PORT_PATH, 0, &uart);

    file_descriptor = uart.fd;

    sem_switch_to_idle();

    close_serial_port(file_descriptor);

    return 0;
}
