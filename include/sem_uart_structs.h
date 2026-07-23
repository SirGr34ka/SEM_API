#pragma once

#include <stdint.h> // uint*_t

/**
 * @struct sem_uart_cfg
 *
 * @brief
 * UART configuration structure
 */
typedef struct sem_uart_cfg {
  uint32_t
      baudrate;   /**< Baudrate value. Only standart baudrates are supported. */
  uint8_t parity; /**< Parity bit type value. Supported values: 0 for no parity;
                     1 for even parity; 2 for odd parity. */
  uint8_t stop_bit; /**< Stop bit type value. Supported values: 0 for 1 stop
                       bit; 1 for 2 stop bits; */
  uint8_t
      word_size; /**< Word size value. Supported values: 5, 6, 7 and 8 bits. */
} sem_uart_cfg_t;

/**
 * @struct sem_uart
 *
 * @brief
 * UART interaction structure
 */
typedef struct sem_uart {
  int fd;              ///< File descriptor
  sem_uart_cfg_t *cfg; /**< Pointer to UART config. Can be NULL to set default
                          configuration. */
} sem_uart_t;