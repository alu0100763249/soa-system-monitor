#include "mihilo.h"

Mihilo::Mihilo(int tiempo, QObject *parent)
    : QThread(parent)
{
    mTiempo = tiempo;

}

void Mihilo::insertar(QPair <QString, QString> p)
{
    QMutexLocker locker(&mutex);
    queue.enqueue(p);
    emit enviadatos();
    //QFuture<Datos> = QConcurrent::run(procesa, pid);

}

QPair<QString, QString> Mihilo::extraer()
{
   QMutexLocker locker(&mutex);
   return queue.dequeue();
}

int Mihilo::devuelvetamano()
{
    return vect_disp.size();
}





void Mihilo::run()
{
    QPair<QString, QString> p;
    QFile mfile;
    QByteArray contenido;
    QDir directorio;
    directorio.setPath("/sys/class/hwmon");
    directorio.setNameFilters(QStringList()<<"hwmon*"<<"temp*"<<"name");
    QStringList fileList = directorio.entryList();
    QFileInfoList files;
    while(true){
        for(auto entry: fileList) {
           directorio.cd(entry);
           files = directorio.entryInfoList(QDir::AllEntries | QDir::Hidden);
           vect_disp=files;
           for(auto archivos: files){

               mfile.setFileName(archivos.absoluteFilePath());
               mfile.open(QIODevice::ReadOnly | QIODevice::Text);
               contenido=mfile.readAll();
               p.first=archivos.fileName();
               p.second=contenido.trimmed();
               insertar(p);
               mfile.close();
               msleep(mTiempo);
            }
           directorio.cd("..");
        }
    }
}



