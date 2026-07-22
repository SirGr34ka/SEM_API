#pragma once

#include "sem_uart_structs.h" // sem_uart_t

void sem_uart_init(const char *uart_path, sem_uart_t *uart);

/**
 * @brief
 * Closes a serial port
 *
 * @param fd
 * file descriptor for serial port
 */
void sem_uart_close(sem_uart_t *uart);