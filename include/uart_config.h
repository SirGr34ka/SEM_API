#pragma once

/**
 * @brief
 * Opens a serial port
 *
 * @param port_path
 * path to serial device file
 *
 * @return
 * File descriptor int number
 */
int open_serial_port(const char *port_path);

/**
 * @brief
 * Configures a serial port
 *
 * Serial port will be configured with baudrate B115200 and raw mode via termios
 * attributes.
 *
 * @param fd
 * file descriptor for serial port
 */
void configure_serial_port(const int fd);

/**
 * @brief
 * Closes a serial port
 *
 * @param fd
 * file descriptor for serial port
 */
void close_serial_port(const int fd);