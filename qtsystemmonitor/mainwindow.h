#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidgetItem>
#include <QVector>
#include <QDir>
#include <sys/types.h>
#include <pwd.h>

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
    QVector <Qproceso> procesos;
    passwd *psswd;
};

#endif // MAINWINDOW_H
