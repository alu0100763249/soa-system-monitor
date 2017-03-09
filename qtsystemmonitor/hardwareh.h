#ifndef HARDWAREH_H
#define HARDWAREH_H

#include<QProcess>
#include<QByteArray>
#include<QDebug>

class Hardwareh : public QObject
{
    Q_OBJECT
public:
    explicit Hardwareh(QObject *parent = nullptr);
    void procesohard();
    QByteArray recibirhard();
signals:
    void envsignal();

public slots:

private:
    QByteArray ba;
};

#endif // HARDWAREH_H
