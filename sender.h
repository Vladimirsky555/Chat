#ifndef SENDER_H
#define SENDER_H

#include <QHostAddress>
#include <QString>

class Sender
{
public:
    QHostAddress address;
    quint16 port;
    QString UserName;

    Sender();
};

#endif // SENDER_H
