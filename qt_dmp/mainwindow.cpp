#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qcustomplot.h"
#include <qprocess.h>
#include <qdebug.h>
#include <unistd.h>
#include <QTimer>
#include <signal.h>
#include <QWidget>
#include <pthread.h>
#include <QEventLoop>
#include <QKeyEvent>
#include <QDateTime>
#include <QThread>
#include <QtMath>
#include <QApplication>
#include <QTcpServer>
#include <QRegExp>
#include <QTcpSocket>
#include "Tthread.h"
#include <time.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    /************VALUE SET**********/
    _connect=false;


    /*************TELNET SET********/
    telnet = new QProcess(this);
    telnet->setProcessChannelMode(QProcess::MergedChannels);
    connect(telnet, SIGNAL(readyRead()), this, SLOT(read_telnet()));
    connect(telnet, SIGNAL(started()), this, SLOT(slotProcessStarted()));
    connect(telnet, SIGNAL(finished(int)), this, SLOT(slotProcessFinished()));

    /**********OPEN GL**************/
    cubeshow=1;
    xval=0,yval=0,yyval=0;zval=0;
    cube2 = new MyGLWidget;
    DrawCube(x,y,degree);
    init =0;
    /************GATT TOOL**********/
    gatttool = new QProcess(this);
    gatttool->setProcessChannelMode(QProcess::MergedChannels);
    connect(gatttool, SIGNAL(readyRead()), this, SLOT(read_gatttool()));
    connect(gatttool, SIGNAL(started()), this, SLOT(slotProcessStarted()));
    connect(gatttool, SIGNAL(finished(int)), this, SLOT(slotProcessFinished()));
    ping_timer = new QTimer(this);
    connect(ping_timer, SIGNAL(timeout()), this, SLOT(send_ping()));

    cmd_value=5;
    ping_value=0;
    config_value=1;
     _connected=false;
    configset=true;
    /***********KEY EVENT*************/
    key_event=true;
    key_check_timer = new QTimer(this);
    //connect(key_check_timer, SIGNAL(timeout()), this, SLOT());

    /*********************************/
    mission_timer = new QTimer(this);
    connect(mission_timer, SIGNAL(timeout()), this, SLOT(show_time()));
    /************ADB SOCKET***********/
    /* nextBlocksize=0;
     connect(ui->socket_connect_Button, SIGNAL(clicked()), this, SLOT(connect_to_server()));
     connect(&adb_socket, SIGNAL(connected()), this, SLOT(on_Connect_Server()));
     connect(ui->socket_send_Button, SIGNAL(clicked()), this, SLOT(send_Request()));
     connect(&adb_socket, SIGNAL(readyRead()), this, SLOT(read_Message()));
     connect(&adb_socket, SIGNAL(disconnected()), this, SLOT(connection_Closed_By_Server()));
     connect(&adb_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error()));*/

    tt = new Tthread();


    /*
     *
     *  커스 텀 프 롯 1
     *
     * */

    ui->customPlot_1->addGraph(); // blue line
    ui->customPlot_1->graph(0)->setPen(QPen(Qt::blue));
    ui->customPlot_1->graph(0)->setBrush(QBrush(QColor(240, 255, 200)));
    ui->customPlot_1->graph(0)->setAntialiasedFill(false);
    ui->customPlot_1->addGraph(); // red line
    ui->customPlot_1->graph(1)->setPen(QPen(Qt::red));
    ui->customPlot_1->graph(0)->setChannelFillGraph(ui->customPlot_1->graph(1));



    ui->customPlot_1->addGraph(); // blue dot
    ui->customPlot_1->graph(2)->setPen(QPen(Qt::blue));
    ui->customPlot_1->graph(2)->setLineStyle(QCPGraph::lsNone);
    ui->customPlot_1->graph(2)->setScatterStyle(QCPScatterStyle::ssDisc);
    ui->customPlot_1->addGraph(); // red dot
    ui->customPlot_1->graph(3)->setPen(QPen(Qt::red));
    ui->customPlot_1->graph(3)->setLineStyle(QCPGraph::lsNone);
    ui->customPlot_1->graph(3)->setScatterStyle(QCPScatterStyle::ssDisc);

    /*************************yaw*********************/

    ui->customPlot_1->addGraph(); // green line
    ui->customPlot_1->graph(4)->setPen(QPen(Qt::green));
    ui->customPlot_1->graph(0)->setChannelFillGraph(ui->customPlot_1->graph(4));
    ui->customPlot_1->addGraph();//green dot(yaw)
    ui->customPlot_1->graph(5)->setPen(QPen(Qt::green));
    ui->customPlot_1->graph(5)->setLineStyle(QCPGraph::lsNone);
    ui->customPlot_1->graph(5)->setScatterStyle(QCPScatterStyle::ssDisc);



    ui->customPlot_1->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    ui->customPlot_1->xAxis->setDateTimeFormat("hh:mm:ss");
    ui->customPlot_1->xAxis->setAutoTickStep(false);
    ui->customPlot_1->xAxis->setTickStep(2);
    ui->customPlot_1->axisRect()->setupFullAxesBox();

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(ui->customPlot_1->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot_1->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->customPlot_1->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot_1->yAxis2, SLOT(setRange(QCPRange)));




