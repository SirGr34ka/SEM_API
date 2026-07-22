#pragma once

#include <stdint.h> // uint*_t

typedef struct sem_uart_cfg {
  uint32_t baudrate;
  uint8_t parity;
  uint8_t stop_bit;
  uint8_t word_size;
} sem_uart_cfg_t;

typedef struct sem_uart {
  int fd;
  sem_uart_cfg_t *cfg;
} sem_uart_t;