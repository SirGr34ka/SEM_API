#pragma once

#include <stddef.h>

extern void send_command( int fd , char* command , size_t size );
extern int recieve_data( int fd );