/*
 *
 *
 *  커스 텀 플 롯
 *
 * */


    ui->customPlot->addGraph(); // blue line
    ui->customPlot->graph(0)->setPen(QPen(Qt::blue));
    ui->customPlot->graph(0)->setBrush(QBrush(QColor(240, 255, 200)));
    ui->customPlot->graph(0)->setAntialiasedFill(false);
    ui->customPlot->addGraph(); // red line
    ui->customPlot->graph(1)->setPen(QPen(Qt::red));
    ui->customPlot->graph(0)->setChannelFillGraph(ui->customPlot->graph(1));



    ui->customPlot->addGraph(); // blue dot
    ui->customPlot->graph(2)->setPen(QPen(Qt::blue));
    ui->customPlot->graph(2)->setLineStyle(QCPGraph::lsNone);
    ui->customPlot->graph(2)->setScatterStyle(QCPScatterStyle::ssDisc);
    ui->customPlot->addGraph(); // red dot
    ui->customPlot->graph(3)->setPen(QPen(Qt::red));
    ui->customPlot->graph(3)->setLineStyle(QCPGraph::lsNone);
    ui->customPlot->graph(3)->setScatterStyle(QCPScatterStyle::ssDisc);

    /*************************yaw*********************/

    ui->customPlot->addGraph(); // green line
    ui->customPlot->graph(4)->setPen(QPen(Qt::green));
    ui->customPlot->graph(0)->setChannelFillGraph(ui->customPlot->graph(4));
    ui->customPlot->addGraph();//green dot(yaw)
    ui->customPlot->graph(5)->setPen(QPen(Qt::green));
    ui->customPlot->graph(5)->setLineStyle(QCPGraph::lsNone);
    ui->customPlot->graph(5)->setScatterStyle(QCPScatterStyle::ssDisc);



    ui->customPlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    ui->customPlot->xAxis->setDateTimeFormat("hh:mm:ss");
    ui->customPlot->xAxis->setAutoTickStep(false);
    ui->customPlot->xAxis->setTickStep(2);
    ui->customPlot->axisRect()->setupFullAxesBox();

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(ui->customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->yAxis2, SLOT(setRange(QCPRange)));

    // setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
    //QTimer mission_timer = new QTimer(this);
    //connect(mission_timer, SIGNAL(timeout()), this, SLOT(show_time()));

    dataTimer = new QTimer(this);
    connect(dataTimer, SIGNAL(timeout()), this, SLOT(realtimeDataSlot()));
    dataTimer->start(20); // Interval 0 means to refresh as fast as possible


}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::realtimeDataSlot(){




    /*
     *
     * custom plot
     *
     * */
    // calculate two new data points:
    double key = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;
    static double lastPointKey = 0;
    if (key-lastPointKey > 0.01) // at most add point every 10 ms
    {
      double value0 = zval;
              //qSin(key*1.6+qCos(key*1.7)*2)*10 + qSin(key*1.2+0.56)*20 + 26;
      double value1 = xval;
              //qSin(key*1.3+qCos(key*1.2)*1.2)*7 + qSin(key*0.9+0.26)*24 + 26;
      double value2 = yyval;
      // add data to lines:
      ui->customPlot->graph(0)->addData(key, value0);
      ui->customPlot->graph(1)->addData(key, value1);
      ui->customPlot->graph(4)->addData(key, value2);
      // set data of dots:
      ui->customPlot->graph(2)->clearData();
      ui->customPlot->graph(2)->addData(key, value0);
      ui->customPlot->graph(3)->clearData();
      ui->customPlot->graph(3)->addData(key, value1);
      ui->customPlot->graph(5)->clearData();
      ui->customPlot->graph(5)->addData(key, value2);
      // remove data of lines that's outside visible range:
      ui->customPlot->graph(0)->removeDataBefore(key-8);
      ui->customPlot->graph(1)->removeDataBefore(key-8);
      ui->customPlot->graph(4)->removeDataBefore(key-8);
      // rescale value (vertical) axis to fit the current data:
      ui->customPlot->graph(0)->rescaleValueAxis();
      ui->customPlot->graph(1)->rescaleValueAxis(true);
      ui->customPlot->graph(4)->rescaleValueAxis(true);
      /*
       *
       *  custom plot 1
       *
       * */
      double value3 = a_roll;
              //qSin(key*1.6+qCos(key*1.7)*2)*10 + qSin(key*1.2+0.56)*20 + 26;
      double value4 = a_pitch;
      double value5;
      //qSin(key*1.3+qCos(key*1.2)*1.2)*7 + qSin(key*0.9+0.26)*24 + 26;
      if(ui->checkBox->isChecked()){
         value5 = a_yaw;
      }else{
          value5 = 0;
      }
      // add data to lines:
      ui->customPlot_1->graph(0)->addData(key, value3);
      ui->customPlot_1->graph(1)->addData(key, value4);
      ui->customPlot_1->graph(4)->addData(key, value5);
      // set data of dots:
      ui->customPlot_1->graph(2)->clearData();
      ui->customPlot_1->graph(2)->addData(key, value3);
      ui->customPlot_1->graph(3)->clearData();
      ui->customPlot_1->graph(3)->addData(key, value4);
      ui->customPlot_1->graph(5)->clearData();
      ui->customPlot_1->graph(5)->addData(key, value5);
      // remove data of lines that's outside visible range:
      ui->customPlot_1->graph(0)->removeDataBefore(key-8);
      ui->customPlot_1->graph(1)->removeDataBefore(key-8);
      ui->customPlot_1->graph(4)->removeDataBefore(key-8);
      // rescale value (vertical) axis to fit the current data:
        ui->customPlot_1->graph(0)->rescaleValueAxis();
      ui->customPlot_1->graph(1)->rescaleValueAxis(true);
      ui->customPlot_1->graph(4)->rescaleValueAxis(true);


      lastPointKey = key;
    }
    // make key axis range scroll with the data (at a constant range size of 8):
    ui->customPlot->xAxis->setRange(key+0.25, 8, Qt::AlignRight);
    ui->customPlot->replot();
    ui->customPlot_1->xAxis->setRange(key+0.25, 8, Qt::AlignRight);
    ui->customPlot_1->replot();

    // calculate frames per second:
    static double lastFpsKey;
    static int frameCount;
    ++frameCount;
    if (key-lastFpsKey > 2) // average fps over 2 seconds
    {
      ui->statusBar->showMessage(
            QString("%1 FPS, Total Data points: %2")
            .arg(frameCount/(key-lastFpsKey), 0, 'f', 0)
            .arg(ui->customPlot->graph(0)->data()->count()+
                 ui->customPlot->graph(1)->data()->count()+
                 ui->customPlot->graph(4)->data()->count())
            , 0);
      ui->statusBar->showMessage(
            QString("%1 FPS, Total Data points: %2")
            .arg(frameCount/(key-lastFpsKey), 0, 'f', 0)
            .arg(ui->customPlot_1->graph(0)->data()->count()+
                 ui->customPlot_1->graph(1)->data()->count()+
                 ui->customPlot_1->graph(4)->data()->count())
            , 0);
      lastFpsKey = key;
      frameCount = 0;
    }

}

