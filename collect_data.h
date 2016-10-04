#ifndef COLLECTDATA_H
#define COLLECTDATA_H


#include <QSerialPort>
#include <QObject>
#include <QDebug>
#include <QCoreApplication>
#include <QTimer>
#include <QLabel>
#include <QElapsedTimer>
#include <iostream>
#include <vector>
#include <stdint.h>
#include <stdlib.h>
#include <iostream>

#define SERIAL_MIN_FRAME_SIZE 6
#define UH_HEADER_RX 0xC1
#define FRAME_SIZE 10

typedef unsigned char uint8_t;

#define FRAME_HEAD 0xC1
#define DATA_SIZE 4




#define TRUE 1
#define FALSE 0

#define FRAME_SEND_TIME_PERIOD_MS 100
#define RECIVED_BUFFOR_MAX_SIZE 32










class CollectData : public QObject {
    Q_OBJECT



public:

    CollectData(QSerialPort *serial);
    ~CollectData();

    QSerialPort *returnSerialPort(void) const;
    QTimer *returnFrameSendingTimer(void) const;

    unsigned char dataFromDevice[512];
    unsigned char data_to_send[FRAME_SIZE];




private :
    QSerialPort *_temp_serial_ptr;
    QByteArray _dane;
    QTimer *_frame_sending_timer;
    long int sending_frames_counter;




    unsigned char irqRxBuff[512];

    int iRx;



   // std::vector < uint8_t > _temp_buffor_in;



    int _incoming_frame_size;
    int _data_coping_counter;
    int _outgoing_frame_counter;


    int parseData(const unsigned char data_in);
    unsigned char frm_calcCrc(unsigned char *frame, unsigned int size, unsigned int setFlag);


public slots:
    void collectOutgoingFrame();
    void parseDataFromSerialPort(void);
    void sendDataFrame(bool x);
    void startSendigFrames(void);
    void stopSendigFrames(void);

signals:

    void incoming_frame_complete(void);
    void outgoing_frame_sended(void);
    void recived_frame_remaining_time(QString);
    void sending_frame_remaining_time(QString);

};

#endif // COLLECTDATA_H
