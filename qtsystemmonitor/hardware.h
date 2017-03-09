#ifndef HARDWARE_H
#define HARDWARE_H



class Hardware : public QObject
{
    Q_OBJECT
public:
    Hardware();
    ~Hardware();
    void procesohard();
/*public slots:
    void procesohard();*/


};

#endif // HARDWARE_H
