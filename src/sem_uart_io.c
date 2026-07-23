#include "sem_uart_io.h"

#include <stdio.h>  // printf()
#include <stdlib.h> // exit(), EXIT_FAILURE
#include <errno.h>  // errno
#include <string.h> // strcpy(), memset() strerror()
#include <unistd.h> // read(), write(), ssize_t

#define MAX_FRAME 0x0000E0BC
#define MAX_WORD 92 // 122 for UltraScale

/**
 * @brief
 * Makes full address by concatinating LFA, word address and byte address for
 * query and injection commands usage
 *
 * Full address looks like this mask: 00ll llll llll llll wwww wwwb bbbb, where
 * 'l' bit for LFA, 'w' bit for WA and 'b' bit for BA. Function checks if LFA,
 * WA and BA in the range, else reports in terminal about out of range.
 *
 * @param lfa
 * linear frame address
 *
 * @param wa
 * word address
 *
 * @param ba
 * byte address
 *
 * @return
 * Full address in LFA format
 */
static uint64_t format_address(const sem_addr_t *addr)
{
    if (addr == NULL) {
        printf("Address is undefined! Define the address.\r\n");
        exit(EXIT_FAILURE);
    }

    const uint16_t MAX_BIT = 31;

    uint32_t lfa;
    uint16_t wa;
    uint16_t ba;
    uint64_t formated_addr = 0;

    lfa = addr->lfa;
    wa = addr->wa;
    ba = addr->ba;

    if (lfa > (MAX_FRAME - 2)) {
        printf(
            "LFA %d is out of valid range! Choose valid frame address within "
            "0..%d range.\r\n",
            lfa,
            MAX_FRAME - 2);
        exit(EXIT_FAILURE);
    } else {
        formated_addr |= lfa << 12;
    }

    if (wa > MAX_WORD) {
        printf(
            "Word address %d is out of valid range! Choose valid word address "
            "within 0..%d range.\r\n",
            wa,
            MAX_WORD);
        exit(EXIT_FAILURE);
    } else {
        formated_addr |= wa << 5;
    }

    if (addr->ba > MAX_BIT) {
        printf(
            "Bit address %d is out of valid range! Choose valid bit address "
            "within 0..%d range.\r\n",
            ba,
            MAX_BIT);
        exit(EXIT_FAILURE);
    } else {
        formated_addr |= addr->ba;
    }

    return formated_addr;
}

static void check_is_lfa_reserved(const uint32_t lfa)
{
    if (!((lfa > (MAX_FRAME / 3 * 2)) && (lfa < (MAX_FRAME - 1)))) {
        printf(
            "LFA is out of valid range! Choose valid frame address within "
            "%d..%d range.\r\n",
            (MAX_FRAME / 3 * 2) + 1,
            MAX_FRAME - 2);
        exit(EXIT_FAILURE);
    }
}

void sem_uart_send(const sem_uart_t *uart, sem_uart_cmd_t cmd, const sem_addr_t *addr)
{
    uint64_t formated_addr;
    const size_t CMD_STR_SIZE = 16;
    char cmd_str[CMD_STR_SIZE];

    memset(cmd_str, 0, CMD_STR_SIZE);

    switch (cmd) {
        /* States moving */
        case MOVE_TO_IDLE : {
            strcpy(cmd_str, "I");
        } break;

        case MOVE_TO_OBSERVATION : {
            strcpy(cmd_str, "O");
        } break;

        case MOVE_TO_DETECT_ONLY : {
            strcpy(cmd_str, "D");
        } break;

        case MOVE_TO_DIAGNOSTIC_SCAN : {
            strcpy(cmd_str, "U");
        } break;

        /* Commands in IDLE */
        case DO_QUARY : {
            strcpy(cmd_str, "Q C00");
            formated_addr = format_address(addr);
            snprintf(cmd_str + 5, CMD_STR_SIZE - 5, "%0*lX", 8, formated_addr);
        } break;

        case DO_INJECTION : {
            check_is_lfa_reserved(addr->lfa);
            strcpy(cmd_str, "N C00");
            formated_addr = format_address(addr);
            snprintf(cmd_str + 5, CMD_STR_SIZE - 5, "%0*lX", 8, formated_addr);
        } break;

        default : {
            strcpy(cmd_str, "I");
        }
    }

    ssize_t write_res = write(uart->fd, (uint8_t *)cmd_str, CMD_STR_SIZE);

    if (write_res < 0) {
        printf("Error %i happened while writing command to UART: %s\r\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    printf("\r\nCommand '%s' sent\r\n", cmd_str);
}

/**
 * @brief
 * Checks a char array for the HLT message
 *
 * @param data
 * char array
 */
static void check_hlt(const char *data)
{
    char *state_changed_to_hlt = strstr(data, "SC 9F");
    char *hlt_messege = strstr(data, "HLT");

    if ((state_changed_to_hlt != NULL) || (hlt_messege != NULL)) {
        printf(
            "SEM controller detected internal inconsistency! FPGA must be "
            "reconfigured.\r\n");
        exit(EXIT_FAILURE);
    }
}

void sem_uart_recieve(const sem_uart_t *uart, char *buffer, size_t size)
{
    memset(buffer, 0, size);

    ssize_t bytes = read(uart->fd, (uint8_t *)buffer, size);

    if (bytes < 0) {
        printf("Error %i happened while reading UART: %s\r\n", errno, strerror(errno));
    }

    if (bytes >= 5) {
        check_hlt(buffer);
    }
}
