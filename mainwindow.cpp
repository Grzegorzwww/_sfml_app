#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    serialPort = new C_serialPort(this);                                            //operaion on serial port class init
    serialPortGUI = new C_serialPortGUI(serialPort->serialport_availableports);     //serial port properies conrol GUI
    engine = new appEngine(ui, this);
    dataParser = new CollectData(serialPort->returnSerialPortPointer());





     connect(serialPort->serial_port, SIGNAL(readyRead()), dataParser, SLOT(parseDataFromSerialPort()));


     connect(ui->pushButton, SIGNAL(clicked(bool)), serialPortGUI, SLOT(showWindow(bool)));
     connect(ui->pushButton_2, SIGNAL(clicked(bool)),  this,  SLOT(getSerialConfig(bool)));
     connect(ui->pushButton_3, SIGNAL(clicked(bool)), dataParser, SLOT(sendDataFrame(bool)));


     connect(dataParser, SIGNAL(incoming_frame_complete()), this, SLOT(showIncomingFrame()));
     connect(dataParser, SIGNAL(incoming_frame_complete()), this, SLOT(procedureFrameUnfold()));
     connect(dataParser, SIGNAL(outgoing_frame_sended()), this, SLOT(showOutgoingFrame()));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete serialPort;
    delete serial;
    delete serialPortGUI;
    delete engine;
}


void MainWindow::getSerialConfig(bool){

    serialProperty_pointer = serialPortGUI->returnSerialPrperty();
    serialPort->openSerialPort_FromGui(serialProperty_pointer, this);
    qDebug() << serialProperty_pointer->name;
    qDebug() << serialProperty_pointer->serialport_baudrate;
    qDebug() << serialProperty_pointer->serialport_databits;
    qDebug() << serialProperty_pointer->serialport_flowcontrol;
    qDebug() << serialProperty_pointer->serialport_parity;
    qDebug() << serialProperty_pointer->serialport_stopbits;
}

 void MainWindow::showIncomingFrame(void){
     QString str;
     int i;
     for(i = 0; i < 10; i++){
         str.push_back(QString::number(dataParser->dataFromDevice[i], 16));
         if(i != 10 - 1)
         str.push_back(QString(" - "));
     }
     ui->lineEdit->setText(str);
 }
 void MainWindow::showOutgoingFrame(void){
     QString str;
     int i;
     for(i = 0; i < 10; i++){
         str.push_back(QString::number(dataParser->data_to_send[i], 16));
         if(i != 10 - 1)
         str.push_back(QString(" - "));
     }
     ui->lineEdit_2->setText(str);
 }

void MainWindow::procedureFrameUnfold(void){
    engine->unfoldFrame(dataParser->dataFromDevice, FRAME_SIZE);
}
