#pragma once

#include <termios.h>

typedef struct sem_uart {
  int fd;
  struct termios *sem_uart_attr_ptr;
} sem_uart_t;

void sem_uart_init(const char *port_path, int custom_attr_flag,
                   sem_uart_t *uart);

/**
 * @brief
 * Closes a serial port
 *
 * @param fd
 * file descriptor for serial port
 */
void close_serial_port(const int fd);