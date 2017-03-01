#ifndef MIHILO_H
#define MIHILO_H

#include <QThread>
#include <QDir>
#include <QDebug>
#include <QString>
#include <QFileInfo>
#include <QPair>
#include <QQueue>
#include <QMutex>
#include <QFileInfoList>

class Mihilo : public QThread
{
    Q_OBJECT
public:
    Mihilo(int tiempo, QObject *parent = nullptr);
    void insertar(QPair<QString, QString> p);
    QPair<QString, QString> extraer();
    int devuelvetamano();
signals:
    void enviadatos();
protected:
    void run() override;
private:
    int mTiempo; //milisegundos
    QQueue< QPair<QString, QString> > queue;
    QMutex mutex;
    QFileInfoList vect_disp;

};

#endif // MIHILO_H
