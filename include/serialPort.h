#ifndef SERIALPORT_H
#define SERIALPORT_H


#include <termios.h>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <iostream>
#include <fcntl.h>
#include <unistd.h>

#define PORT_NAME "/dev/ttyUSB0"
#define DEF_PORT_BAUDRATE B115200

using namespace std;


class serialPort
{

public:

    serialPort();
    serialPort(string port_name, speed_t baudrate);
    ~serialPort();

    bool initConnection();
    int getRecivedDataSize();



private:

    speed_t _baudrate;
    string _port_name;
    int _serial_port_fd;
    ssize_t _recived_data_size;
    unsigned char _rx_buffer[256];

};

#endif // SERIALPORT_H
