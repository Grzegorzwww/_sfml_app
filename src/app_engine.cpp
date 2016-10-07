#include "app_engine.h"


app_engine::app_engine()
{



     collected_data = new collectData();
     game_gui = new gameGui();


     keyboard_events = new keyboardEvents(game_gui->returnAppWindow());




}
app_engine::~app_engine()
{

}
void app_engine::run(void){

    game_gui->run();
    keyboard_events->run();


    //game_gui->run_();
    //pthread_create(&gui_thread, NULL, &gameGui::run_gui, &game_gui);


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






