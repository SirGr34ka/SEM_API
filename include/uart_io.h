#pragma once

/**
 * @name UART commands
 *
 * @brief Commands which can be sent to the SEM module via UART
 */
///@{
#define MOVE_TO_IDLE 00   /**< Move to IDLE state command */
#define DO_QUARY_RAND 01  /**< Run QUARY command with random argument */
#define DO_QUARY_CONST 02 /**< Run QUARY command with user defined argument */

#define MOVE_TO_OBSERVATION 10 /**< Move to OBSERVATION state command */

#define MOVE_TO_DETECT_ONLY 20 /**< Move to DETECT ONLY state command */

#define MOVE_TO_DIAGNOSTIC_SCAN                                                \
  30 /**< Move to DIAGNOSTIC SCAN state command  \*/
///@}

/**
 * @brief
 * Checks if a file has opened and has RW access mode
 *
 * @param fd
 * file descriptor file
 */
void check_fd(const int fd);

// void send_command(const int fd, const int command, ...);
void send_command(const int fd, const int command_num);

void recieve_data(const int fd);