/***************************************
    SYSTEM BUTTON
***************************************/

void MainWindow::on_scan_button_clicked()
{
    ui->textEdit->append("clicked scan\n");
    //hcitool scan

     QProcess p;
    QString cmd("hcitool scan");

        //start the process
        p.start(cmd);

        //Wait for the processs to finish with a timeout of 20 seconds
        if(p.waitForFinished(20000))
        {
            //Clear the list widget
            //this->ui->listWidget->clear();

            //Read the command line output and store it in QString out
            QString out(p.readAllStandardOutput());

            //Split the QString every new line and save theve in a QStringList
            QStringList OutSplit = out.split("\n");

            foreach(QString line , OutSplit){
                ui->textEdit->append(line);
            }

        }
        p.close();
}



void MainWindow::on_pand_button_clicked()
{
    system("pand --killall");
    QString cmd ="sudo pand";

        cmd.append(" --connect ");
        cmd.append(ui->pand_combobox->currentText());
        cmd.append(" -dGN -n");
        system(cmd.toUtf8());

     ui->ip_button->setEnabled(true);
    system("ifconfig bnep0 192.168.1.2 up");

}
void MainWindow::on_ip_button_clicked()
{
     QStringList args;
   if(ui->ip_button->text()=="CONNECT"){
       ui->textEdit->clear();
       ui->ip_line->setEnabled(false);
       ui->ip_button->setText("DIS CONNECT");
       if(telnet->isOpen()==true)
           telnet->finished(1);
       telnet->setProgram("telnet");
       qDebug()<<"telnet set";
       args<<ui->ip_line->text().toUtf8();
       qDebug()<<"telnet args set";
       telnet->setArguments(args);
       qDebug()<<args;
       telnet->start(QProcess::Unbuffered | QProcess::ReadWrite);
       qDebug()<<"telnet read write";
       if(telnet->waitForStarted(3000))
       {
           qDebug()<<"init if";
           telnet->setTextModeEnabled(true);
           ui->statusBar->showMessage("Connecting...");

       }

    }else{

       ui->lineEdit->setText("exit");
       write_telnet();

       ui->lineEdit->clear();;
       ui->textEdit->clear();
       ui->textEdit->append("disconnect");
       ui->ip_button->setText("CONNECT");
       ui->statusBar->showMessage("Disconnect");
       _connect=false;
       ui->ip_line->setEnabled(true);
   }
}

void MainWindow::on_ctrl_C_clicked()
{
    const char b=0x03;
    const char *a = &b;

     telnet->write(a);
     ui->lineEdit->clear();
     ui->lineEdit->setText(" ");
     write_telnet();
     ui->lineEdit->clear();
     ui->lineEdit->setText(" ");
     write_telnet();
     ui->lineEdit->clear();
}
void MainWindow::on_dragon_prog_clicked()
{
    ui->lineEdit->setText("dragon-prog");
    write_telnet();
    ui->lineEdit->clear();
    sleep(2);
    on_ctrl_C_clicked();
    ui->lineEdit->clear();
    ui->lineEdit->setText(" ");
    write_telnet();
}

void MainWindow::on_clear_button_clicked()
{
    ui->textEdit->clear();
}

/***************************************
    READ WRITE BUTTON
***************************************/

void MainWindow::on_lineEdit_editingFinished()
{
    write_telnet();
    ui->lineEdit->clear();
}

