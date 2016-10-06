#ifndef COLLECTDATA_H
#define COLLECTDATA_H

#include <iostream>
#include "cserialport.h"
#include <string>
#include <vector>




using namespace std;

class collectData
{
    friend class CSerialPort;

    public:
        collectData();
        virtual ~collectData();

        //char incoming_data[20];
       // unsigned int data_size;
        //int x;

        vector <char> incoming_data;

        void getDataFromSerial(vector <char> data);
        friend ostream & operator<<(ostream &os, collectData &in);




    protected:

    private:





};

#endif // COLLECTDATA_H
