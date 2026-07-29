#include "sem_test.h"
#include "sem_uart_handlers.h"

#include <stdio.h>  // printf()
#include <unistd.h> // sleep()
#include <stddef.h>

static void print_recieved_data(const char *buffer, size_t size)
{
    for (size_t i = 0; i < size; ++i) {
        printf("%c", buffer[i]);

        if (buffer[i] == '\r') {
            printf("\n");
        }
    }

    printf("\r\n");
}

static int(char *buffer, size_t size)
{
    return 0;
}

void sem_test_inj_and_corr(sem_uart_t *uart, sem_addr_t *addr)
{
    const size_t SIZE = 1024;
    char buffer[SIZE];

    sem_addr_t test_addr;

    if (addr == NULL) {
        test_addr.lfa = 50000;
        test_addr.wa = 46;
        test_addr.ba = 5;
    } else {
        test_addr.lfa = addr->lfa;
        test_addr.wa = addr->wa;
        test_addr.ba = addr->ba;
    }

    printf("\r\nInjection test scenario starts...\r\n");

    /* Wait of SEM module rebooting */
    sem_uart_recieve(uart, buffer, SIZE, NULL, NULL);
    print_recieved_data(buffer, SIZE);

    /* Move to IDLE state */
    sem_uart_send(uart, MOVE_TO_IDLE, NULL);

    sleep(1);
    sem_uart_recieve(uart, buffer, SIZE, NULL, NULL);
    print_recieved_data(buffer, SIZE);

    /* Use of QUERY command */
    sem_uart_send(uart, DO_QUARY, &test_addr);

    sleep(1);
    sem_uart_recieve(uart, buffer, SIZE, NULL, NULL);
    print_recieved_data(buffer, SIZE);

    /* Use of INJECTION command */
    sem_uart_send(uart, DO_INJECTION, &test_addr);

    sleep(1);
    sem_uart_recieve(uart, buffer, SIZE, NULL, NULL);
    print_recieved_data(buffer, SIZE);

    /* Use of QUERY command */
    sem_uart_send(uart, DO_QUARY, &test_addr);

    sleep(1);
    sem_uart_recieve(uart, buffer, SIZE, NULL, NULL);
    print_recieved_data(buffer, SIZE);

    /* Move to OBSERVATION state */
    sem_uart_send(uart, MOVE_TO_OBSERVATION, NULL);

    sleep(1);
    sem_uart_recieve(uart, buffer, SIZE, NULL, NULL);
    print_recieved_data(buffer, SIZE);

    /* Move to IDLE state */
    sem_uart_send(uart, MOVE_TO_IDLE, NULL);

    sleep(1);
    sem_uart_recieve(uart, buffer, SIZE, NULL, NULL);
    print_recieved_data(buffer, SIZE);

    /* Use of QUERY command */
    sem_uart_send(uart, DO_QUARY, &test_addr);

    sleep(1);
    sem_uart_recieve(uart, buffer, SIZE, NULL, NULL);
    print_recieved_data(buffer, SIZE);

    /* Move to DETECT-ONLY state */
    sem_uart_send(uart, MOVE_TO_DETECT_ONLY, NULL);

    sleep(1);
    sem_uart_recieve(uart, buffer, SIZE, NULL, NULL);
    print_recieved_data(buffer, SIZE);
}

void sem_test_status(sem_uart_t *uart)
{
    const size_t SIZE = 128;
    char buffer[SIZE];

    /* Wait of SEM module rebooting */
    sem_uart_recieve(uart, buffer, SIZE, NULL, null_handler);
    print_recieved_data(buffer, SIZE);

    /* Move to IDLE state */
    sem_uart_send(uart, MOVE_TO_IDLE, NULL);

    sleep(1);
    sem_uart_recieve(uart, buffer, SIZE, NULL, NULL);
    print_recieved_data(buffer, SIZE);

    /* Use of STATUS command */
    sem_uart_send(uart, DO_STATUS, NULL);

    sleep(1);
    sem_uart_recieve(uart, buffer, SIZE, NULL, NULL);
    print_recieved_data(buffer, SIZE);

    /* Move to DETECTION-ONLY state */
    sem_uart_send(uart, MOVE_TO_DETECT_ONLY, NULL);

    sleep(1);
    sem_uart_recieve(uart, buffer, SIZE, NULL, NULL);
    print_recieved_data(buffer, SIZE);

    /* Use of STATUS command */
    sem_uart_send(uart, DO_STATUS, NULL);

    sleep(1);
    sem_uart_recieve(uart, buffer, SIZE, NULL, NULL);
    print_recieved_data(buffer, SIZE);

    /* Use of STATUS command */
    sem_uart_send(uart, DO_STATUS, NULL);

    /* Move to DETECTION-ONLY state */
    sem_uart_send(uart, MOVE_TO_DETECT_ONLY, NULL);

    sleep(1);
    sem_uart_recieve(uart, buffer, SIZE, NULL, NULL);
    print_recieved_data(buffer, SIZE);

    sleep(1);
    sem_uart_recieve(uart, buffer, SIZE, NULL, NULL);
    print_recieved_data(buffer, SIZE);
}
