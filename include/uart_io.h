#pragma once

#include <stddef.h>

void send_command(int fd, char *command, size_t size);
int recieve_data(int fd);