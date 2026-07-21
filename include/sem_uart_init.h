#pragma once

#include "sem_uart_struct.h" // sem_uart_t

void sem_uart_init_default(const char *uart_path, sem_uart_t *uart);

void sem_uart_init_custom(const char *uart_path, sem_uart_t *uart);

// void sem_uart_init(const char *port_path, int custom_attr, sem_uart_t *uart);
//                                               custom_attr = 0 ->
//                                               sem_uart_init_default()
//                                               custom_attr = 1 ->
//                                               sem_uart_init_custom()

/**
 * @brief
 * Closes a serial port
 *
 * @param fd
 * file descriptor for serial port
 */
void sem_uart_close(sem_uart_t *uart);