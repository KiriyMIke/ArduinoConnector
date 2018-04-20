#ifndef ARDUINO_H
#define ARDUINO_H

#include <QtSerialPort>
#include <QtCore>

#include <array>


class Arduino : public QObject
{
    Q_OBJECT

private:
    QSerialPort* _serial;

    // TODO: Use one response buffer
    QByteArray _answerCode;
    QByteArray _periodRaw;

    int _writeTimeout = 1000;

    // TODO: period struct
    uint32_t _period;

    // TODO: Use QByteArray
    char *_requestForPeriod;
    char *_requestForData;

    void sendPeriodRequest() const;

public:
    explicit Arduino(const QString &portName);

    static QString identPort();

    // TODO: Think, maybe it's private
    void writeData(QByteArray data) const;

signals:
    void getPeriod();
    void getData();

public slots:
    void read();

    void readPeriod() const;
    void readData() const;



};
#endif // ARDUINO_H
