#include "collect_data.h"


CollectData::CollectData(QSerialPort *serial = 0)
{

    _temp_serial_ptr = serial;
    if(_temp_serial_ptr->isOpen()){
        qDebug() <<"_temp_serial_ptr->isOpen()";
    }

    _frame_sending_timer = new QTimer(this);
    iRx = 0;
    sending_frames_counter = 0;
    //_buffor_in.clear();


    connect(_frame_sending_timer, SIGNAL(timeout()), this, SLOT(collectOutgoingFrame()));

}
CollectData::~CollectData(){
    _frame_sending_timer->stop();
    delete _frame_sending_timer;

}



QSerialPort *CollectData::returnSerialPort(void) const {
    return  _temp_serial_ptr;
}
QTimer *CollectData::returnFrameSendingTimer(void) const{

    return _frame_sending_timer;
}

void CollectData::startSendigFrames(void){

    _frame_sending_timer->start(FRAME_SEND_TIME_PERIOD_MS);
}
void CollectData::stopSendigFrames(void){
    _frame_sending_timer->stop();
}

void CollectData::collectOutgoingFrame(){


}





void CollectData::sendDataFrame(bool x){

    int i;
    if( _temp_serial_ptr->isOpen()){

        data_to_send[0] =  0xC1;
        data_to_send[1] =  0x00;
        data_to_send[2] =  sending_frames_counter++;
        if(sending_frames_counter == 127)
            sending_frames_counter = 0;
        data_to_send[3] =  0x02;
        data_to_send[4] =  0x03;
        data_to_send[5] =  0x04;
        data_to_send[6] =  0x05;
        data_to_send[7] =  0x06;
        data_to_send[8] =  0x07;

        data_to_send[FRAME_SIZE -1] = FRAME_SIZE;
        frm_calcCrc(data_to_send, FRAME_SIZE, 1);
        std::cerr << "------------------------";
        for(i = 0; i < FRAME_SIZE;  i++){
            _temp_serial_ptr->putChar( data_to_send[i]);
            //qDebug() << QString::number(data_to_send[i], 16);

        }
    emit outgoing_frame_sended();
    }
    else
        std::cerr << "serial port is not open !"<<endl;
}



void CollectData::parseDataFromSerialPort(void){
    int i;
    _temp_serial_ptr->setReadBufferSize(1024);
    if (_temp_serial_ptr->bytesAvailable() > 0) {
        _dane = _temp_serial_ptr->readAll();
        /*std::cerr << "data_to_send : ------------------------";
        std::cerr<<endl;
        for(i = 0; i < _dane.size(); i++){
            qDebug()  <<QString::number( _dane[i], 16);
        }*/

         for(i = 0; i < _dane.size(); i++){
            if(parseData(_dane[i]) == 1){
                for(i = 0; i < 10; i++){
                    //qDebug() << QString::number(dataFromDevice[i] , 16);

                }
                //qDebug() <<"emit incoming_frame_complete();";
                emit incoming_frame_complete();
                _dane.clear();

            }
        }
    }
}


int CollectData::parseData(const unsigned char data_in){

    unsigned char temp =  data_in;
    irqRxBuff[iRx] = temp;

    /*if(_buffor_in.size() > 1024)
        _buffor_in.clear();*/

    if((temp>(SERIAL_MIN_FRAME_SIZE -1)) && (temp<(iRx+2)) && (irqRxBuff[iRx-1]==irqRxBuff[iRx+2-temp]) && ((irqRxBuff[iRx+1-temp])==UH_HEADER_RX)){
        unsigned int iHead = iRx + 1 - temp;
        if(irqRxBuff[iRx-1] == frm_calcCrc(&irqRxBuff[iHead], temp, 0)){

            unsigned int n=0;
            for(; n<temp; ++n){
                dataFromDevice[n] = irqRxBuff[iHead + n];
            }
            iRx = 0;
            return 1;
        }
    }
    if(++iRx == 512)
        iRx = 0;

    return 0;
}

unsigned char CollectData::frm_calcCrc(unsigned char *frame, unsigned int size, unsigned int setFlag){
    unsigned char sum = *frame + *(frame + size - 1);
    unsigned char n;
    for(n=2; n < (size-2); ++n)
        sum += *(frame+n);
    sum %= 256;
    if(setFlag){
        *(frame+1) = sum;
        *(frame+size-2) = sum;
    }
    return sum;
}