void MainWindow::read_telnet(){//slotConnect

    QString allText = telnet->readAll();
    QStringList trash_value,blue_data;
    QStringList lines = allText.split('\n');

    foreach(QString line, lines)
    {
        if(line.contains("[Delos]"))
        {
            if(!_connect)
                ui->statusBar->showMessage("Connected");
            _connect = true;
        }
        _dataReceived = true;
    if(line.contains("_J_")){
        trash_value = line.split(":");
        blue_data = trash_value[1].split(" ");
        for(int i=0;i<blue_data.length();i++){
                if((i+1)>=blue_data.length()){
                    break;
                }
                /******************DT*************/
                if(blue_data.at(i).compare("dT")==0){
                    //ui->lineEdit_dT->setText(blue_data.at(i+1));
                    //ui->lineEdit_dT_2->setText(blue_data.at(i+1));
                }
                //****************ACCEL DATA************
                else if(blue_data.at(i).compare("RR")==0){
                    a_roll=blue_data.at(i+1).toDouble();
                    ui->roll_out_line_3->setText(QString::number(a_roll));
                    DrawCube(a_pitch,a_yaw,a_roll);

                }else if(blue_data.at(i).compare("PP")==0){
                    //ui->accel_y_out_line->setText(blue_data.at(i+1));
                    a_pitch=blue_data.at(i+1).toDouble();
                    ui->pitch_out_line_3->setText(QString::number(a_pitch));
                    DrawCube(a_pitch,a_yaw,a_roll);

                }else if(blue_data.at(i).compare("YY")==0){
                    //acc_z=blue_data.at(i+1).toDouble();
                    a_yaw=blue_data.at(i+1).toDouble();
                    ui->yaw_out_line_3->setText(QString::number(a_yaw));
                    DrawCube(a_pitch,a_yaw,a_roll);
                }else if(blue_data.at(i).compare("PIT")==0){
                    off_roll = blue_data.at(i+1).toDouble();
                    double val = off_roll - R_OFF;
                    ui->roll_out_line->setText(QString::number(val));
                    yyval = off_roll;
                }else if(blue_data.at(i).compare("STATE")==0){
                    off_pitch = blue_data.at(i+1).toDouble();
                    double val = off_pitch - P_OFF;
                    zval = off_pitch;
                    ui->pitch_out_line->setText(QString::number(val));
                    //DrawCube(xval,yyval,zval);

                }else if(blue_data.at(i).compare("TAR")==0){
                    off_yaw = blue_data.at(i+1).toDouble();
                    xval = off_yaw;
                    ui->yaw_out_line->setText(QString::number(off_yaw));

                    //ui->lineEdit_YAW->setText(  QString::number(off_yaw));
                    //ui->lineEdit_YAW_2->setText(  QString::number(off_yaw));
  /*
                    double val = off_yaw - Y_OFF;

                    tt->yaw = val;//val-yaw_offset;
                  if(tt->yaw>180)
                        tt->yaw = tt->yaw-360;
                    else if(tt->yaw<-180)
                        tt->yaw = tt->yaw+360;

                    yval=blue_data.at(i+1).toDouble();
                    if(yval>180)
                        yval = yval-360;
                    else if(yval<-180)
                        yval = yval+360;
                    yyval=val;//yval-init;

                    QString yaw_qstring = QString::number(yyval);
                    ui->yaw_out_line->setText(yaw_qstring);
                    DrawCube(xval,yyval,zval);
*/
                    //ui->lineEdit_YAW->setText(  QString::number(tt->yaw));
                    //ui->lineEdit_YAW_2->setText(  QString::number(tt->yaw));


                }
                //****************GRYo DATA***********
                else if(blue_data.at(i).compare("GX")==0){
                    //gyro_x=blue_data.at(i+1).toDouble();
                    //ui->lineEdit_GX->setText(blue_data.at(i+1));
                    //ui->lineEdit_GX_2->setText(blue_data.at(i+1));
                    //ui->gyro_x_out_line->setText(blue_data.at(i+1));

                }else if(blue_data.at(i).compare("GY")==0){
                    //gyro_y=blue_data.at(i+1).toDouble();
                    //ui->lineEdit_GY->setText(blue_data.at(i+1));
                    //ui->lineEdit_GY_2->setText(blue_data.at(i+1));
                    //ui->gyro_y_out_line->setText(blue_data.at(i+1));

                }else if(blue_data.at(i).compare("GZ")==0){
                    //gyro_z=blue_data.at(i+1).toDouble();
                    //ui->lineEdit_GZ->setText(blue_data.at(i+1));
                    //ui->lineEdit_GZ_2->setText(blue_data.at(i+1));
                    //ui->gyro_z_out_line->setText(blue_data.at(i+1));

                }
                //****************press DATA***********
                else if(blue_data.at(i).compare("PRESS")==0){
                    //press_ground=blue_data.at(i+1).toDouble();
                    //ui->lineEdit_PRESS->setText(blue_data.at(i+1));
                    //ui->lineEdit_PRESS_2->setText(blue_data.at(i+1));
                    //ui->height_out_line->setText(blue_data.at(i+1));
                    //ui->height_out_line->setText(blue_data.at(i+1));
                }
                else if(blue_data.at(i).compare("TEMP")==0){

                    //ui->temp_out_line->setText(blue_data.at(i+1));
                    //ui->lineEdit_TEMP->setText(blue_data.at(i+1));
                    //ui->lineEdit_TEMP_2->setText(blue_data.at(i+1));

                }
            }



    }else if(line.contains("_J_DATA")){
            trash_value = line.split(":");
            blue_data = trash_value[1].split(" ");
            for(int i=0;i<blue_data.length();i++){
                    /******************DT*************/
                    if(blue_data.at(i).compare("dT")==0){
                        pre_DT=blue_data.at(i+1).toDouble()/1000000000;
                    }
                    //****************KALMAN DATA************
                    else if(blue_data.at(i).compare("RR")==0){
                        zval=blue_data.at(i+1).toDouble();
                        ui->roll_out_line->setText(blue_data.at(i+1));
                        DrawCube(xval,yyval,zval);
                    }else if(blue_data.at(i).compare("PP")==0){
                        xval=blue_data.at(i+1).toDouble()*-1;
                        ui->pitch_out_line->setText(blue_data.at(i+1));
                        DrawCube(xval,yyval,zval);
                    }else if(blue_data.at(i).compare("YY")==0){
                        yval=blue_data.at(i+1).toDouble()*-1;
                        if(yval>180)
                            yval = yval-360;
                        else if(yval<-180)
                            yval = yval+360;
                        yyval=yval-init;
                        QString yaw_qstring = QString::number(yyval);
                        ui->yaw_out_line->setText(yaw_qstring);
                        DrawCube(xval,yyval,zval);
                    }
                    //****************ACCEL DATA************
                    else if(blue_data.at(i).compare("WX")==0){
                        //acc_x=blue_data.at(i+1).toDouble();
                        //ui->accel_x_out_line->setText(blue_data.at(i+1));
                    }else if(blue_data.at(i).compare("WY")==0){
                        //acc_y=blue_data.at(i+1).toDouble();
                        //ui->accel_y_out_line->setText(blue_data.at(i+1));
                    }else if(blue_data.at(i).compare("WZ")==0){
                        //acc_z=blue_data.at(i+1).toDouble();
                        //ui->accel_z_out_line->setText(blue_data.at(i+1));
                    }
                    //****************GRYo DATA***********
                    else if(blue_data.at(i).compare("GX")==0){
                        //gyro_x=blue_data.at(i+1).toDouble();
                        //ui->gyro_x_out_line->setText(blue_data.at(i+1));
                    }else if(blue_data.at(i).compare("GY")==0){
                        //gyro_y=blue_data.at(i+1).toDouble();
                        //ui->gyro_y_out_line->setText(blue_data.at(i+1));
                    }else if(blue_data.at(i).compare("GZ")==0){
                        //gyro_z=blue_data.at(i+1).toDouble();
                        //ui->gyro_z_out_line->setText(blue_data.at(i+1));
                    }
                    //****************press DATA***********
                    else if(blue_data.at(i).compare("GND")==0){
                        press_ground=blue_data.at(i+1).toDouble();
                        //ui->height_out_line->setText(blue_data.at(i+1));
                    }else if(blue_data.at(i).compare("NOW")==0){
                        height=(press_ground-blue_data.at(i+1).toDouble())/10;
                        QString _height =QString::number(height);
                        //ui->height_out_line->setText(_height);
                    }
                    //*****************BATTERY****************
                    else if(blue_data.at(i).compare("BAT")==0){
                        double bat_per;
                        //gyro_x=blue_data.at(i+1).toDouble();
                        bat_per=(blue_data.at(i+1).toDouble()-710)/200*100;
                        QString diff = QString("%1%").arg(bat_per);
                        //ui->bat_label->setText(diff);
                    }
                }
            }
        if(line.contains("_J_BLE")){
            //ui->textEdit->clear();
        }else
        ui->textEdit->append(line);
        }
}
void MainWindow::on_yaw_Button_clicked()
{


 //   R_OFF -= ui->roll_out_line->text().toDouble();
   // P_OFF -= ui->pitch_out_line->text().toDouble();
  //  Y_OFF -= ui->yaw_out_line->text().toDouble();
R_OFF=off_roll;
    P_OFF=off_pitch;
    Y_OFF=off_yaw;

//init = ui->yaw_out_line->text().toDouble()
   // init +=(yval-init);
    //pre_init=init;

}

