#ifndef MESSAGE_H
#define MESSAGE_H

#include <QString>
#include <QByteArray>

enum MessageType {
    mt_message = 1,
    mt_who_are_you,
    mt_i_am,
    mt_are_you_hear,
    mt_i_am_hear

};

class Message
{
    QString message;
    MessageType mt;

    void initIt();

public:
    Message();
    Message(QString, MessageType);

    void setMessage(QString);
    QString getMessage();

    void setMessageType(MessageType);
    MessageType getMessageType();

    QByteArray packMessage();
    static Message restoreMessage(QByteArray);
};

#endif // MESSAGE_H
