#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mythread.h"
#include <Qprocess>
#include <QString>
#include <Qstringlist>
#include <fstream>
#include <QFile>

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //查找可用的串口
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        QSerialPort serial;
        serial.setPort(info);
        if(serial.open(QIODevice::ReadWrite))
        {
            ui->PortBox->addItem(serial.portName());
            serial.close();
        }
    }
    //设置波特率下拉菜单默认显示第三项
    ui->BaudBox->setCurrentIndex(5);
    //关闭发送按钮的使能
    ui->sendButton->setEnabled(false);
    //qDebug("界面设定成功！");
}

MainWindow::~MainWindow()
{
    delete ui;
}

//清空接受窗口
void MainWindow::on_clearButton_clicked()
{
    ui->textEdit->clear();
}

//发送数据
void MainWindow::on_sendButton_clicked()
{
    serial->write("0x0\r\n");
}

//读取接收到的数据
void MainWindow::Read_Data()
{
    QByteArray buf;
    buf = serial->readAll();
    ofstream fout;
    QString str;
    fout.open("imagedata.txt");
    //QFile f("imagedata.txt");
    //f.open(QIODevice::Append);
    //QDataStream s(&f);
    if(!buf.isEmpty())
    {
        str = ui->textEdit->toPlainText();
        if (tr(buf)=="Connected!\r\n") {
            ui->label_ifc->setText("Connected!");
            str+=tr(buf);
            ui->textEdit->clear();
            ui->textEdit->append(str);
        }
        else if(tr(buf)=="IDS\r\n"){
            str+=tr("ImageInputing...");
            ui->textEdit->clear();
            ui->textEdit->append(str);
        }
        else if(tr(buf)=="IDE\r\n"){
            fout << imageBuffer.toStdString();
            fout<<flush;
            fout.close();
            str+=tr("ImageDataInputed!\n");
            ui->textEdit->clear();
            ui->textEdit->append(str);

            QProcess *data2image = new QProcess;
            QString startProgram = "D:\\GraduationProject\\txt2jpeg\\txt2jpeg\\Debug\\txt2jpeg.exe";
            data2image->start(startProgram);
            data2image->waitForFinished();
            str+=tr("DataToImage Completed!\n");
            ui->textEdit->clear();
            ui->textEdit->append(str);

            str+=tr("Plate Recognizing\n");
            ui->textEdit->clear();
            ui->textEdit->append(str);

            QProcess *RecogP = new QProcess;
            startProgram = "D:\\GraduationProject\\OpenCVTest_VS2015_CV3.4H\\OpenCVTest\\x64\\Debug\\OpenCVTest.exe";
            QStringList arguments;
            arguments << "D:\\GraduationProject\\QTtest\\SerialPort\\SerialPort\\build-SerialPort-Desktop_Qt_5_8_0_MSVC2015_64bit-Debug\\out.jpg";
            RecogP->start(startProgram, arguments);
            RecogP->waitForFinished();
            ifstream fin;
            char PN[8];
            fin.open("D:\\GraduationProject\\QTtest\\SerialPort\\SerialPort\\build-SerialPort-Desktop_Qt_5_8_0_MSVC2015_64bit-Debug\\LicencePlateNum.txt");
            fin.getline(PN, 8);
            str+=tr(PN);
            ui->textEdit->clear();
            ui->textEdit->append(str);

            ifstream PIR;
            string pir;
            string PN1;
            bool Found=false;
            PN1 = PN;
            PIR.open("D:\\GraduationProject\\QTtest\\SerialPort\\SerialPort\\build-SerialPort-Desktop_Qt_5_8_0_MSVC2015_64bit-Debug\\PlatesInRecord.txt");
            while (getline (PIR,pir)){
                if (PN1==pir){
                    serial->write("0x1\r\n");
                    Found=true;
                }
            }
            if (Found==false) serial->write("0x2\r\n");

        }
        else {
            imageBuffer+=tr(buf);
        }
    }
    buf.clear();
}

/*

        QByteArray test_buf;
        test_buf = serial->readAll();
        QString test_str;
        test_str = tr(test_buf);
        if (test_str=="OKOK")   ui->label_ifc->setText("onnected!");

*/

void MainWindow::on_openButton_clicked()
{
    if(ui->openButton->text()==tr("打开串口"))
    {
        serial = new QSerialPort;
        //设置串口名
        serial->setPortName(ui->PortBox->currentText());
        //打开串口
        serial->open(QIODevice::ReadWrite);
        //设置波特率
        serial->setBaudRate(ui->BaudBox->currentText().toInt());
        //设置数据位数
        switch(ui->BitNumBox->currentIndex())
        {
        case 8: serial->setDataBits(QSerialPort::Data8); break;
        default: break;
        }
        //设置奇偶校验
        switch(ui->ParityBox->currentIndex())
        {
        case 0: serial->setParity(QSerialPort::NoParity); break;
        default: break;
        }
        //设置停止位
        switch(ui->StopBox->currentIndex())
        {
        case 1: serial->setStopBits(QSerialPort::OneStop); break;
        case 2: serial->setStopBits(QSerialPort::TwoStop); break;
        default: break;
        }
        //设置流控制
        serial->setFlowControl(QSerialPort::NoFlowControl);

        //关闭设置菜单使能
        ui->PortBox->setEnabled(false);
        ui->BaudBox->setEnabled(false);
        ui->BitNumBox->setEnabled(false);
        ui->ParityBox->setEnabled(false);
        ui->StopBox->setEnabled(false);
        ui->openButton->setText(tr("关闭串口"));
        ui->sendButton->setEnabled(true);

        //连接信号槽
        QObject::connect(serial, &QSerialPort::readyRead, this, &MainWindow::Read_Data);
        serial->write("0x0\r\n");

    }
    else
    {
        //关闭串口
        serial->clear();
        serial->close();
        serial->deleteLater();

        //恢复设置使能
        ui->PortBox->setEnabled(true);
        ui->BaudBox->setEnabled(true);
        ui->BitNumBox->setEnabled(true);
        ui->ParityBox->setEnabled(true);
        ui->StopBox->setEnabled(true);
        ui->openButton->setText(tr("打开串口"));
        ui->sendButton->setEnabled(false);
        ui->label_ifc->setText("Not Connected");
    }
}