void MainWindow::write_telnet(){//slotTestRun

    QString test, text = ui->lineEdit-> text();
    QTextStream textStream(&text, QIODevice::ReadOnly);
    QEventLoop eventLoop;
    QTimer timer;
    connect(&timer, SIGNAL(timeout()), &eventLoop, SLOT(quit()));

    while((test = textStream.readLine()) != "")
    {
        test = test.remove("\n");
        if(test.contains("wait"))
        {
            timer.setSingleShot(true);
            disconnect(telnet, SIGNAL(readyRead()), &eventLoop, SLOT(quit()));
            timer.start(5000);
            eventLoop.exec();
            read_telnet();
        }
        else
        {
            connect(&timer, SIGNAL(timeout()), &eventLoop, SLOT(quit()));
            _dataReceived = false;
            telnet->write(test.toUtf8());

            timer.setSingleShot(false);
            timer.setInterval(50);
            timer.start();
            int retries = 100;
            while(!_dataReceived && retries--)
            {
                telnet->write("\n");
                eventLoop.exec(QEventLoop::ExcludeUserInputEvents);
            }

        }
    }
}

/***************************************
    OPEN GL
***************************************/

void MainWindow::DrawCube(double x,double y, double z)
{
    ui->gl_Layout->removeWidget(cube2);

    if(cubeshow==1){
        cube2->resize(500,400);
        ui->gl_Layout->addWidget(cube2);
        cubeshow=0;

    }
    /*qDebug()<<x;
    qDebug()<<y;
    qDebug()<<z;
    qDebug()<<"------------------";*/

    ui->gl_Layout->addWidget(cube2);

    cube2->SetXYZD(x,y,z);//cube에데이터전
    cube2->paintGL();//cube다시그리기
    cube2->update();//cube 갱신


    glTranslatef(0.0f,0.0f,-20); //move along z-axis

}

