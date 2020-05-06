#include "message.h"
#include <QDataStream>

void Message::initIt()
{
    message = "";
    mt = mt_message;
}

Message::Message()
{
    initIt();
}

Message::Message(QString message, MessageType mt)
{
    this->message = message;
    this->mt = mt;
}

void Message::setMessage(QString message)
{
    this->message = message;
}

QString Message::getMessage()
{
    return message;
}

void Message::setMessageType(MessageType mt)
{
    this->mt = mt;
}

MessageType Message::getMessageType()
{
    return mt;
}

QByteArray Message::packMessage()
{
    QByteArray arr;
    QDataStream stream(&arr, QIODevice::WriteOnly);

    stream << message;
    stream << mt;

    return arr;
}

Message Message::restoreMessage(QByteArray arr)
{
    QDataStream stream(arr);

    QString message;
    int i_mt;
    MessageType mt;

    stream >> message;
    stream >> i_mt;

    mt = (MessageType)i_mt;

    return Message(message, mt);
}
