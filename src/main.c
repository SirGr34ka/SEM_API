#include "uart_config.h"
#include "uart_io.h"

#include <stdio.h>
#include <unistd.h>

#define SERIAL_PORT_PATH "/dev/ttyUL0"

int file_descriptor;

static void sem_switch_to_idle(void)
{
    recieve_data(file_descriptor);

    send_command(file_descriptor, MOVE_TO_IDLE);

    sleep(1);
    recieve_data(file_descriptor);

    send_command(file_descriptor, DO_QUARY_CONST);

    sleep(1);
    recieve_data(file_descriptor);
}

int main(void)
{
    printf("Starting the sem_api application...\r\n");

    file_descriptor = open_serial_port(SERIAL_PORT_PATH);

    configure_serial_port(file_descriptor);

    sem_switch_to_idle();

    close_serial_port(file_descriptor);

    return 0;
}
