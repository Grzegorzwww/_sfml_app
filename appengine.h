#ifndef APPENGINE_H
#define APPENGINE_H


#include <QObject>
#include <QDebug>
#include <QString>
#include <QLinkedList>
#include "ui_mainwindow.h"
#include "sdl_window.h"

#define INCLI_X_LSB 5
#define INCLI_X_MSB 4

#define INCLI_Y_LSB 7
#define INCLI_Y_MSB 6

class appEngine : public QObject
{
    Q_OBJECT
public:
    explicit appEngine(Ui::MainWindow *_ui, QObject *parent = 0);


     Ui::MainWindow *ui;
     sdl_window *engine_window;

private:


    QByteArray _data_in;
    unsigned short inclination_x;
    unsigned short inclination_y;

signals:

public slots:

     void unfoldFrame(unsigned char *frame, int frame_size);
};

#endif // APPENGINE_H
