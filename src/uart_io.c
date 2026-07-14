#include "uart_io.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>

#define MAX_FRAME 0x0000E0BC

void check_fd(const int fd)
{
    const int flags = fcntl(fd, F_GETFL);
    const int access_flag = O_ACCMODE & flags;

    if ((flags < 0) || (access_flag != O_RDWR)) {
        printf("Could not open the file! File descriptor was: %d.\r\n", fd);
        exit(EXIT_FAILURE);
    }
}

int is_lfa_reserved(const uint32_t lfa)
{
    return !((lfa > (MAX_FRAME / 3 * 2)) && (lfa < (MAX_FRAME - 1)));
}

/**
 * @brief
 * Makes full address by concatinating LFA, word address and byte address for query and injection
 * commands usage
 *
 * Full address looks like this mask: 00ll llll llll llll wwww wwwb bbbb, where 'l' bit for LFA, 'w'
 * bit for WA and 'b' bit for BA. Function checks if LFA, WA and BA in the range, else reports
 * in terminal about out of range.
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
static uint64_t make_address(const uint32_t lfa, const uint16_t wa, const uint16_t ba)
{
    const uint16_t MAX_WORD = 92; // 122 for UltraScale
    const uint16_t MAX_BIT = 31;

    uint64_t address = 0;

    if (is_lfa_reserved(lfa)) {
        printf("LFA is out of valid range! Choose valid frame address within %d..%d range.\r\n",
               (MAX_FRAME / 3 * 2) + 1,
               MAX_FRAME - 2);
        exit(EXIT_FAILURE);
    } else {
        address |= lfa << 12;
    }

    if (wa > MAX_WORD) {
        printf(
            "Word address is out of valid range! Choose valid word address within 0..%d range.\r\n",
            MAX_WORD);
        exit(EXIT_FAILURE);
    } else {
        address |= wa << 5;
    }

    if (ba > MAX_BIT) {
        printf(
            "Bit address is out of valid range! Choose valid bit address within 0..%d range.\r\n",
            MAX_BIT);
        exit(EXIT_FAILURE);
    } else {
        address |= ba;
    }

    return address;
}

void send_command(const int fd,
                  const uint8_t command_num,
                  const uint32_t lfa,
                  const uint16_t wa,
                  const uint16_t ba)
{
    check_fd(fd);

    uint64_t address;
    const size_t COMMAND_SIZE = 16;
    char command[COMMAND_SIZE];

    memset(command, 0, COMMAND_SIZE);

    switch (command_num) {
        /* States moving */
        case MOVE_TO_IDLE:
            strcpy(command, "I");
            break;

        case MOVE_TO_OBSERVATION:
            strcpy(command, "O");
            break;

        case MOVE_TO_DETECT_ONLY:
            strcpy(command, "D");
            break;

        case MOVE_TO_DIAGNOSTIC_SCAN:
            strcpy(command, "U");
            break;

        /* Commands in IDLE */
        case DO_QUARY:
            strcpy(command, "Q C00");
            address = make_address(lfa, 0, 0);
            snprintf(command + 5, COMMAND_SIZE - 5, "%0*lX", 8, address);
            break;

        case DO_INJECTION:
            strcpy(command, "N C00");
            address = make_address(lfa, wa, ba);
            snprintf(command + 5, COMMAND_SIZE - 5, "%0*lX", 8, address);
            break;

        default:
            strcpy(command, "I");
            break;
    }

    if (write(fd, (uint8_t *)command, COMMAND_SIZE) < 0) {
        printf("Could not write to file! File descriptor was: %d.\r\n", fd);
        exit(EXIT_FAILURE);
    }

    printf("\r\nCommand '%s' sent\r\n", command);
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
        printf("SEM controller detected an internal inconsistency! FPGA must be reconfigured!\r\n");
        exit(EXIT_FAILURE);
    }
}

void recieve_data(const int fd)
{
    check_fd(fd);

    const size_t BUFFER_SIZE = 1024;
    char buffer[BUFFER_SIZE];

    memset(buffer, 0, BUFFER_SIZE);

    ssize_t bytes = read(fd, (uint8_t *)buffer, BUFFER_SIZE);

    if (bytes >= 5) {
        check_hlt(buffer);
    }

    for (size_t i = 0; i < BUFFER_SIZE; ++i) {
        printf("%c", buffer[i]);

        if (buffer[i] == '\r') {
            printf("\n");
        }
    }

    printf("\r\n");
}
