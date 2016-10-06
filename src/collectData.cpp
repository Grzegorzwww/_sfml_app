#include "collectData.h"

using namespace std;



collectData::collectData()
{

    cout << "enter collectData()"<<endl;




}

collectData::~collectData()
{
    //dtor
}


void collectData::getDataFromSerial(vector <char> data) {

    incoming_data = data;


    cout <<"Incoming frame: ";

    for(int i = 0; i <incoming_data.size(); i++)
    {
        unsigned char temp = incoming_data.at(i);
        cout << hex << (int)temp;
        if(i !=  incoming_data.size() -1 )
            cout <<"-";
    }
    cout << endl;

}

ostream & operator<<(ostream &os, collectData &in){
    cout.setf(ios::hex, ios::basefield);

    for (int i = 0; i < in.incoming_data.size(); i++)
    {
        unsigned char temp = in.incoming_data.at(i);
        os <<hex << (int)temp;
        if(i != in.incoming_data.size() -1 )
            os <<"-";
    }
    os << endl;

    return os;
}

