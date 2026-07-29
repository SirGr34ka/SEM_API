#pragma once

#include "sem_uart_io.h" // sem_addr_t, sem_uart_send(), sem_uart_recieve()

/**
 * @brief
 * Injection & Correction test scenario
 *
 * @param uart
 * UART interaction structure pointer
 *
 * @param addr
 * SEM addresses structure pointer
 *
 * @plantumlfile sem_test_inj_and_corr.puml
 */
void sem_test_inj_and_corr(sem_uart_t *uart, sem_addr_t *addr);

void sem_test_status(sem_uart_t *uart);