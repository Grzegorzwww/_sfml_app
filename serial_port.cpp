#include <QDebug>
#include <serial_port.h>




using namespace std;

/*-----------------------------------------class C_serialPort-----------------------------------------*/
C_serialPort::C_serialPort(QObject *parent){

    number_of_availableports = getAllAvailablePorts();
    serialport_isopen = false;

    serial_port = new QSerialPort(parent);
    ready_to_close = false;

    //    qDebug() << QString::number( number_of_availableports, 10);
    //    qDebug() << serialport_availableports[0] << endl;
    //    qDebug() << serialport_availableports[1] << endl;
}

C_serialPort::C_serialPort(){}

C_serialPort::~C_serialPort(){



}
QSerialPortInfo C_serialPort::returnSerialInfo(void){

    return info_serialport;
}

void C_serialPort::openSerialPort_Default(const char *portname){

    //serial_port = new QSerialPort(QString::fromUtf8(portname), parent);

    serial_port->setPortName(QString::fromLocal8Bit(portname));
    serial_port->setBaudRate(QSerialPort::Baud115200);
    serial_port->setDataBits(QSerialPort::Data8);
    serial_port->setParity(QSerialPort::NoParity);
    serial_port->setStopBits(QSerialPort::TwoStop);
    serial_port->setFlowControl(QSerialPort::NoFlowControl);
    serial_port->open(QIODevice::ReadWrite);
    serialport_isopen = true;
}


void C_serialPort::openSerialPort_FromGui(SerialportPropertyStruct *serialProperty, QObject *parent){


    //QSerialPortInfo &info
    //serial_port = new QSerialPort(serialProperty->name, parent);
    serial_port->setPortName(serialProperty->name);
    serial_port->setBaudRate(serialProperty->serialport_baudrate);
    serial_port->setDataBits(serialProperty->serialport_databits);
    serial_port->setParity(serialProperty->serialport_parity);
    serial_port->setStopBits(serialProperty->serialport_stopbits);
    serial_port->setFlowControl(serialProperty->serialport_flowcontrol);
    serial_port->open(QIODevice::ReadWrite);
    serialport_isopen = true;
}


int C_serialPort::getAllAvailablePorts(){
    int i = 0;
    foreach (info_serialport, QSerialPortInfo::availablePorts()) {
        serialport_availableports[i++] = info_serialport.portName();
    }
    serialport_availableports[i] = QString::null;
    return i;
}
void C_serialPort::closeSerialPort(){


    serial_port->close();
    serialport_isopen = false;


}
void C_serialPort::setSerialBaud(QSerialPort::BaudRate baud){
    serial_port->setBaudRate(baud);
}
void C_serialPort::setSerialDataBits(QSerialPort::DataBits databits){
    serial_port->setDataBits(databits);
}
void C_serialPort::setSerialStopBits(QSerialPort::StopBits stopbits){
    serial_port->setStopBits(stopbits);
}
void C_serialPort::setSerialParity(QSerialPort::Parity parity){
    serial_port->setParity(parity);
}
void C_serialPort::setSerialFlowControl(QSerialPort::FlowControl flowcontrol){
    serial_port->setFlowControl(flowcontrol);
}
QSerialPort *C_serialPort::returnSerialPortPointer(void){

    return serial_port;
}




//QByteArray *collectDatagetData(void){

//    QByteArray data;
//    data = serialport->readAll();
//    return &data;
//}


/*-----------------------------------------class C_serialPortGUI-----------------------------------------*/


C_serialPortGUI::C_serialPortGUI(QString *ports) {

    serial_window = new QDialog();

    createSerialConfigWindow(ports);        //create serial port properties window

    connect(Akcept, SIGNAL(clicked()), this,  SLOT(on_Akcept_clicked()));   //akcep properties
    connect(Cancel, SIGNAL(clicked()), this,  SLOT(on_Cancel_clicked()));   //cancel properties
    connect(Default, SIGNAL(clicked()), this,  SLOT(on_Default_clicked())); //set default properties for communication with driver CCS_01
}


void C_serialPortGUI::showWindow(bool)
{
    serial_window->show();
    serial_window->setWindowTitle("Komunikacja szeregowa - Parametry");
    serial_window->resize(600, 180);
    on_Default_clicked();
}


void C_serialPortGUI::hideWindow(void)
{
    serial_window->hide();
}

C_serialPortGUI::~C_serialPortGUI(){

    delete Akcept;
    delete Cancel;
    delete Default;
    delete name;
    delete baudrate;
    delete databits;
    delete portname_combobox;
    delete baudrate_combobox;
    delete databits_combobox;
    delete stopBits_group;
    delete parity_group;
    delete flowcontrol_group;
    delete stopBits_vbox;
    delete parity_vbox;
    delete flowcontrol_vbox;
    delete stopbits_one;
    delete stopbits_two;
    delete stopbits_oneandhalf;
    delete flowcontrol_no;
    delete flowcontrol_hardware;
    delete flowcontrol_software;
    delete parity_no;
    delete parity_even;
    delete parity_odd;
    delete serial_window;

}

