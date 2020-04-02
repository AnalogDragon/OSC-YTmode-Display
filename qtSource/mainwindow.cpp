#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    _serial = new QSerialPort();

    connect(ui->cb_serial,&clickedCombobox::clicked,[=](bool){
        ui->cb_serial->clear();
        QList<QSerialPortInfo>  infos = QSerialPortInfo::availablePorts();
        if(infos.isEmpty())
        {
            ui->cb_serial->addItem("无串口");
            return;
        }
        foreach (QSerialPortInfo info, infos)
        {
            ui->cb_serial->addItem(info.portName());
        }
    });

    ui->cb_serial->clear();
    QList<QSerialPortInfo>  infos = QSerialPortInfo::availablePorts();
    if(infos.isEmpty())
    {
        ui->cb_serial->addItem("无串口");
        return;
    }
    foreach (QSerialPortInfo info, infos)
    {
        ui->cb_serial->addItem(info.portName());
    }

    ui->cb_mode->clear();
    ui->cb_mode->addItem("RGB");
    ui->cb_mode->addItem("Gray");
    ui->cb_mode->addItem("Mono");

    ui->le_time->setValidator(new QIntValidator(5, 1000, this));

    _sendTimer = new QTimer();
    connect(_sendTimer,&QTimer::timeout,[=](){
        drawImage();
        sengImage();
        ImageNumber++;
        if( ImageNumber >= imagefiles.size())
        {
            _sendTimer->stop();
            ui->bn_Start->setText("START");
            _started = false;
            ImageNumber = 0;
            drawImage();
        }
    });

    ui->bn_Start->setEnabled(false);
    ui->bn_Front->setEnabled(false);
    ui->bn_next->setEnabled(false);
    ui->hs_Mono->setEnabled(false);
}
MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::drawImage()
{
    QImage *image = new QImage;
    if( imagefiles.isEmpty()) return;
    image->load(imagefiles.at(ImageNumber));
    _SendImage = image->scaled(ui->label->width(),ui->label->height(),Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    _SendPixMap = QPixmap::fromImage(Threshold_pro(&_SendImage));

    if( ui->cb_mode->currentIndex() != 0 )
    {
        _SendPixMap = QPixmap::fromImage(Threshold_pro(&_SendImage));
    }
    else
    {
        _SendPixMap = QPixmap::fromImage(_SendImage);
    }

    ui->label->setPixmap(_SendPixMap);
    ui->lb_info->setText(QString("%1/%2").arg(ImageNumber + 1).arg(imagefiles.size()));
}

void MainWindow::sengImage()
{
    if( !_serial->isOpen()) return;
    _SendImage = _SendImage.scaled(200,150,Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    QByteArray sendarray;
    sendarray.append(char(0xAA));
    char bitdata = 0;
    int countX = _SendImage.width();
    int xpos = 0;

    countX = ( countX % 8 != 0 )? (countX / 8 + 1) : (countX / 8);

    for(int x = 0; x < countX; x++)
    {
        for(int y = 0; y < _SendImage.height(); y++ )
        {
            for( int bit = 0; bit < 8; bit++ )
            {
                xpos = x * 8 + bit;
                bitdata <<= 1;
                if( xpos < _SendImage.width())
                {

                    int Gray = qGray(QRgb(_SendImage.pixel(xpos,y)));
                    if( Gray < _ThresholdValue )
                    {
                        bitdata |= 0x01;
                    }
                }
            }
            sendarray.append(bitdata);
            bitdata = 0;
        }
    }

    for( int i = 0; i < 10; i++ )
    {
        sendarray.append(char(0xee));
    }

    if( _serial->isOpen())
    {
        _serial->write(sendarray);
    }
}

void MainWindow::on_bn_Open_pressed()
{
    if( _serial->isOpen())
    {
        _serial->close();
        ui->bn_Open->setText("OPEN");
        ui->bn_Start->setEnabled(false);
    }
    else
    {
        _serial->setPortName(ui->cb_serial->currentText());
        _serial->setBaudRate(1500000,QSerialPort::AllDirections);//设置波特率和读写方向
        _serial->setDataBits(QSerialPort::Data8);                               //数据位为8位
        _serial->setFlowControl(QSerialPort::NoFlowControl);                    //无流控制
        _serial->setParity(QSerialPort::NoParity);                              //无校验位
        _serial->setStopBits(QSerialPort::OneStop);
        _serial->open(QIODevice::ReadWrite);

        ui->bn_Open->setText("CLOSE");
        ui->bn_Start->setEnabled(true);
    }
}

void MainWindow::on_bn_image_pressed()
{
    imagefiles.clear();
    imagefiles = QFileDialog::getOpenFileNames(
                this,
                "open",
                "../",
                "image(*.bmp *.jpeg *jpg)");


    for(int i = 0; i < imagefiles.size(); i++)
    {
        qDebug()<<imagefiles.at(i);
    }
    ImageNumber = 0;
    if( !imagefiles.isEmpty())
    {
        ui->lb_info->setText(QString("%1/%2").arg(ImageNumber + 1).arg(imagefiles.size()));
        if(imagefiles.size()<=1)
        {
            ui->bn_Front->setEnabled(false);
            ui->bn_next->setEnabled(false);
        }
        else
        {
            ui->bn_Front->setEnabled(false);
            ui->bn_next->setEnabled(true);
        }
        drawImage();
    }
    else
    {
        ui->bn_Front->setEnabled(false);
        ui->bn_next->setEnabled(false);
    }
}

void MainWindow::on_bn_Front_pressed()
{
    ImageNumber--;
    ui->bn_next->setEnabled(true);
    ui->bn_Front->setEnabled(true);
    if( ImageNumber == 0 )
    {
        ui->bn_Front->setEnabled(false);
    }
    else if((ImageNumber + 1) >= imagefiles.size())
    {
        ui->bn_next->setEnabled(false);
    }

    drawImage();
}

void MainWindow::on_bn_next_pressed()
{
    ImageNumber++;
    ui->bn_next->setEnabled(true);
    ui->bn_Front->setEnabled(true);

    if((ImageNumber + 1) >= imagefiles.size())
    {
        ui->bn_next->setEnabled(false);
    }
    else if( ImageNumber == 0 )
    {
        ui->bn_Front->setEnabled(false);
    }

    drawImage();
}

void MainWindow::on_bn_Start_pressed()
{
    if( _started )
    {
        ui->bn_Start->setText("START");
        _started = false;
        _sendTimer->stop();
    }
    else
    {
        ui->bn_Start->setText("STOP");
        _started = true;
        _sendTimer->start(ui->le_time->text().toInt());
    }
}

QImage MainWindow::Threshold_pro( QImage *image)
{
    int w = image->width();
    int h = image->height();

    QImage *newimage = new QImage(w,h,QImage::Format_RGB888);

    QRgb rgbVal = 0;
    int grayVal = 0;

    for( int y = 0 ; y < h; y++)
    {
        for( int x = 0; x < w; x++)
        {
            rgbVal = image->pixel(x,y);
            grayVal = qGray(rgbVal);
            if(ui->cb_mode->currentIndex() == 2 )
                grayVal = grayVal < _ThresholdValue ? 0 : 255;
            newimage->setPixel(x,y,QColor(grayVal,grayVal,grayVal).rgb());
        }
    }
    return *newimage;
}

void MainWindow::on_cb_mode_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    if( index == 2 )
    {
        ui->hs_Mono->setEnabled(true);
    }
    else
    {
        ui->hs_Mono->setEnabled(false);
    }
    drawImage();
}

void MainWindow::on_hs_Mono_valueChanged(int value)
{
    _ThresholdValue = value;
    drawImage();
}
