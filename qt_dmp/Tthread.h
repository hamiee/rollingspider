#ifndef THREAD_H
#define THREAD_H
#include <QThread>
#include <QTcpSocket>

class Tthread : public QThread
{
public:
    QTcpSocket* socket;
    Tthread();
    ~Tthread();
    void run();
    double yaw;
    double target_yaw;
   double target_press;
   int time1,time2;
    int cmd;
    int m_1_2_sleep_time;
    double arg;
    int height;
    int up_wow;
    int forward_dist;
};

#endif // THREAD_H
