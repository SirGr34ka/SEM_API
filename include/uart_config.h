#pragma once

extern int open_serial_port( char* port_path );
extern void configure_serial_port( int fd );
extern void close_serial_port( int fd );