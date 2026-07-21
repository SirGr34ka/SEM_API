#include "sem_uart_init.h"

#include <fcntl.h>  // open()
#include <stdlib.h> // exit(), EXIT_FAILURE
#include <stdio.h>  // printf()
#include <unistd.h> // close()
#include <errno.h>  // errno
#include <string.h> // strerror()

/**
 * @brief
 * Opens COM-port device file
 *
 * @param char_dev_path
 * path to COM-port device file
 *
 * @return
 * File descriptor
 */
static int open_com_port(const char *com_port_path)
{
    int fd;

    fd = open(com_port_path, O_RDWR | O_NONBLOCK | O_NOCTTY);

    if (fd < 0) {
        printf("Error %i happend while opening serial port: %s\r\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    return fd;
}

/**
 * @brief
 * Configures COM-port
 *
 * COM-port will be configured with baudrate B115200 and raw mode via termios
 * attributes.
 *
 * @param fd
 * file descriptor for COM-port
 *
 * @param attr_ptr
 * pointer to termios structure with new attributes
 */
static void configure_com_port(int fd, termios_t *attr_ptr)
{
    int setattr_res;

    setattr_res = tcsetattr(fd, TCSANOW, attr_ptr);

    if (setattr_res < 0) {
        printf("Error %i happend while setting new COM-port attributes: %s\r\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    /* To check that all changes have been performed successfully */
    int getattr_res;

    getattr_res = tcgetattr(fd, attr_ptr);

    if (getattr_res < 0) {
        printf("Error %i happend while getting new COM-port attributes after setting: %s\r\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }
}

// void sem_uart_init(const char *port_path, int custom_attr_flag, sem_uart_t *uart)
// {
//     uart->fd = open_serial_port(port_path);

//     if (!custom_attr_flag) {
//         configure_serial_port(uart->fd);

//         printf("Port configured!\r\n");

//         tcgetattr(uart->fd, uart->sem_uart_attr_ptr);

//         printf("Port attr saved!\r\n");
//     } else {
//         if (tcsetattr(uart->fd, TCSANOW, uart->sem_uart_attr_ptr)) {
//             printf("Something went wrong while setting UART port attributes!\r\n");
//             exit(EXIT_FAILURE);
//         }
//     }
// }

void sem_uart_init_default(const char *uart_path, sem_uart_t *uart)
{
    int fd;

    fd = open_com_port(uart_path);
    uart->fd = fd;

    termios_t attr;

    int getattr_res;

    getattr_res = tcgetattr(fd, &attr);

    if (getattr_res < 0) {
        printf("Error %i happend while getting UART attributes before changes: %s\r\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    uart->sem_uart_old_attr = attr;

    cfsetispeed(&attr, B115200);
    cfsetospeed(&attr, B115200);

    // *Fix* Need to check alternative flags in manual
    attr.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
    attr.c_oflag &= ~OPOST;
    attr.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
    attr.c_cflag &= ~(CSIZE | PARENB | CSTOPB);
    attr.c_cflag |= CS8;

    configure_com_port(fd, &attr);

    uart->sem_uart_attr = attr;
}

void sem_uart_init_custom(const char *uart_path, sem_uart_t *uart)
{
    int fd;

    fd = open_com_port(uart_path);
    uart->fd = fd;

    termios_t attr;

    int getattr_res;

    getattr_res = tcgetattr(fd, &attr);

    if (getattr_res < 0) {
        printf("Error %i happend while getting UART attributes before changes: %s\r\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    uart->sem_uart_old_attr = attr;

    configure_com_port(fd, &(uart->sem_uart_attr));
}

void sem_uart_close(sem_uart_t *uart)
{
    int fd;

    fd = uart->fd;

    configure_com_port(fd, &(uart->sem_uart_old_attr)); // Setting of old UART attributes

    int close_res;

    close_res = close(fd);

    if (close_res < 0) {
        printf("Error %i happend while closing UART: %s\r\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }
}
