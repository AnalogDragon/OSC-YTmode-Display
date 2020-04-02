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
    QTimer *_scanSerialTimer;

    int ImageNumber;
    int _ThresholdValue = 127;
};

#endif // MAINWINDOW_H
