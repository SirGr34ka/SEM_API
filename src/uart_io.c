#include "uart_io.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
// #include <stdarg.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>

void check_fd(const int fd)
{
    const int flags = fcntl(fd, F_GETFL);
    const int access_flag = O_ACCMODE & flags;

    if ((flags < 0) || (access_flag != O_RDWR)) {
        printf("Could not open the file! File descriptor was: %d.\r\n", fd);
        exit(EXIT_FAILURE);
    }
}

// void send_command(const int fd, const int command, ...)
void send_command(const int fd, const int command_num)
{
    check_fd(fd);

    const size_t COMMAND_SIZE = 16;
    char command[COMMAND_SIZE];

    memset(command, 0, COMMAND_SIZE);

    switch (command_num) {
        /* States */
        case MOVE_TO_IDLE:
            strcpy(command, "I");
            break;

        case MOVE_TO_OBSERVATION:
            strcpy(command, "O");
            break;

        case MOVE_TO_DETECT_ONLY:
            strcpy(command, "D");
            break;

        case MOVE_TO_DIAGNOSTIC_SCAN:
            strcpy(command, "U");
            break;

        /* Commands */
        case DO_QUARY_CONST:
            strcpy(command, "Q C000C350000"); // LFA: 50000
            break;

        default:
            strcpy(command, "I");
            break;
    }

    if (write(fd, (uint8_t *)command, COMMAND_SIZE) < 0) {
        printf("Could not write to file! File descriptor was: %d.\r\n", fd);
        exit(EXIT_FAILURE);
    }

    printf("\r\nCommand '%s' sent\r\n", command);
}

void recieve_data(const int fd)
{
    check_fd(fd);

    const size_t BUFFER_SIZE = 1024;
    char buffer[BUFFER_SIZE];

    memset(buffer, 0, BUFFER_SIZE);

    read(fd, (uint8_t *)buffer, BUFFER_SIZE);

    for (size_t i = 0; i < BUFFER_SIZE; ++i) {
        printf("%c", buffer[i]);

        if (buffer[i] == '\r') {
            printf("\n");
        }
    }

    printf("\r\n");
}
