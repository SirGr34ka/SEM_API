#include "sem_uart_init.h"

#include <fcntl.h>  // open()
#include <stdlib.h> // exit(), EXIT_FAILURE
#include <stdio.h>  // printf()
#include <unistd.h> // close()
#include <errno.h>  // errno
#include <string.h> // strerror()
#include <termios.h> // struct termios, flags, cfset?speed()

#define DEFAULT_BAUDRATE 115200
#define DEFAULT_PARITY 0
#define DEFAULT_STOP_BIT 0
#define DEFAULT_WORD_SIZE 8

typedef struct termios termios_t;

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
        printf("Error %i happened while opening serial port: %s\r\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    return fd;
}

static speed_t get_termios_baud_from_uint(uint32_t baudrate)
{
    switch (baudrate) {
        case 0:
            return B0;

        case 50:
            return B50;

        case 75:
            return B75;

        case 110:
            return B110;

        case 134:
            return B134;

        case 150:
            return B150;

        case 200:
            return B200;

        case 300:
            return B300;

        case 600:
            return B600;

        case 1200:
            return B1200;

        case 1800:
            return B1800;

        case 2400:
            return B2400;

        case 4800:
            return B4800;

        case 9600:
            return B9600;

        case 19200:
            return B19200;

        case 38400:
            return B38400;

        case 57600:
            return B57600;

        case 115200:
            return B115200;

        case 230400:
            return B230400;

        case 460800:
            return B460800;

        case 500000:
            return B500000;

        case 576000:
            return B576000;

        case 921600:
            return B921600;

        case 1000000:
            return B1000000;

        case 1152000:
            return B1152000;

        case 1500000:
            return B1500000;

        case 2000000:
            return B2000000;

        case 2500000:
            return B2500000;

        case 3000000:
            return B3000000;

        case 3500000:
            return B3500000;

        case 4000000:
            return B4000000;

        default:
            printf("Input baudrate %d is not supported! Only standart baudrates is supported.\r\n",
                   baudrate);
            exit(EXIT_FAILURE);
    }
}

static tcflag_t get_termios_parity_from_uint(uint8_t parity)
{
    switch (parity) {
        case 0:
            return 0;

        case 1:
            return PARENB;

        case 2:
            return PARENB | PARODD;

        default:
            printf(
                "Input parity value %d is not supported! Only 0 (no parity), 1 (even parity) and 2 "
                "(odd parity) values are supported.\r\n",
                parity);
            exit(EXIT_FAILURE);
    }
}

static tcflag_t get_termios_stop_bit_from_uint(uint8_t stop_bit)
{
    switch (stop_bit) {
        case 0:
            return 0;

        case 1:
            return CSTOPB;

        default:
            printf(
                "Input stop bit value %d is not supported! Only 0 (1 bit) and 1 (2 bits) values "
                "are supported.\r\n",
                stop_bit);
            exit(EXIT_FAILURE);
    }
}

static tcflag_t get_termios_word_size_from_uint(uint8_t word_size)
{
    switch (word_size) {
        case 5:
            return CS5;

        case 6:
            return CS6;

        case 7:
            return CS7;

        case 8:
            return CS8;

        default:
            printf("Input word size %d is not supported! Only 5, 6, 7, 8 values are supported.\r\n",
                   word_size);
            exit(EXIT_FAILURE);
    }
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
        printf("Error %i happened while setting new COM-port attributes: %s\r\n",
               errno,
               strerror(errno));
        exit(EXIT_FAILURE);
    }

    /* To check that all changes have been performed successfully */
    int getattr_res;

    getattr_res = tcgetattr(fd, attr_ptr);

    if (getattr_res < 0) {
        printf("Error %i happened while getting new COM-port attributes after setting: %s\r\n",
               errno,
               strerror(errno));
        exit(EXIT_FAILURE);
    }
}

void sem_uart_init(const char *uart_path, sem_uart_t *uart)
{
    int fd;

    fd = open_com_port(uart_path);
    uart->fd = fd;

    uint32_t baudrate;
    uint8_t parity;
    uint8_t stop_bit;
    uint8_t word_size;
    sem_uart_cfg_t *cfg_ptr;

    cfg_ptr = uart->sem_uart_attr;

    termios_t attr;
    int getattr_res;

    getattr_res = tcgetattr(fd, &attr);

    if (getattr_res < 0) {
        printf("Error %i happened while getting UART attributes before changes: %s\r\n",
               errno,
               strerror(errno));
        exit(EXIT_FAILURE);
    }

    /* flags */
    attr.c_iflag &=
        ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON | IXOFF | IXANY);
    attr.c_oflag &= ~OPOST;
    attr.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
    attr.c_cflag &= ~(CSIZE | PARENB | CSTOPB);
    attr.c_cflag |= CREAD | CLOCAL;

    if (cfg_ptr == NULL) {
        printf("Default UART config will be set: ");

        baudrate = DEFAULT_BAUDRATE;
        parity = DEFAULT_PARITY;
        stop_bit = DEFAULT_STOP_BIT;
        word_size = DEFAULT_WORD_SIZE;
    } else {
        printf("Custom UART config will be set: ");

        baudrate = cfg_ptr->baudrate;
        parity = cfg_ptr->parity;
        stop_bit = cfg_ptr->stop_bit;
        word_size = cfg_ptr->word_size;
    }

    /* baudrate */
    cfsetispeed(&attr, get_termios_baud_from_uint(baudrate));
    cfsetospeed(&attr, get_termios_baud_from_uint(baudrate));

    printf("baudrate = %d, ", baudrate);

    /* parity */
    attr.c_cflag |= get_termios_parity_from_uint(parity);

    printf("parity = %d, ", parity);

    /* stop bit */
    attr.c_cflag |= get_termios_stop_bit_from_uint(stop_bit);

    printf("stop bit = %d, ", stop_bit);

    /* word size */
    attr.c_cflag |= get_termios_word_size_from_uint(word_size);

    printf("word size = %d.\r\n", word_size);

    configure_com_port(fd, &attr);
}

void sem_uart_close(sem_uart_t *uart)
{
    int fd;

    fd = uart->fd;

    int close_res;

    close_res = close(fd);

    if (close_res < 0) {
        printf("Error %i happened while closing UART: %s\r\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }
}
