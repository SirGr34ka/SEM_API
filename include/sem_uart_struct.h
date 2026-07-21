#pragma once

#include <termios.h>

typedef struct termios termios_t;

typedef struct sem_uart {
  int fd;
  termios_t sem_uart_attr;
  termios_t sem_uart_old_attr;
} sem_uart_t;