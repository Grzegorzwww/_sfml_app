#include "app_engine.h"


app_engine::app_engine()
{
     collected_data = new collectData();
     game_gui = new gameGui();

}
app_engine::~app_engine()
{
    //dtor
}
void app_engine::run(void){





}
void app_engine::createConnection(string com_name )
{
    serial = new CSerialPort();
   // serial->connect<getData>();
    serial->connect<collectData, &collectData::getDataFromSerial>(collected_data);
    serial->setBaudRate(CSerialPort::Baud115200);
    serial->setFlowControl(CSerialPort::NoFlowControl);
    serial->setDataBits(CSerialPort::Data8);
    serial->setParity(CSerialPort::NoParity);
    cout << com_name;
    serial->setPortName(com_name);

    if (serial->openPort(CSerialPort::OM_READ_WRITE))
    {
        cout << com_name<< " openned..." << endl;
    }

}






