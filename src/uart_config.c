#include "uart_config.h"

#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

/**
 * @brief
 * Opens a serial port
 *
 * @param port_path
 * path to serial device file
 *
 * @return
 * File descriptor int number
 */
static int open_serial_port(const char *port_path)
{
    int fd;

    fd = open(port_path, O_RDWR | O_NONBLOCK);

    if (fd < 0) {
        printf("Could not open the file '%s'! File descriptor was: %d.\r\n", port_path, fd);
        exit(EXIT_FAILURE);
    }

    return fd;
}

/**
 * @brief
 * Configures a serial port
 *
 * Serial port will be configured with baudrate B115200 and raw mode via termios
 * attributes.
 *
 * @param fd
 * file descriptor for serial port
 */
static void configure_serial_port(const int fd)
{
    struct termios tty_attributes;

    if (tcgetattr(fd, &tty_attributes)) {
        printf("Something went wrong while getting port attributes!\r\n");
        exit(EXIT_FAILURE);
    }

    cfsetispeed(&tty_attributes, B115200);
    cfsetospeed(&tty_attributes, B115200);

    // *Fix* Need to check alternative flags in manual
    tty_attributes.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
    tty_attributes.c_oflag &= ~OPOST;
    tty_attributes.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
    tty_attributes.c_cflag &= ~(CSIZE | PARENB | CSTOPB);
    tty_attributes.c_cflag |= CS8;

    if (tcsetattr(fd, TCSANOW, &tty_attributes)) {
        printf("Something went wrong while setting port attributes!\r\n");
        exit(EXIT_FAILURE);
    }
}

void sem_uart_init(const char *port_path, int custom_attr_flag, sem_uart_t *uart)
{
    uart->fd = open_serial_port(port_path);

    if (!custom_attr_flag) {
        configure_serial_port(uart->fd);
        tcgetattr(uart->fd, uart->sem_uart_attr_ptr);
    } else {
        if (tcsetattr(uart->fd, TCSANOW, uart->sem_uart_attr_ptr)) {
            printf("Something went wrong while setting UART port attributes!\r\n");
            exit(EXIT_FAILURE);
        }
    }
}

void close_serial_port(const int fd)
{
    if (close(fd) < 0) {
        printf("Could not close the file! File descriptor was: %d\r\n", fd);
        exit(EXIT_FAILURE);
    }
}
