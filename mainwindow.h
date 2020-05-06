#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QUdpSocket>
#include <QWidgetItem>
#include <QListWidgetItem>

#include "message.h"
#include "sender.h"
#include <QTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    QUdpSocket soc;
    QMap<quint32, Sender> senders;
    QTimer timer;

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void sendMessage();
    void saveToFile();
    void loadFromFile();


private slots:
    void updateList();
    void resiveDatagramm();
    void on_btnSend_clicked();
    void on_lineEdit_returnPressed();
    void on_edtLogin_editingFinished();


    void on_btnClear_clicked();


private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
