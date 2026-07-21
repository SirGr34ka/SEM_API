#pragma once

#include <stdint.h>

/**
 * @name UART commands
 *
 * @brief Commands which can be sent to the SEM controller via UART
 */
///@{
#define MOVE_TO_IDLE 00 /**< Move to IDLE state command */
#define DO_QUARY 01     /**< Run QUARY command with random argument */
#define DO_INJECTION 02 /**< Run QUARY command with user defined argument */

#define MOVE_TO_OBSERVATION 10 /**< Move to OBSERVATION state command */

#define MOVE_TO_DETECT_ONLY 20 /**< Move to DETECT ONLY state command */

#define MOVE_TO_DIAGNOSTIC_SCAN                                                \
  30 /**< Move to DIAGNOSTIC SCAN state command  \*/
///@}

/**
 * @brief
 * Checks if a file has opened and has read-write access mode
 *
 * @param fd
 * file descriptor for serial port
 */
void check_fd(const int fd);

/**
 * @brief
 * Checks if LFA reserved
 *
 * @param lfa
 * linear frame address
 *
 * @return
 * if LFA reserved - 1, else - 0
 */
int is_lfa_reserved(const uint32_t lfa);

/**
 * @brief
 * Sends a command to SEM controller via UART
 *
 * @param fd
 * file descriptor for serial port
 *
 * @param command_num
 * command number, use @ref UART commands macros instead of numbers
 *
 * @param lfa
 * linear frame address used by @ref DO_QUARY and @ref DO_INJECTION commands
 * only, otherwise will be ignored
 *
 * @param wa
 * word address used by @ref DO_INJECTION command only,
 * otherwise will be ignored
 *
 * @param ba
 * bit address used by @ref DO_INJECTION command only,
 * otherwise will be ignored
 */
void send_command(const int fd, const uint8_t command_num, const uint32_t lfa,
                  const uint16_t wa, const uint16_t ba);

/**
 * @brief
 * Recieves data from SEM controller via UART
 *
 * @param fd
 * file descriptor for serial port
 */
void recieve_data(const int fd);