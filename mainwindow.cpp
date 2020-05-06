#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QDataStream>
#include <QFile>

#define FILENAME "data"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("Smartech Chat");

    soc.bind(8000);
    connect(&soc, SIGNAL(readyRead()),
            this, SLOT(resiveDatagramm()));

    connect(&timer, SIGNAL(timeout()),
            this, SLOT(updateList()));
    timer.start(10000);

//    soc.writeDatagram(Message("", mt_who_are_you).packMessage(),
//                      QHostAddress::Broadcast,
//                      8000);

    loadFromFile();
}

MainWindow::~MainWindow()
{
    delete ui;
    saveToFile();
}

void MainWindow::sendMessage()
{
    soc.writeDatagram(
                Message(ui->lineEdit->text(), mt_message).packMessage(),
                QHostAddress::Broadcast,
                8000);

    ui->lineEdit->clear();
}

void MainWindow::saveToFile()
{
    QFile f(FILENAME);
    f.open(QFile::WriteOnly | QFile::Truncate);
    QDataStream str(&f);

    str << ui->plainTextEdit->toPlainText();
    str << ui->edtLogin->text();

    f.close();
}

void MainWindow::loadFromFile()
{
    QFile f(FILENAME);
    if(!f.exists())return;
    f.open(QFile::ReadOnly);
    QDataStream str(&f);

    QString tmp;
    str >> tmp;
    ui->plainTextEdit->appendPlainText(tmp);

    str >> tmp;
    ui->edtLogin->setText(tmp);

    f.close();
}



void MainWindow::resiveDatagramm()
{
    while (soc.hasPendingDatagrams()) {
        int size = (int)soc.size();
        char* data = new char[size];
        QHostAddress *address = new QHostAddress();
        quint16 *port = new quint16();

        soc.readDatagram(data, size, address, port);

        Message m = Message::restoreMessage(QByteArray(data, size));

        if (m.getMessageType() == mt_message) {
            QString senderName = "Unknown";
            if (senders.contains(address->toIPv4Address())) {
                senderName = senders[address->toIPv4Address()].UserName;
            } else {
                Message mSend = Message("", mt_who_are_you);
                soc.writeDatagram(mSend.packMessage(), *address, *port);
            }
            ui->plainTextEdit->appendPlainText(senderName + ": " + m.getMessage());
        } else if (m.getMessageType() == mt_who_are_you) {
            Message mSend = Message(ui->edtLogin->text(), mt_i_am);
            soc.writeDatagram(mSend.packMessage(), *address, *port);
        } else if (m.getMessageType() == mt_i_am) {
            Sender s;
            s.UserName = m.getMessage();
            s.address = *address;
            s.port = *port;
            senders[address->toIPv4Address()] = s;
        } else if(m.getMessageType() == mt_are_you_hear){
            Message mSend = Message("", mt_i_am_hear);
            soc.writeDatagram(mSend.packMessage(), *address, *port);
        } else if(m.getMessageType() == mt_i_am_hear){

        }

        delete  address;
        delete  port;
    }
}

void MainWindow::on_btnSend_clicked()
{
    sendMessage();
}

void MainWindow::on_lineEdit_returnPressed()
{
    sendMessage();
}

void MainWindow::on_edtLogin_editingFinished()
{
    Message mSend = Message(ui->edtLogin->text(), mt_i_am);
    soc.writeDatagram(mSend.packMessage(), QHostAddress::Broadcast, 8000);
}



void MainWindow::on_btnClear_clicked()
{
    ui->plainTextEdit->clear();
}

void MainWindow::updateList()
{
    ui->listUsers->clear();

    foreach(quint32 i, senders.keys())
    {
        ui->listUsers->addItem(senders[i].UserName);
    }
}


