#include "uart_config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>

#define SERIAL_PORT_PATH "/dev/ttyUL0"
#define COMMAND_SIZE 16
#define BUFFER_SIZE 1024

int file_descriptor;

static void sem_switch_to_idle(void)
{
    char command[COMMAND_SIZE];
    char buffer[BUFFER_SIZE];

    //--------------------------------------------------------------
    memset(buffer, 0, BUFFER_SIZE);

    read(file_descriptor, (uint8_t *)buffer, BUFFER_SIZE);

    for (size_t i = 0; i < BUFFER_SIZE; ++i) {
        printf("%c", buffer[i]);

        if (buffer[i] == '\r') {
            printf("\n");
        }
    }

    printf("\r\n");

    //--------------------------------------------------------------

    memset(command, 0, COMMAND_SIZE);

    strcpy(command, "I");

    write(file_descriptor, (uint8_t *)command, COMMAND_SIZE);

    printf("\r\nCommand '%s' sent\r\n", command);

    //--------------------------------------------------------------

    sleep(1);

    memset(buffer, 0, BUFFER_SIZE);

    read(file_descriptor, (uint8_t *)buffer, BUFFER_SIZE);

    for (size_t i = 0; i < BUFFER_SIZE; ++i) {
        printf("%c", buffer[i]);

        if (buffer[i] == '\r') {
            printf("\n");
        }
    }

    printf("\r\n");

    //--------------------------------------------------------------

    memset(command, 0, COMMAND_SIZE);

    strcpy(command, "Q C000C350000"); // LFA: 50000

    write(file_descriptor, (uint8_t *)command, COMMAND_SIZE);

    printf("\r\nCommand '%s' sent\r\n", command);

    //--------------------------------------------------------------

    sleep(1);

    memset(buffer, 0, BUFFER_SIZE);

    read(file_descriptor, (uint8_t *)buffer, BUFFER_SIZE);

    for (size_t i = 0; i < BUFFER_SIZE; ++i) {
        printf("%c", buffer[i]);

        if (buffer[i] == '\r') {
            printf("\n");
        }
    }

    printf("\r\n");
}

int main(void)
{
    printf("Starting the sem_init application...\r\n");

    file_descriptor = open_serial_port(SERIAL_PORT_PATH);

    configure_serial_port(file_descriptor);

    sem_switch_to_idle();

    close_serial_port(file_descriptor);

    return 0;
}