/***************************************
  MISSION BUTTON
***************************************/

void MainWindow::on_aout_clicked()
{

    ui->lineEdit->clear();
    ui->lineEdit->setText(" ");
    write_telnet();
    ui->lineEdit->setText("/bin/JAE/QTtest.out");
    write_telnet();
    ui->lineEdit->clear();

}
void MainWindow::on_sub_start_clicked()
{

    ui->lineEdit->setText(ui->sub_pro_lineEdit->text());
    write_telnet();
    ui->lineEdit->clear();

}
/***************************************
  SENSOR TEST
***************************************/

void MainWindow::on_sensor_button_clicked()
{
    /*
    if(ui->sensor_button->text()=="Sensor ON"){

        if(ui->accel_checkbox->checkState()==2)
        {
            ui->lineEdit->setText("4 7 1");
            write_telnet();
            ui->lineEdit->clear();
            qDebug()<<"accel on";
        }
        if(ui->gyro_checkbox->checkState()==2)
        {
            ui->lineEdit->setText("4 1 1");
            write_telnet();
            ui->lineEdit->clear();
            qDebug()<<"gyro on";
        }
        if(ui->kalman_checkbox->checkState()==2)
        {
            ui->lineEdit->setText("4 2 1");
            write_telnet();
            ui->lineEdit->clear();
            qDebug()<<"kalman on";
        }
        if(ui->bat_checkbox->checkState()==2)
        {
            ui->lineEdit->setText("4 3 1");
            write_telnet();
            ui->lineEdit->clear();
            qDebug()<<"BAT on";
        }
        if(ui->press_checkBox->checkState()==2)
        {
            ui->lineEdit->setText("4 4 1");
            write_telnet();
            ui->lineEdit->clear();
            qDebug()<<"BARO on";
        }

        qDebug()<<"------------------------";
        ui->sensor_button->setText("Sensor Off");
    }else{
        ui->lineEdit->setText("4 1 0");
        write_telnet();
        ui->lineEdit->clear();

        ui->lineEdit->setText("4 2 0");
        write_telnet();
        ui->lineEdit->clear();

        ui->lineEdit->setText("4 3 0");
        write_telnet();
        ui->lineEdit->clear();

        ui->lineEdit->setText("4 4 0");
        write_telnet();
        ui->lineEdit->clear();

        ui->lineEdit->setText("0");
        write_telnet();
        ui->lineEdit->clear();
        ui->sensor_button->setText("Sensor ON");
    }*/
}
void MainWindow::sensor_line_clean(){

    ui->pitch_out_line->clear();
    ui->roll_out_line->clear();
    ui->yaw_out_line->clear();
}

/***************************************
  MOTER SET SLOT
***************************************/

/*
void MainWindow::on_moter_speed_button_clicked()
{
    //QStringList args;

    motor0_val = ui->motor0_offset_line->text();
    motor1_val = ui->motor1_offset_line->text();
    motor2_val = ui->motor2_offset_line->text();
    motor3_val = ui->motor3_offset_line->text();
    //ui->lineEdit->setText("1 "+motor0_val+" "+motor1_val+" "+motor2_val+" "+motor3_val);
    //write_telnet();
    //ui->lineEdit->clear();




}

void MainWindow::on_moter_stop_clicked()
{
    ui->lineEdit->setText("1 0 0 0 0");
    write_telnet();
    ui->lineEdit->clear();
    ui->motor0_line->clear();
    ui->motor1_line->clear();
    ui->motor2_line->clear();
    ui->motor3_line->clear();
}
*/
/***************************************
  PID TEST
***************************************/


/***************************************
  trash slot
***************************************/
void MainWindow::slotProcessStarted(){
    qDebug() << __FUNCTION__;
}
void MainWindow::slotProcessFinished(){
    qDebug() << __FUNCTION__;
}

/***************************************
    GATT TOOL
***************************************/


/***************************************
    KEY EVENT
***************************************/


void MainWindow::keyPressEvent(QKeyEvent *event){

    if(key_event==true){
        switch(event->key())
        {
            case Qt::Key_Q:
                speed = 0;
                key_write();
            break;
            case Qt::Key_E:
                speed = 9000;
                key_write();
                sleep(1);
                speed = 8000;
                key_write();
            break;
            case Qt::Key_W:
                speed = speed +1000;
                key_write();
            break;
            case Qt::Key_S:
                speed = speed - 1000;
               key_write();
            break;

         }

    }
}
void MainWindow::key_write(){
    if(speed<0)
        speed=0;
    else if(speed>9999)
        speed=9999;
    qst_speed=QString::number(speed);
    ui->lineEdit->setText("3 "+qst_speed);
    write_telnet();
    ui->lineEdit->clear();
}

