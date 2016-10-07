#ifndef APP_ENGINE_H
#define APP_ENGINE_H

#include <iostream>
#include <string>
#include "cdelegate.h"
#include "cserialport.h"
#include "TimerObject.h"
#include "collectData.h"

#include <sys/time.h>
#include <sys/resource.h>
#include <sys/epoll.h>
#include "game_gui.h"
#include "keyboard_events.h"




using namespace std;

class app_engine
{
    public:
        app_engine();
        virtual ~app_engine();
        void run(void);
        void createConnection(string com_name = "/dev/ttyUSB0" );





    private:
        CSerialPort *serial;
        collectData *collected_data;
        gameGui *game_gui;
        keyboardEvents *keyboard_events;

        pthread_t gui_thread;




    protected:
};

void getData(vector <char> data);
    //void getDataFromSerial(vector< char> data);

#endif // APP_ENGINE_H