void C_serialPortGUI::createSerialConfigWindow(QString *ports){

    qDebug() << "Otwarcie Okna: Komunikacja szeregowa - Parametry" << endl;
    createLabels();
    createCompoBoxs(ports);
    createGroups();
    createPushButtons();
}

void C_serialPortGUI::createCompoBoxs(QString *ports){
    int i;
    portname_combobox = new QComboBox(serial_window);
    portname_combobox->setGeometry(120, 15, 70, 20);
    for(i = 0; i < MAX_AVAILABLEPORTS; i++){
        if(ports[i] ==  QString::null)
            break;
        else
            portname_combobox->addItem(ports[i]);
        qDebug() << ports[i] << endl;
    }

    baudrate_combobox = new QComboBox(serial_window);
    baudrate_combobox->setGeometry(120, 50, 70, 20);

    baudrate_combobox->addItem("9600", BAUD9600);
    baudrate_combobox->addItem("19200", BAUD19200);
    baudrate_combobox->addItem("38400", BAUD38400);
    baudrate_combobox->addItem("57600", BAUD57600);
    baudrate_combobox->addItem("115200", BAUD115200);


    databits_combobox = new QComboBox(serial_window);
    databits_combobox->setGeometry(120, 85, 70, 20);
    databits_combobox->addItem("5", DATABITS_5);
    databits_combobox->addItem("6", DATABITS_6);
    databits_combobox->addItem("7", DATABITS_7);
    databits_combobox->addItem("8", DATABITS_8);

}

void C_serialPortGUI::createLabels(void){
    name  = new QLabel(serial_window);
    name->setGeometry(20, 20, 100, 10);
    name->setText(UI_LABEL_PORTNAME);

    baudrate = new QLabel(serial_window);
    baudrate ->setGeometry(20, 55, 100, 10);
    baudrate->setText(UI_LABEL_BAUDRATENAME);

    databits = new QLabel(serial_window);
    databits ->setGeometry(20, 90, 100, 10);
    databits->setText(UI_LABEL_DATABITS);
}


void C_serialPortGUI::createGroups(void){

    //STOP BITS
    stopBits_group = new QGroupBox(serial_window);
    stopBits_group->setGeometry(210, 10, 120, 95);
    stopBits_group->setTitle(UI_GROUPBOX_STOPBITS_NAME);
    stopBits_group->setStyleSheet("QGroupBox::title {subcontrol-position: top center;}");

    stopbits_one = new QRadioButton(serial_window);
    stopbits_one->setGeometry(230, 30, 90, 15 );
    stopbits_one->setText(UI_RADIO_STOP_1);
    stopbits_oneandhalf = new QRadioButton(serial_window);
    stopbits_oneandhalf->setGeometry(230, 50, 90, 15 );
    stopbits_oneandhalf->setText(UI_RADIO_STOP_15);
    stopbits_two = new QRadioButton(serial_window);
    stopbits_two->setGeometry(230, 70, 90, 15 );
    stopbits_two->setText(UI_RADIO_STOP_2);

    stopBits_vbox = new QVBoxLayout;

    stopBits_vbox->addWidget(stopbits_one);
    stopBits_vbox->addWidget(stopbits_oneandhalf);
    stopBits_vbox->addWidget(stopbits_two);
    stopBits_group->setLayout(stopBits_vbox);


    //FLOW CONTROL
    flowcontrol_group = new QGroupBox(serial_window);
    flowcontrol_group->setGeometry(340, 10, 120, 95);
    flowcontrol_group->setTitle(UI_GROUPBOX_FLOWCONTROL);
    flowcontrol_group->setStyleSheet("QGroupBox::title {subcontrol-position: top center;}");

    flowcontrol_no = new QRadioButton(serial_window);
    flowcontrol_no->setGeometry(360, 30, 90, 15);
    flowcontrol_no->setText(UI_RADIO_FLOW_NO);
    flowcontrol_hardware = new QRadioButton(serial_window);
    flowcontrol_hardware->setGeometry(360, 50, 90, 15);
    flowcontrol_hardware->setText(UI_RADIO_FLOW_HARDWARE);
    flowcontrol_software = new QRadioButton(serial_window);
    flowcontrol_software->setGeometry(360, 70, 90, 15);
    flowcontrol_software->setText(UI_RADIO_FLOW_SOFTWARE);

    flowcontrol_vbox = new QVBoxLayout;

    flowcontrol_vbox->addWidget(flowcontrol_no);
    flowcontrol_vbox->addWidget(flowcontrol_hardware);
    flowcontrol_vbox->addWidget(flowcontrol_software);
    flowcontrol_group->setLayout(flowcontrol_vbox);


    //PARITY

    parity_group = new QGroupBox(serial_window);
    parity_group->setGeometry(470, 10, 120, 95);
    parity_group->setTitle(UI_GROUPBOX_PARITY);
    parity_group->setStyleSheet("QGroupBox::title {subcontrol-position: top center;}");

    parity_no = new QRadioButton(serial_window);
    parity_no->setGeometry(490, 30, 90, 15);
    parity_no->setText(UI_RADIO_PARITY_NO);
    parity_even = new QRadioButton(serial_window);
    parity_even->setGeometry(490, 50, 90, 15);
    parity_even->setText(UI_RADIO_PARITY_EVEN);
    parity_odd = new QRadioButton(serial_window);
    parity_odd->setGeometry(490, 90, 90, 15);
    parity_odd->setText(UI_RADIO_PARITY_ODD);

    parity_vbox = new QVBoxLayout;

    parity_vbox->addWidget(parity_no);
    parity_vbox->addWidget(parity_even);
    parity_vbox->addWidget(parity_odd);
    parity_group->setLayout(parity_vbox);

}