void MainWindow::on_joystick_clicked()
{
    /*if(ui->joystick->text()=="START")
    {
    ui->lineEdit->clear();
    ui->lineEdit->setText("1");
    write_telnet();
    ui->lineEdit->clear();
    sensor_line_clean();
    ui->joystick->setText("STOP");
    }else{
        ui->lineEdit->clear();
        ui->lineEdit->setText("0");
        write_telnet();
        ui->lineEdit->clear();
        mission_timer->stop();
        ui->joystick->setText("START");
    }
*/





    /*
    if(ui->joystick->text()=="JOY ON"){
        pid_line_update(false);
        ui->lineEdit->setEnabled(false);
        ui->ble_lineEdit->setEnabled(false);
        key_event=true;
        ui->joystick->setText("JOY OFF");

    }else{
        pid_line_update(true);
        ui->lineEdit->setEnabled(true);
        ui->ble_lineEdit->setEnabled(true);
        key_event=false;
        ui->joystick->setText("JOY ON");
    }*/
}

void MainWindow::on_emergency_clicked()
{
    MainWindow::send_cmd("emercy");
    /*ui->lineEdit->setText("3 0");
    write_telnet();
    ui->lineEdit->clear();
    on_ctrl_C_clicked();
    ui->lineEdit->setText("a");
    write_telnet();
    ui->lineEdit->clear();
    mission_timer->stop();
*/

}


void MainWindow::show_time(){
    qint64 current_time = QTime::currentTime().msecsTo(start_time)*-1;
    int h = current_time / 1000 / 60 / 60;
    int m = (current_time / 1000 / 60) - (h * 60);
    int s = (current_time / 1000) - (m * 60);
    current_time = current_time - (s * 1000);
    QString diff = QString("%1:%2:%3:%4").arg(h).arg(m).arg(s).arg(current_time);
    //ui->clock_label->setText(diff);

}

void MainWindow::on_mission_button_clicked()
{
/*
    clock_t start_point, end_point;

    start_point = clock();



    if(!(tt->isRunning())){

        tt->cmd=1;
        //tt->target_press = ui->lineEdit_PRESS->text().toDouble() - ui->set_height_out_line->text().toDouble();
        //ui->distance_out_line->setText(QString::number(tt->target_press));
        //tt->m_1_2_sleep_time = ui->set_throttle_out_line->text().toInt();


        //double aa = ui->set_height_out_line->text().toDouble();
        tt->arg=0;

        if((aa>80)&&(aa <= 100)){
            tt->arg = 1;
            tt->time1 = 2000;
            tt->time2  = 2000;
            tt->up_wow=12;
        }else if((100<aa)&&(aa<=150)){
            tt->arg = 1;
            tt->time1 = 1700;
            tt->time2 = 1700;
            tt->up_wow=22;
        }else if((150<aa)&&(aa<=200)){
            tt->arg = 1;
            tt->time1 = 2000;
            tt->time2 = 2000;
            tt->up_wow=50;
        }


        //int loop = ui->count_time_lineEdit->text().toInt();
        while(loop--){
            tt->start();
            while(tt->isRunning()){
                QApplication::processEvents();

                end_point = clock();
                ui->lineEdit_m1_2_time->setText(QString::number(((double)(end_point - start_point)/CLOCKS_PER_SEC)));
            }
            tt->wait();
        }


    }
*/
}

/********************ADB SOCKET*****************************/
/*
void MainWindow::connect_to_server(){
    adb_socket.connectToHost(QHostAddress::LocalHost, 8010);//"127.0.0.1",8888);
}
void MainWindow::on_Connect_Server(){
    ui->socket_textEdit->setText("Socket Connected\n");

}

void MainWindow::send_Request(){
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out<<quint16(0);
    out<<ui->socket_lineEdit->text();
    out.device()->seek(0);
    out<<quint16(block.size() - sizeof(quint16));
    adb_socket.write(block);
    ui->socket_textEdit->append("send<< "+block);
}
void MainWindow::read_Message(){
    QDataStream in(&adb_socket);
    while(true){
             //nextBlcokSize 가 0 이면 아직 데이터를 못받은것
            if(nextBlocksize == 0){
                //수신된 데이터가 nextBlockSize 바이트보다 큰지 확인
                if(adb_socket.bytesAvailable() < sizeof(quint16))
                    ;
                else
                    in>>nextBlocksize;
                continue;
            }
            //nextBlcokSize가 도착하면 사이즈만큼 데이터가 도착했는지 확인
           else if(adb_socket.bytesAvailable() < nextBlocksize)
                continue;

            //데이터를 표시
           else if(adb_socket.bytesAvailable() >= nextBlocksize){
                QString strBuf;
                in>>strBuf;

                ui->socket_textEdit->setText(strBuf);
                this->nextBlocksize = 0;

                break;
            }
        }
}

void MainWindow::connection_Closed_By_Server(){
    adb_socket.close();
}

void MainWindow::error(){
    ui->socket_textEdit->setText(adb_socket.errorString());
}

*/


