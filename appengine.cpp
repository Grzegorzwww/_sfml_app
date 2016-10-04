#include "appengine.h"

appEngine::appEngine(Ui::MainWindow *_ui, QObject *parent) :
    ui(_ui)
{
    qDebug() << "AppEngine";

    inclination_x = 0;
    inclination_y = 0;

    ui->progressBar->setMaximum(3464);
    ui->progressBar->setMinimum(2720);

    ui->progressBar_2->setMaximum(3550);
    ui->progressBar_2->setMinimum(2816);


     engine_window = new sdl_window(ui);

}

void appEngine::unfoldFrame(unsigned char *frame, int frame_size){


    _data_in.clear();
    int i;
    for(i = 0; i < frame_size; i++){
        _data_in.append(frame[i]);
    }

    inclination_x = (unsigned char)_data_in.at(INCLI_X_LSB);
    inclination_x |= (unsigned char)_data_in.at(INCLI_X_MSB ) << 8;


    inclination_y = (unsigned char)_data_in.at(INCLI_Y_LSB);
    inclination_y |= (unsigned char)_data_in.at(INCLI_Y_MSB ) << 8;

//    inclination_x &= (0x01FFC);
//    inclination_y &= (0x01FFC);

    inclination_x &= (0x01FF8);
    inclination_y &= (0x01FF8);

    ui->progressBar->setValue(inclination_x );
    ui->progressBar_2->setValue(inclination_y );

    ui->label_3->setText(QString::number(inclination_x, 10));
    ui->label_4->setText(QString::number(inclination_y, 10));

    ui->label_5->setText(QString::number(inclination_x, 2));
    ui->label_6->setText(QString::number(inclination_y, 2));

        ui->progressBar->setValue(inclination_x );
        ui->progressBar_2->setValue(inclination_y );



        /*for(i = 0; i < _data_in.size(); i++){
            qDebug() << QString::number(_data_in.at(i), 16);
        }
        qDebug() <<"-------------------------------------";
    */
}
