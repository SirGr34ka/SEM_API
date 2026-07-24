#pragma once

#include "sem_uart_structs.h" // sem_uart_t

#include <stddef.h> // size_t

/**
 * @typedef sem_addr_t
 * @struct sem_addr
 *
 * @brief
 * SEM addresses
 *
 * @details
 * Addresses which needed to concatinate into full LFA format address.
 */
typedef struct sem_addr {
  uint32_t lfa; ///< Linear frame address
  uint16_t wa;  ///< Word address
  uint16_t ba;  ///< Bit address
} sem_addr_t;

typedef int (*sem_uart_handler_t)(const char *buffer, size_t size);

/**
 * @typedef sem_uart_cmd_t
 *
 * @brief
 * UART command type
 */
typedef uint8_t sem_uart_cmd_t;

/**
 * @name UART commands
 *
 * @brief
 * Commands which can be sent to the SEM controller via UART
 */
///@{
#define MOVE_TO_IDLE 00 ///< Move to IDLE state command
#define DO_QUARY 01     ///< Run QUARY command with random argument
#define DO_INJECTION 02 ///< Run QUARY command with user defined argument

#define MOVE_TO_OBSERVATION 10 ///< Move to OBSERVATION state command

#define MOVE_TO_DETECT_ONLY 20 ///< Move to DETECT ONLY state command

#define DO_STATUS 31 ///< Run STATUS command

#define MOVE_TO_DIAGNOSTIC_SCAN 40 ///< Move to DIAGNOSTIC SCAN state command
///@}

/**
 * @brief
 * Sends a command to SEM controller via UART
 *
 * @param uart
 * UART interaction structure pointer
 *
 * @param command_num
 * UART command @ref "UART commands"
 *
 * @param addr
 * SEM addresses structure pointer
 */
void sem_uart_send(const sem_uart_t *uart, sem_uart_cmd_t cmd,
                   const sem_addr_t *addr);

/**
 * @brief
 * Recieves data from SEM controller via UART
 *
 * @param uart
 * UART interaction structure pointer
 *
 * @param buffer
 * char buffer where to write recieved data
 *
 * @param size
 * size of buffer
 */
void sem_uart_recieve(const sem_uart_t *uart, char *buffer, size_t size,
                      sem_uart_handler_t handler);