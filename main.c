#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <pthread.h>

#define SERIAL_PORT_PATH "/dev/ttyUL0"
#define COMMAND_SIZE     16
#define BUFFER_SIZE      1024

struct termios g_tty;
int            g_fd;

// FILE OPERATION
static int file_open_and_get_descriptor( const char *fname )
{
    int fd;

    fd = open( fname , O_RDWR | O_NONBLOCK );

    if ( fd < 0 )
    {
        printf( "Could not open file %s...%d\r\n" , fname , fd );
    }

    return fd;
}

static int file_close( int fd )
{
    return close( fd );
}


static void open_serial_port( void )
{
    g_fd = file_open_and_get_descriptor( SERIAL_PORT_PATH );

    if ( g_fd < 0 )
    {
        printf( "Something went wrong while opening the port...\r\n" );
        exit( EXIT_FAILURE );
    }
}

static void configure_serial_port( void )
{
    if ( tcgetattr( g_fd , &g_tty ) )
    {
        printf( "Something went wrong while getting port attributes...\r\n" );
        exit( EXIT_FAILURE );
    }

    cfsetispeed( &g_tty , B115200 );
    cfsetospeed( &g_tty , B115200 );

    g_tty.c_iflag &= ~( IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON );
    g_tty.c_oflag &= ~OPOST;
    g_tty.c_lflag &= ~( ECHO   | ECHONL | ICANON | ISIG   | IEXTEN );
    g_tty.c_cflag &= ~( CSIZE  | PARENB | CSTOPB );
    g_tty.c_cflag |=  CS8;

    if ( tcsetattr( g_fd , TCSANOW , &g_tty ) )
    {
        printf( "Something went wrong while setting port attributes...\r\n" );
        exit( EXIT_FAILURE );
    }
}

static void close_serial_port( void )
{
    file_close( g_fd );
}


static void sem_switch_to_idle( void )
{
    char    command [ COMMAND_SIZE ];
    char    buffer  [ BUFFER_SIZE  ];

    //--------------------------------------------------------------

    memset( buffer , 0 , BUFFER_SIZE );
    
    read( g_fd , ( uint8_t* )buffer , BUFFER_SIZE  );

    for ( size_t i = 0 ; i < BUFFER_SIZE ; ++i )
    {
        printf( "%c" , buffer[ i ] );

        if ( buffer[ i ] == '\r' )
            printf( "\n" );
    }

    printf( "\r\n" );

    //--------------------------------------------------------------

    memset( command , 0 , COMMAND_SIZE );

    strcpy( command , "I" );

    write( g_fd , ( uint8_t* )command , COMMAND_SIZE );

    printf( "\r\nCommand '%s' sent\r\n" , command );

    //--------------------------------------------------------------

    sleep( 1 );

    memset( buffer , 0 , BUFFER_SIZE );

    read( g_fd , ( uint8_t* )buffer  , BUFFER_SIZE );

    for ( size_t i = 0 ; i < BUFFER_SIZE ; ++i )
    {
        printf( "%c" , buffer[ i ] );

        if ( buffer[ i ] == '\r' )
            printf( "\n" );
    }

    printf( "\r\n" );

    //--------------------------------------------------------------

    memset( command , 0 , COMMAND_SIZE );

    strcpy( command , "Q C000C350000" ); // LFA: 50000

    write( g_fd , ( uint8_t* )command , COMMAND_SIZE );

    printf( "\r\nCommand '%s' sent\r\n" , command );

    //--------------------------------------------------------------

    sleep( 1 );

    memset( buffer , 0 , BUFFER_SIZE );
    
    read( g_fd , ( uint8_t* )buffer  , BUFFER_SIZE );

    for ( size_t i = 0 ; i < BUFFER_SIZE ; ++i )
    {
        printf( "%c" , buffer[ i ] );

        if ( buffer[ i ] == '\r' )
            printf( "\n" );
    }

    printf( "\r\n" );
}


int main( void )
{
    printf( "Starting the sem_init application...\r\n" );

    open_serial_port();

    configure_serial_port();

    // perform_demo();
    sem_switch_to_idle();

    close_serial_port();

    return 0;
}

