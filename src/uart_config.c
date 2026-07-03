#include "./../include/uart_config.h"

#include <fcntl.h>
#include <stdlib.h>
#include <termios.h>
#include <stdio.h>

int open_serial_port( char* port_path )
{
    int fd;

    fd = open( port_path , O_RDWR | O_NONBLOCK ); // *Fix* Need to check alternative flags in manual

    if ( fd < 0 )
    {
        printf( "Could not open the file '%s'! File descriptor was: %d.\r\n" , port_path , fd );
        exit( EXIT_FAILURE );
    }

    return fd;
}

void configure_serial_port( int fd )
{
    struct termios tty_attributes;

    if ( tcgetattr( fd , &tty_attributes ) )
    {
        printf( "Something went wrong while getting port attributes!\r\n" );
        exit( EXIT_FAILURE );
    }

    cfsetispeed( &tty_attributes , B115200 );
    cfsetospeed( &tty_attributes , B115200 );

    // *Fix* Need to check alternative flags in manual
    tty_attributes.c_iflag &= ~( IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON );
    tty_attributes.c_oflag &= ~OPOST;
    tty_attributes.c_lflag &= ~( ECHO   | ECHONL | ICANON | ISIG   | IEXTEN );
    tty_attributes.c_cflag &= ~( CSIZE  | PARENB | CSTOPB );
    tty_attributes.c_cflag |=  CS8;

    if ( tcsetattr( fd , TCSANOW , &tty_attributes ) )
    {
        printf( "Something went wrong while setting port attributes!\r\n" );
        exit( EXIT_FAILURE );
    }
}

void close_serial_port( int fd )
{
    // int close_code;
    
    close( fd );

    // if ( close_code < 0 )
    // {
    //     printf( "Could not close the file! File descriptor was: %d\r\n" , fd );
    //     exit( EXIT_FAILURE );
    // }
}