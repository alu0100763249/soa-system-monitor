#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mihilo.h"
#include <QTimer>
#include <QFuture>
#include <QtConcurrent/QtConcurrent>
#include "hardwareh.h"
#include <QVector>





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
    ui->tableWidget->setHorizontalHeaderLabels(QString("SENSOR; TEMPERATURA").split(";"));
    ui->tableWidget->setColumnWidth(0,200);
    ui->tableWidget->setColumnWidth(1,150);

    ui->tableWidget_2->setColumnCount(5);
    ui->tableWidget_2->setHorizontalHeaderLabels(QString("NOMBRE; PID; CMDLINE; HILOS; PROPIETARIO").split(";"));
    ui->tableWidget_2->setColumnWidth(0,200);
    ui->tableWidget_2->setColumnWidth(2,300);

    mTimer = new QTimer(this);
    connect(mTimer,SIGNAL(timeout()), this, SLOT(onTimeout()));
    mTimer->start(5000);

    ph.moveToThread(&hardware);
    connect(&ph, SIGNAL(envsignal()),this,SLOT(hardwareprocces()));
    ph.procesohard();

}



MainWindow::~MainWindow()
{
    delete ui;
}

Qproceso MainWindow::RecogeProceso(QDir r)
{
    Qproceso p;
    QFile mfile;
    QByteArray contenido;
    QFileInfoList files;


            files = r.entryInfoList(QDir::AllEntries | QDir::Hidden);
            for(auto archivos: files){
                   mfile.setFileName(archivos.absoluteFilePath());
                   mfile.open(QIODevice::ReadOnly | QIODevice::Text);
                   if(archivos.fileName()=="cmdline"){
                      contenido=mfile.readLine();
                      p.lc=contenido.trimmed();
                   }
                   else if (archivos.fileName()=="status"){

                    while(mfile.atEnd()){
                       while(mfile.readLine(6)!="Name:"){
                        mfile.readLine();
                       }
                       contenido=mfile.readLine();
                       p.nombre=contenido.trimmed();


                       while(mfile.readLine(5)!="Pid:"){
                        mfile.readLine();
                       }

                       contenido=mfile.readLine();
                       p.pid=contenido.trimmed();

                       while(mfile.readLine(5)!="Uid:"){
                         mfile.readLine();
                       }
                       contenido=mfile.readLine(6);
                       p.propietario=contenido.toInt();
                       mfile.readLine();

                       while(mfile.readLine(9)!="Threads:"){
                        mfile.readLine();
                       }
                           contenido=mfile.readLine();
                           p.nhilos=contenido.trimmed();



                    }
                 }

            mfile.close();
            }

            return p;
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

void MainWindow::onTimeout()
{

   QDir directorio;
   directorio.setPath("/proc");
   directorio.setNameFilters(QStringList()<<"1*"<<"2*"<<"3*"<<"4*"<<"5*"<<"6*"<<"7*"<<"8*"<<"9*"<<"status"<<"cmdline");
   QStringList fileList = directorio.entryList();

   for(auto entry: fileList) {
       QFutureWatcher <Qproceso> *wrp = new QFutureWatcher<Qproceso>;
       directorio.cd(entry);
       QFuture<Qproceso> rp = QtConcurrent::run(this,&MainWindow::RecogeProceso,directorio);
       wrp->setFuture(rp);

       connect(wrp, &QFutureWatcher<Qproceso>::finished,[=](){

          psswd=getpwuid(rp.result().propietario);
          if(psswd!=NULL){
           if(entrada1text!=""){

               if(rp.result().pid!=entrada1text){

                   if(ui->tableWidget_2->item(rowpro,0)==NULL){

                       ui->tableWidget_2->insertRow(rowpro);
                       ui->tableWidget_2->setItem(rowpro,0,new QTableWidgetItem(rp.result().nombre));
                       ui->tableWidget_2->setItem(rowpro,1,new QTableWidgetItem(rp.result().pid));
                       ui->tableWidget_2->setItem(rowpro,2,new QTableWidgetItem(rp.result().lc));
                       ui->tableWidget_2->setItem(rowpro,3,new QTableWidgetItem(rp.result().nhilos));
                       ui->tableWidget_2->setItem(rowpro,4,new QTableWidgetItem(psswd->pw_name));

                       rowpro++;
                   }
                   else  {
                       ui->tableWidget_2->setItem(rowpro,0,new QTableWidgetItem(rp.result().nombre));
                       ui->tableWidget_2->setItem(rowpro,1,new QTableWidgetItem(rp.result().pid));
                       ui->tableWidget_2->setItem(rowpro,2,new QTableWidgetItem(rp.result().lc));
                       ui->tableWidget_2->setItem(rowpro,3,new QTableWidgetItem(rp.result().nhilos));
                       ui->tableWidget_2->setItem(rowpro,4,new QTableWidgetItem(psswd->pw_name));
                       rowpro++;

                       }
               }
               else {

                   rowpro=0;
                   ui->tableWidget_2->setItem(rowpro,0,new QTableWidgetItem(rp.result().nombre));
                   ui->tableWidget_2->setItem(rowpro,1,new QTableWidgetItem(rp.result().pid));
                   ui->tableWidget_2->setItem(rowpro,2,new QTableWidgetItem(rp.result().lc));
                   ui->tableWidget_2->setItem(rowpro,3,new QTableWidgetItem(rp.result().nhilos));
                   ui->tableWidget_2->setItem(rowpro,4,new QTableWidgetItem(psswd->pw_name));

                   rowpro++;
               }
           }


           if(entrada1text==""){
               ui->tableWidget_2->insertRow(rowpro);
               ui->tableWidget_2->setItem(rowpro,0,new QTableWidgetItem(rp.result().nombre));
               ui->tableWidget_2->setItem(rowpro,1,new QTableWidgetItem(rp.result().pid));
               ui->tableWidget_2->setItem(rowpro,2,new QTableWidgetItem(rp.result().lc));
               ui->tableWidget_2->setItem(rowpro,3,new QTableWidgetItem(rp.result().nhilos));
               ui->tableWidget_2->setItem(rowpro,4,new QTableWidgetItem(psswd->pw_name));

               entrada1 = ui->tableWidget_2->item(0,1);
               entrada1text = entrada1->text();
               rowpro++;
           }

           wrp->waitForFinished();
           wrp->deleteLater();
        }

          else{
           psswd=getpwuid(1000);
           if(entrada1text!=""){

               if(rp.result().pid!=entrada1text){

                   if(ui->tableWidget_2->item(rowpro,0)==NULL){

                       ui->tableWidget_2->insertRow(rowpro);
                       ui->tableWidget_2->setItem(rowpro,0,new QTableWidgetItem(rp.result().nombre));
                       ui->tableWidget_2->setItem(rowpro,1,new QTableWidgetItem(rp.result().pid));
                       ui->tableWidget_2->setItem(rowpro,2,new QTableWidgetItem(rp.result().lc));
                       ui->tableWidget_2->setItem(rowpro,3,new QTableWidgetItem(rp.result().nhilos));
                       ui->tableWidget_2->setItem(rowpro,4,new QTableWidgetItem(psswd->pw_name));

                       rowpro++;
                   }
                   else  {
                       ui->tableWidget_2->setItem(rowpro,0,new QTableWidgetItem(rp.result().nombre));
                       ui->tableWidget_2->setItem(rowpro,1,new QTableWidgetItem(rp.result().pid));
                       ui->tableWidget_2->setItem(rowpro,2,new QTableWidgetItem(rp.result().lc));
                       ui->tableWidget_2->setItem(rowpro,3,new QTableWidgetItem(rp.result().nhilos));
                       ui->tableWidget_2->setItem(rowpro,4,new QTableWidgetItem(psswd->pw_name));
                       rowpro++;

                       }
               }
               else {

                   rowpro=0;
                   ui->tableWidget_2->setItem(rowpro,0,new QTableWidgetItem(rp.result().nombre));
                   ui->tableWidget_2->setItem(rowpro,1,new QTableWidgetItem(rp.result().pid));
                   ui->tableWidget_2->setItem(rowpro,2,new QTableWidgetItem(rp.result().lc));
                   ui->tableWidget_2->setItem(rowpro,3,new QTableWidgetItem(rp.result().nhilos));
                   ui->tableWidget_2->setItem(rowpro,4,new QTableWidgetItem(psswd->pw_name));

                   rowpro++;
               }
           }


           if(entrada1text==""){
               ui->tableWidget_2->insertRow(rowpro);
               ui->tableWidget_2->setItem(rowpro,0,new QTableWidgetItem(rp.result().nombre));
               ui->tableWidget_2->setItem(rowpro,1,new QTableWidgetItem(rp.result().pid));
               ui->tableWidget_2->setItem(rowpro,2,new QTableWidgetItem(rp.result().lc));
               ui->tableWidget_2->setItem(rowpro,3,new QTableWidgetItem(rp.result().nhilos));
               ui->tableWidget_2->setItem(rowpro,4,new QTableWidgetItem(psswd->pw_name));

               entrada1 = ui->tableWidget_2->item(0,1);
               entrada1text = entrada1->text();
               rowpro++;
           }

           wrp->waitForFinished();
           wrp->deleteLater();
        }
       });
      //qDebug()<<proceso.propietario<<proceso.pid<<proceso.nhilos<<proceso.lc;
       directorio.cd("..");


   }
   //RecogeProceso("/proc");
}

void MainWindow::hardwareprocces()
{

    jd=QJsonDocument::fromJson(ph.recibirhard());


    // Set the number of columns in the tree
        ui->treeWidget->setColumnCount(1);

        // Set the headers
        ui->treeWidget->setHeaderLabels(QStringList() << "Nombre");

        // Add root node
        addTreeRoot(jd.object().value("id").toString(),jd);

}

void MainWindow::addTreeRoot(QString name, QJsonDocument json)
{
    // QTreeWidgetItem(QTreeWidget * parent, int type = Type)
        QTreeWidgetItem *treeItem = new QTreeWidgetItem(ui->treeWidget);

        // QTreeWidgetItem::setText(int column, const QString & text)
        treeItem->setText(0, name);
        QJsonArray v= json.object().value("children").toArray();
        for (int i=0; i<=v.size();i++){
         addTreeChild(treeItem, v[i].toObject().value("id").toString(),v[i].toObject());
         //addTreeChild(treeItem, v[i].toObject().value("id").toString());
        }

}

void MainWindow::addTreeChild(QTreeWidgetItem *parent, QString name, QJsonObject obj)
{
    // QTreeWidgetItem(QTreeWidget * parent, int type = Type)
        QTreeWidgetItem *treeItem = new QTreeWidgetItem();

        // QTreeWidgetItem::setText(int column, const QString & text)
        treeItem->setText(0, name);
        QJsonArray v= obj.value("children").toArray();
        if(!v.isEmpty())
            for (int i=0; i<=v.size();i++){

             addTreeChild(treeItem, v[i].toObject().value("id").toString(),v[i].toObject());
             //addTreeChild(treeItem, v[i].toObject().value("id").toString());
            }

        // QTreeWidgetItem::addChild(QTreeWidgetItem * child)
        parent->addChild(treeItem);

}







