#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

#include "uart_io.h"

void check_fd(int fd)
{
    const int flags = fcntl(fd, F_GETFD);
    const int access_flag = O_ACCMODE & flags;

    if ((flags < 0) || (access_flag != O_RDWR)) {
        printf("Could not open the file! File descriptor was: %d.\r\n", fd);
        exit(EXIT_FAILURE);
    }
}

void send_command(int fd, char *command, size_t size)
{
    check_fd(fd);
}

int recieve_data(int fd)
{
    check_fd(fd);
}
