#include "hardwareh.h"

Hardwareh::Hardwareh(QObject *parent) : QObject(parent)
{

}

void Hardwareh::procesohard()
{
    QProcess mProcess;
    mProcess.start("lshw -json");
    mProcess.waitForFinished();
    mProcess.waitForReadyRead();
    ba = mProcess.readAllStandardOutput();
    emit envsignal();
}

QByteArray Hardwareh::recibirhard()
{
    return ba;
}


