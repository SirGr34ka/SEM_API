#pragma once

int open_serial_port(char *port_path);
void configure_serial_port(int fd);
void close_serial_port(int fd);