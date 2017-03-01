#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidgetItem>

namespace Ui {
class MainWindow;
}

class Mihilo;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

 private slots:
    void recibedatos();

private:
    Ui::MainWindow *ui;
    Mihilo *mMihilo;
    int fila=0;
    QTableWidgetItem *entrada0;
    QString entrada0text;
};

#endif // MAINWINDOW_H
