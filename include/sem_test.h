#pragma once

#include "sem_uart_io.h" // sem_addr_t, sem_uart_send(), sem_uart_recieve()

void sem_test_inj_and_correct(sem_uart_t *uart, sem_addr_t *addr);

void sem_test_status(sem_uart_t *uart);