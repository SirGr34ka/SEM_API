#pragma once

#include "sem_uart_structs.h" // sem_uart_t

/**
 * @brief
 * Initializates UART for SEM
 *
 * @details
 * This function opens UART device file and configures it with input
 * configuration, after which input/output logic can be used with @ref
 * sem_uart_send and @ref sem_uart_recieve functions.
 * If cfg_ptr in sem_uart struct set as NULL then default configuration will be
 * set: baudrate = 115200, parity bit = 0, stop bit = 0, word size = 8.
 *
 * @param uart_path
 * path to UART device file
 *
 * @param uart
 * UART interaction structure pointer
 */
void sem_uart_init(const char *uart_path, sem_uart_t *uart);

/**
 * @brief
 * Closes a serial port
 *
 * @param uart
 * UART interaction structure pointer
 */
void sem_uart_close(sem_uart_t *uart);