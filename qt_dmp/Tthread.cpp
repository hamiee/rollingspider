#include "Tthread.h"

Tthread::Tthread()
{

}

Tthread::~Tthread()
{

}


void Tthread::run()
{
    qDebug()<<"thread run";
    if(cmd==0){  //이 륙 회 전 전 진  착 륙
        socket->write(QString("takeoff\n").toUtf8());
        socket->waitForBytesWritten(1000);

        QThread::msleep(3000);


    #if 1
        socket->write(QString("l_turn\n").toUtf8());
        //socket->waitForBytesWritten(1000);
        socket->write(QString("1\n").toUtf8());

        socket->write(QString("50\n").toUtf8());

        socket->waitForBytesWritten(1000);


        while(yaw<target_yaw);


        //QThread::msleep(1000);

        //ui->lineEdit_YAW->text().toDouble();

        socket->write(QString("l_turn\n").toUtf8());
        //socket->waitForBytesWritten(1000);
        socket->write(QString("0\n").toUtf8());
        socket->waitForBytesWritten(1000);
    #endif
        socket->write(QString("forward\n").toUtf8());
        socket->waitForBytesWritten(1000);
        socket->write(QString("1\n").toUtf8());
        socket->write(QString("50\n").toUtf8());
        socket->waitForBytesWritten(1000);
        QThread::msleep(forward_dist);
        socket->write(QString("forward\n").toUtf8());
        socket->waitForBytesWritten(1000);
        socket->write(QString("0\n").toUtf8());
        socket->waitForBytesWritten(1000);
        QThread::msleep(1000);

        socket->write(QString("landing\n").toUtf8());
        socket->waitForBytesWritten(1000);


    }else if(cmd==1){ //반 복이착륙
        socket->write(QString("takeoff\n").toUtf8());
        socket->waitForBytesWritten(1000);
        if(arg!=0){
            QThread::msleep(2000);
            while(arg--){
                socket->write(QString("up\n").toUtf8());
                socket->write(QString("1\n").toUtf8());
                QString aa = QString::number(up_wow);
                socket->write(QString(aa+"\n").toUtf8());
                socket->waitForBytesWritten(1000);

                QThread::msleep(time1);

                socket->write(QString("up\n").toUtf8());
                socket->write(QString("0\n").toUtf8());
                socket->waitForBytesWritten(1000);
                QThread::msleep(time2);

               /*
                socket->write(QString("down\n").toUtf8());
                socket->write(QString("1\n").toUtf8());
                socket->write(QString("50\n").toUtf8());
                socket->waitForBytesWritten(1000);
                socket->write(QString("down\n").toUtf8());
                socket->write(QString("0\n").toUtf8());
                socket->waitForBytesWritten(1000);
                */
            }


        }
        QThread::msleep(m_1_2_sleep_time);
        socket->write(QString("landing\n").toUtf8());
        socket->waitForBytesWritten(1000);
QThread::msleep(3000);

    }
//    QThread::msleep(3000);
//




qDebug()<<"thread end";


}
