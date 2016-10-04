#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "serial_port.h"
#include <QDebug>
#include "appengine.h"
#include <collect_data.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


    appEngine *engine;

private:
    Ui::MainWindow *ui;

    QSerialPort *serial;
    C_serialPort *serialPort;
    C_serialPortGUI *serialPortGUI;
    SerialportPropertyStruct serialProperty;
    SerialportPropertyStruct *serialProperty_pointer = &serialProperty;
    CollectData *dataParser;






public slots:
     void getSerialConfig(bool);
     void showIncomingFrame(void);
     void showOutgoingFrame(void);
     void procedureFrameUnfold(void);

};

#endif // MAINWINDOW_H