void MainWindow::on_socket_connect_Button_clicked()
{

    socket=new QTcpSocket(this);
    connect(socket,SIGNAL(readyRead()),this,SLOT(socket_readyRead()));
    connect(socket,SIGNAL(connected()),this,SLOT(socket_connected()));
    //socket->connectToHost(ui->socket_ip_lineEdit->text(),ui->socket_port_lineEdit->text().toInt());
    if(socket->waitForConnected(3000))
        {
            qDebug() << "Connected!";
            // send
            //socket->write("hello server\r\n\r\n\r\n\r\n");
            //socket->waitForBytesWritten(1000);
            socket->waitForReadyRead(3000);
            qDebug() << "Reading: " << socket->bytesAvailable();
            qDebug() << socket->readAll();
            tt->socket=socket;
        }
        else
        {
            qDebug() << "Not connected!";
        }
}


void MainWindow::socket_readyRead()
{
   /*  while(socket->canReadLine())
    {
        qDebug()<<"ready read";
        QString line=QString::fromUtf8(socket->readLine());
        line=line.left(line.length()-1);
        ui->socket_textEdit->append(QString("%2").arg(line));
    }
    //ui->socket_textEdit->scrollToBottom();*/
}


void MainWindow::socket_connected()
{
    qDebug()<<"socket connected";
    //socket->write(QString("me:"+ui->socket_lineEdit->text()+"\n").toUtf8());
}


void MainWindow::on_socket_lineEdit_returnPressed()
{
    on_socket_send_Button_clicked();
}

void MainWindow::send_cmd(QString val){
    socket->write(QString(val+"\n").toUtf8());
}

void MainWindow::on_socket_send_Button_clicked()
{
    //QString message=ui->socket_lineEdit->text().trimmed();

    //ui->socket_lineEdit->setFocus();
}


void MainWindow::on_disconnect_btn_clicked()
{
    MainWindow::send_cmd("emercy");
    MainWindow::send_cmd("exit");

    socket->close();
}



//QThread::usleep(unsigned long microseconds);
//QThread::msleep(unsigned long milliseconds);
//QThread::sleep(unsigned long second);
void MainWindow::on_pushButton_mission1_clicked()
{


}



void MainWindow::on_pushButton_emergency_clicked()
{
    MainWindow::send_cmd("emercy");
}

void MainWindow::on_pushButton_3_clicked()
{
    MainWindow::send_cmd("emercy");
}

void MainWindow::on_pushButton_mission_1_2_start_clicked()
{


}
/*
void MainWindow::on_pushButton_mission_1_2_start_2_clicked()
{
    if(!(tt->isRunning())){
        tt->cmd=2;
        tt->arg = ui->lineEdit_m1_2_time_2->text().toInt();
        tt->start();
        while(tt->isRunning()){
            QApplication::processEvents();
        }
        tt->wait();
    }
}
*/
void MainWindow::on_pushButton_4_clicked()
{
    MainWindow::send_cmd("emercy");
}

void MainWindow::on_pushButton_mission_1_2_start_3_clicked()
{

}


void MainWindow::on_tabWidget_destroyed()
{
    MainWindow::send_cmd("emercy");
    MainWindow::send_cmd("exit");

    socket->close();
}

void MainWindow::on_pushButton_clicked()
{

QString aa = QString::number(ui->lineEdit_gamma->text().toDouble());
    ui->lineEdit->setText("gamma pitch "+aa+"\n");
    write_telnet();
    ui->lineEdit->clear();
}


void MainWindow::on_pushButton_2_clicked()
{
    QString a1 = QString::number(ui->lineEdit_a1->text().toDouble());
    QString a2 = QString::number(ui->lineEdit_a2->text().toDouble());
    QString a3 = QString::number(ui->lineEdit_a3->text().toDouble());
    QString a4 = QString::number(ui->lineEdit_a4->text().toDouble());
        ui->lineEdit->setText("1 test "+a1+" "+a2+" "+a3+" "+a4+" 0\n");
        write_telnet();
        ui->lineEdit->clear();
}

void MainWindow::on_PB_EMER_clicked()
{
    ui->lineEdit->setText("0\n0\n");
    write_telnet();
    ui->lineEdit->clear();
}

void MainWindow::on_pushButton_5_clicked()
{
    /*a_roll=ui->roll_out_line_3->text().toDouble();
    //ui->accel_y_out_line->setText(blue_data.at(i+1));
    a_pitch=ui->pitch_out_line_3->text().toDouble();


    //acc_z=blue_data.at(i+1).toDouble();
    a_yaw=ui->yaw_out_line_3->text().toDouble();

    DrawCube(a_pitch,a_yaw,a_roll);
*/
    ui->lineEdit->setText("bool EKF 0\n");
    write_telnet();
    ui->lineEdit->clear();

}

void MainWindow::on_pushButton_6_clicked()
{
    ui->lineEdit->setText("bool EKF 1\n");
    write_telnet();
    ui->lineEdit->clear();
}

void MainWindow::on_pushButton_7_clicked()
{
    ui->lineEdit->setText("sensor rpy 1\nbool cancel 0\nbool throttle 0\n throttle -100000\n1\n");
    write_telnet();
    ui->lineEdit->clear();
}

void MainWindow::on_PB_EMER_2_clicked()
{
    ui->lineEdit->setText("0\n");
    write_telnet();
    ui->lineEdit->clear();
}

void MainWindow::on_lineEdit_2_editingFinished()
{

    ui->lineEdit->setText("target pitch "+ui->lineEdit_2->text()+ "\n");
    write_telnet();
    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
}
