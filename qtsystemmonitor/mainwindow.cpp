#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mihilo.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mMihilo = new Mihilo(10,this);
    mMihilo->start();

    connect(mMihilo, SIGNAL(enviadatos()),this, SLOT(recibedatos()));

    //ui->tableWidget->setRowCount(12);
    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->setColumnWidth(0,200);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::recibedatos()
{
    QPair<QString, QString> p;
    p=mMihilo->extraer();
    QString cadena;

    if(entrada0text!=""){

        if(p.second!=entrada0text){

            if(ui->tableWidget->item(fila,0)==NULL){

                ui->tableWidget->insertRow(fila);
                ui->tableWidget->setItem(fila,0,new QTableWidgetItem(p.first));
                ui->tableWidget->setItem(fila,1,new QTableWidgetItem(p.second));
                               fila++;
            }
            else  {
                    ui->tableWidget->setItem(fila,0,new QTableWidgetItem(p.first));
                    ui->tableWidget->setItem(fila,1,new QTableWidgetItem(p.second));
                }
                fila++;
        }
        else {

            fila=0;
            ui->tableWidget->setItem(fila,0,new QTableWidgetItem(p.first));
            ui->tableWidget->setItem(fila,1,new QTableWidgetItem(p.second));
            fila++;
        }
    }


    if(entrada0text==""){
        ui->tableWidget->insertRow(fila);
        ui->tableWidget->setItem(fila,0,new QTableWidgetItem(p.first));
        ui->tableWidget->setItem(fila,1,new QTableWidgetItem(p.second));
        entrada0 = ui->tableWidget->item(0,1);
        entrada0text = entrada0->text();
        fila++;
    }



}





