#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidgetItem>
#include <QVector>
#include <QDir>
#include <sys/types.h>
#include <pwd.h>
#include <QThread>
#include "hardwareh.h"
#include<QTreeWidget>
#include<QJsonDocument>
#include<QJsonObject>
#include<QJsonValue>

struct Qproceso{
    QString pid;
    QString lc;
    QString nombre;
    QString nhilos;
    int propietario;
};

namespace Ui {
class MainWindow;
}

class QTimer;
class Mihilo;
class Hardwareh;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    Qproceso RecogeProceso(QDir r);

 private slots:
    void recibedatos();
    void onTimeout();
    void hardwareprocces();

private:
    Ui::MainWindow *ui;
    Mihilo *mMihilo;
    int fila=0;
    int rowpro=0;
    QTableWidgetItem *entrada0;
    QTableWidgetItem *entrada1;
    QString entrada0text;
    QString entrada1text;
    QTimer *mTimer;
    QTimer *mTimer2;
    QVector <Qproceso> procesos;
    passwd *psswd;
    QThread hardware;
    Hardwareh ph;
    QJsonDocument jd;
    void addTreeRoot(QString name, QJsonDocument json);
    void addTreeChild(QTreeWidgetItem *parent, QString name, QJsonObject obj);
};

#endif // MAINWINDOW_H
