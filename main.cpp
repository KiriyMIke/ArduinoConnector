#include <QCoreApplication>
//#include <QtWidgets>

#include "arduino.h"
//#include "drawlines.h"

#include <thread>
#include <chrono>
#include <iostream>

void foo(Arduino &ar)
{
    std::cout << "start waiting\n";
    std::this_thread::sleep_for(std::chrono::seconds(2));
    emit ar.getPeriod();
    std::cout << "stop wait\n";
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Arduino arduino("/dev/cu.usbmodem1421");
    std::thread t1(foo, std::ref(arduino));
    t1.detach();
   // arduino.getPeriod();
    return a.exec();
}
