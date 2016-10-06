#include "serialPort.h"

using namespace std;


serialPort::serialPort()
{

    _baudrate = DEF_PORT_BAUDRATE;
    _port_name = "/dev/ttyUSB1";

}
serialPort::serialPort(string port_name, speed_t baudrate) :  _port_name(port_name) , _baudrate(baudrate)
{
    cout << "Port name: "<<port_name<<endl;
    cout << "Baudrate : "<<(speed_t)_baudrate<<endl;
    //
}
serialPort::~serialPort()
{

}



bool serialPort::initConnection()
{

    _serial_port_fd = open( _port_name.c_str(), O_RDWR | O_NONBLOCK);

    if( _serial_port_fd != ( -1 ) )
    {
        termios loc_termios;
        tcgetattr( _serial_port_fd, &loc_termios );
        cfsetspeed( &loc_termios, _baudrate );
        cfmakeraw( &loc_termios );
        tcsetattr( _serial_port_fd, TCSANOW, &loc_termios );
        tcflush( _serial_port_fd, TCIOFLUSH );
        return true;
    }
    flush( cout );
    return false;
}

int serialPort::getRecivedDataSize(){

        _recived_data_size = read( _serial_port_fd, &_rx_buffer, 256);


        if(_recived_data_size > 0){
			cout << "Frame size = " << _recived_data_size << endl;
			for(int i =0; i < _recived_data_size; i++){
				cout << (int)_rx_buffer[i];
			}
			cout << endl;
		}

		return 1;

}
