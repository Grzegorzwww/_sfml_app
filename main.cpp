#include <iostream>
#include "app_engine.h"

using namespace std;



int main()
{
    int i;

    app_engine *app = new app_engine();
    app->createConnection("/dev/ttyUSB0");

     app->run();


    while(1){
    }

}


