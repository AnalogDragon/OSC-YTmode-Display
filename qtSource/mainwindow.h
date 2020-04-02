#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QFileDialog>
#include <QFile>
#include <QImage>
#include <QPixmap>
#include <QDebug>
#include <QValidator>
#include <QTimer>

#include "clickedcombobox.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QImage Threshold_pro( QImage *image);
    void drawImage();
    void sengImage();
    void testesnd( QByteArray array )
    {
        int bitdata = 0;
        int bitcount = 0;
        for( int bit = 0; bit < array.length(); bit++ )
        {
            bitdata <<= 1;
            if( quint8(array.at(bit)) > 127 )
            {
                bitdata |= 0x01;
            }
            bitcount++;
            if( bitcount >= 8 )
            {
                qDebug()<<QString("%1").arg(bitdata%0x00ff,2,16,QLatin1Char('0'));
                bitcount = 0;
                bitdata = 0;
            }
        }
        if( bitcount != 0 )
        {
            bitdata <<= (8-bitcount);
            qDebug()<<QString("%1").arg(bitdata%0x00ff,2,16,QLatin1Char('0'));
        }
    }
private slots:
    void on_bn_Open_pressed();
    void on_bn_image_pressed();
    void on_bn_Front_pressed();
    void on_bn_next_pressed();
    void on_bn_Start_pressed();

    void on_cb_mode_currentIndexChanged(int index);

    void on_hs_Mono_valueChanged(int value);

private:
    Ui::MainWindow *ui;
    QSerialPort *_serial;

    QImage _SendImage;
    QPixmap _SendPixMap;
    QStringList imagefiles;
    bool _started = false;

    QTimer *_sendTimer;

    int ImageNumber;
    int _ThresholdValue = 127;
};

#endif // MAINWINDOW_H
