#ifndef SERIAL_PORT_H
#define SERIAL_PORT_H



#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QString>
#include <QComboBox>
#include <QWindow>
#include <QWidget>
#include <QDialog>
#include <QPushButton>
#include <QGroupBox>
#include <QRadioButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QObject>
#include <QByteArray>



#define MAX_AVAILABLEPORTS 10

#define BAUD9600 9600
#define BAUD19200 19200
#define BAUD38400 38400
#define BAUD57600 57600
#define BAUD115200 115200
#define DATABITS_5 5
#define DATABITS_6 6
#define DATABITS_7 7
#define DATABITS_8 8

#define PARITY_NO 0
#define PARITY_EVEN 2
#define PARITY_ODD 3
#define PARITY_SPACE 4
#define PARITY_MARK 5

#define STOPBITS_ONE 1
#define STOPBITS_ONEANDHALF 3
#define STOPBITS_TWO 2

#define FLOWCONTROL_NO 0
#define FLOWCONTROL_HARDWARE 1
#define FLOWCONTROL_SOFTWARE 2


#define UI_LABEL_PORTNAME     "         Numer Portu:"
#define UI_LABEL_BAUDRATENAME "Predkosc transmisi:"
#define UI_LABEL_DATABITS     "            Ilosc bitów:"

#define UI_GROUPBOX_STOPBITS_NAME "Bity stopu"
#define UI_GROUPBOX_FLOWCONTROL "Ster. przeplywem"
#define UI_GROUPBOX_PARITY "Kontr. parzyst."

#define UI_PUSHBUTON_AKCEPT "Akcepuj"
#define UI_PUSHBUTON_CANCEL "Anuluj"
#define UI_PUSHBUTON_DEFAULT "Ust. Fabryczne"

#define UI_RADIO_STOP_1 "Jeden"
#define UI_RADIO_STOP_15 "Póltora"
#define UI_RADIO_STOP_2 "Dwa"

#define UI_RADIO_FLOW_NO "Brak"
#define UI_RADIO_FLOW_HARDWARE "Sprzetowo"
#define UI_RADIO_FLOW_SOFTWARE "Programowo"

#define UI_RADIO_PARITY_NO "Brak"
#define UI_RADIO_PARITY_EVEN "even"
#define UI_RADIO_PARITY_ODD "odd"


//typedef unsigned char uint8_t;
//typedef unsigned short uint16_t;
//typedef unsigned int uint32_t;



 struct SerialportPropertyStruct {

    QString name;
    QSerialPort::BaudRate serialport_baudrate;
    QSerialPort::DataBits serialport_databits;
    QSerialPort::Parity serialport_parity;
    QSerialPort::StopBits serialport_stopbits;
    QSerialPort::FlowControl serialport_flowcontrol;
};


class C_serialPort : public QObject {
    Q_OBJECT


public:

    //variables
    QSerialPort *serial_port;


    bool serialport_isopen;
    int number_of_availableports;
    QString  serialport_availableports[MAX_AVAILABLEPORTS];


    //functions
    C_serialPort(QObject *parent);
    C_serialPort();
    ~C_serialPort();

    void openSerialPort_Default(const char *portname);
    int getAllAvailablePorts();


    void setSerialBaud(QSerialPort::BaudRate baud);
    void setSerialDataBits(QSerialPort::DataBits databits);
    void setSerialStopBits( QSerialPort::StopBits stopbits);
    void setSerialParity( QSerialPort::Parity parity);
    void setSerialFlowControl(QSerialPort::FlowControl flowcontrol);
    void openSerialPort_FromGui(SerialportPropertyStruct *serialProperty,  QObject *parent);
    QSerialPort *returnSerialPortPointer(void);
    QSerialPortInfo returnSerialInfo(void);
    QByteArray *getData(void);




private:

    //variables

    QSerialPortInfo info_serialport;
    bool ready_to_close;

public slots:

     void closeSerialPort();

    //functions

signals:
     void serial_port_closing_signal(void);


};

/*-----------------------------------------class C_serialPortGUI-----------------------------------------*/
class C_serialPortGUI : public QObject {
    Q_OBJECT


public:

    QDialog *serial_window;
    struct SerialportPropertyStruct serialport_property;


    C_serialPortGUI(QString *port);
    ~C_serialPortGUI();
    void createSerialConfigWindow(QString *ports);
    void getDataFromGUI(void);

    SerialportPropertyStruct* returnSerialPrperty();

private:



    QPushButton *Akcept;        //create GUI objects
    QPushButton *Cancel;
    QPushButton *Default;

    QLabel *name;
    QLabel *baudrate;
    QLabel *databits;

    QComboBox *portname_combobox;
    QComboBox *baudrate_combobox;
    QComboBox *databits_combobox;

    QGroupBox *stopBits_group;
    QGroupBox *parity_group;
    QGroupBox *flowcontrol_group;

    QVBoxLayout *stopBits_vbox;
    QVBoxLayout *parity_vbox;
    QVBoxLayout *flowcontrol_vbox;

    QRadioButton *stopbits_one;
    QRadioButton *stopbits_two;
    QRadioButton *stopbits_oneandhalf;

    QRadioButton *flowcontrol_no;
    QRadioButton *flowcontrol_hardware;
    QRadioButton *flowcontrol_software;

    QRadioButton *parity_no;
    QRadioButton *parity_even;
    QRadioButton *parity_odd;


    void createCompoBoxs(QString *ports);
    void createLabels(void);
    void createGroups(void );
    void createPushButtons(void);


    public slots:

    void hideWindow(void);
    void showWindow(bool);

    void on_Akcept_clicked(void);
    void on_Cancel_clicked(void);
    void on_Default_clicked(void);


};


#endif // SERIAL_PORT_H