void C_serialPortGUI::createPushButtons(void){

    Akcept = new QPushButton(serial_window);
    Akcept->setGeometry(100, 130, 90, 25 );
    Akcept->setText(UI_PUSHBUTON_AKCEPT);

    Cancel = new QPushButton(serial_window);
    Cancel->setGeometry(250, 130, 90, 25 );
    Cancel->setText(UI_PUSHBUTON_CANCEL);

    Default = new QPushButton(serial_window);
    Default->setGeometry(400, 130, 90, 25 );
    Default->setText(UI_PUSHBUTON_DEFAULT);
}

void C_serialPortGUI::getDataFromGUI(void)
{
    serialport_property.name = portname_combobox->currentText();

    switch(baudrate_combobox->currentIndex())                                //baudrate  QComboBox
    {
    case 0:
        serialport_property.serialport_baudrate = QSerialPort::Baud9600;
        break;
    case 1:
        serialport_property.serialport_baudrate = QSerialPort::Baud19200;
        break;
    case 2:
        serialport_property.serialport_baudrate = QSerialPort::Baud38400;
        break;
    case 3:
        serialport_property.serialport_baudrate = QSerialPort::Baud57600;
        break;
    case 4:
        serialport_property.serialport_baudrate = QSerialPort::Baud115200;
        break;
    default :
        serialport_property.serialport_baudrate = QSerialPort::Baud115200;

    }
    //databits  QComboBox
    switch(databits_combobox->currentIndex())
    {
    case 0:
        serialport_property.serialport_databits = QSerialPort::Data5;
        break;
    case 1:
        serialport_property.serialport_databits = QSerialPort::Data6;
        break;
    case 2:
        serialport_property.serialport_databits = QSerialPort::Data7;
        break;
    case 3:
        serialport_property.serialport_databits = QSerialPort::Data8;
        break;
    default:
        serialport_property.serialport_databits = QSerialPort::Data8;
    }


    if(stopbits_one->isChecked() )                                        //stop bits radiobuttons
        serialport_property.serialport_stopbits = QSerialPort::OneStop;

    else if(stopbits_oneandhalf->isChecked() )
        serialport_property.serialport_stopbits = QSerialPort::OneAndHalfStop;

    else if(stopbits_two->isChecked() )
        serialport_property.serialport_stopbits = QSerialPort::TwoStop;
    else
        serialport_property.serialport_stopbits  = QSerialPort::OneStop;

    if(flowcontrol_no->isChecked() )                                      //flowcontrol radiobuttons
        serialport_property.serialport_flowcontrol = QSerialPort::NoFlowControl;

    else if(flowcontrol_hardware->isChecked() )
        serialport_property.serialport_flowcontrol = QSerialPort::HardwareControl;

    else if(flowcontrol_software->isChecked() )
        serialport_property.serialport_flowcontrol = QSerialPort::SoftwareControl;
    else
        serialport_property.serialport_flowcontrol = QSerialPort::NoFlowControl;

    if(parity_no->isChecked() )                                           //parity radiobuttons
        serialport_property.serialport_parity = QSerialPort::NoParity;

    else if(parity_even->isChecked() )
        serialport_property.serialport_parity = QSerialPort::EvenParity;

    else if(parity_odd->isChecked() )
        serialport_property.serialport_parity = QSerialPort::OddParity;
    else
        serialport_property.serialport_parity = QSerialPort::NoParity;


}
SerialportPropertyStruct *C_serialPortGUI::returnSerialPrperty()
{
    return &serialport_property;
}

void C_serialPortGUI::on_Akcept_clicked(void){                              //portname

    qDebug() <<"Akcpet clicked"<< endl;
    getDataFromGUI();
    this->hideWindow();

}
void C_serialPortGUI::on_Cancel_clicked(void){
    qDebug() <<"Cancel clicked"<< endl;
    this->hideWindow();

}
void C_serialPortGUI::on_Default_clicked(void){
    qDebug() <<"Default clicked"<< endl;

    baudrate_combobox->setCurrentIndex(4);
    databits_combobox->setCurrentIndex(3);
    stopbits_one->setChecked(true);
    flowcontrol_no->setChecked(true);
    parity_no->setChecked(true);

}
