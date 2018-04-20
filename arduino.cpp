#include <iostream>

#include <QSerialPort>

#include "arduino.h"

// FIXME: Improve error handling
Arduino::Arduino(const QString &portName)
    : _serial(new QSerialPort(portName)),
      _requestForPeriod(new char[5] { 8, 0, 0, 0, 0 }),
      _requestForData(new char[5] { 1, 0, 0, 0, 0 })
{
    _serial->open(QSerialPort::ReadWrite);
    _serial->setBaudRate(QSerialPort::Baud9600);
    _serial->setDataBits(QSerialPort::Data8);
    _serial->setParity(QSerialPort::NoParity);
    _serial->setStopBits(QSerialPort::OneStop);
    _serial->setFlowControl(QSerialPort::NoFlowControl);

    if (_serial->isOpen() && _serial->isWritable() && _serial->isReadable())
    {
        std::cout << "Opened successfully!\n";
    }
    else
    {
        std::cout << "Port wasn't opened!\n";
    }


    connect(this, &Arduino::getPeriod, this, &Arduino::readPeriod);
    connect(this, &Arduino::getData, this, &Arduino::readData);
    connect(_serial, &QSerialPort::readyRead, this, &Arduino::read);
}

QString Arduino::identPort()
{
    std::cout << "Availible COM ports\n\n";

    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        std::cout << "Name: " << info.portName().toStdString() << '\n';
        std::cout << "Description: " << info.description().toStdString() << '\n';
        std::cout << "Manufacturer: " << info.manufacturer().toStdString() << "\n\n";
    }

    std::cout << "Enter number of the COM port: ";
    std::string tmpStr;
    std::getline(std::cin, tmpStr);
    tmpStr = "COM" + tmpStr;

    return QString(tmpStr.c_str());
}

uint32_t dataToUint32(QByteArray data)
{
    return ((uint32_t)(uint8_t)data[0]) 
        + (((uint32_t)(uint8_t)data[1]) << 8)
        + (((uint32_t)(uint8_t)data[2]) << 16) 
        + (((uint32_t)(uint8_t)data[3]) << 24);
}

void Arduino::readPeriodResponce()
{
    // FIXME: Wait for ready read + available bytes >= struct size
    std::cout << "kuku\n";
    // TODO: Read to one buffer then split and map to one struct
    _answerCode = _serial->read(1);
    _periodRaw = _serial->read(4);

    _period = dataToUint32(_periodRaw);

    // TODO: std::cout << "Answer: " << parsedStruct '\n';
    std::cout << "Answer: " << _answerCode[0] + '0' - 48 << '\n';
    std::cout << "Period = " << _period << '\n';
}

void Arduino::writeData(const QByteArray data) const
{
    if (_serial->isOpen() && _serial->isWritable())
    {
        auto bytesSent = _serial->write(data);
        std::cout << "Bytes send: " << bytesSent << std::endl;
        _serial->waitForBytesWritten(_writeTimeout);
    }
    else
    {
        std::cout << "Port wasn't opened!\n";
    }
}

void Arduino::sendPeriodRequest() const
{
    writeData(QByteArray(_requestForPeriod, 5));
}

void Arduino::readPeriod() const
{
    sendPeriodRequest();
    readPeriodResponce();
    // Теперь здесь мне надо вызвать функцию, которая распарсит данные, которые пришли в порт.
    // Как это сделать? Я же не знаю в данном моменте, считались ли уже данные или нет,
    // так как за чтение отвечает функция
}

void Arduino::readData() const
{
    sendPeriodRequest();
    readPeriodResponce();
    // Та же проблема. Здесь должна будет вызвана уже другая функция, в отличие от предыдущего случая.
}
