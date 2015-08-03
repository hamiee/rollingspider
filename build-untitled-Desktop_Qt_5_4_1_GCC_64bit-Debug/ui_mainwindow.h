/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QTabWidget *tabWidget;
    QWidget *tab;
    QLabel *label_26;
    QCustomPlot *customPlot;
    QWidget *formLayoutWidget_6;
    QFormLayout *formLayout_6;
    QLabel *label_11;
    QLineEdit *roll_out_line;
    QLabel *label_18;
    QLineEdit *pitch_out_line;
    QLabel *label_13;
    QLineEdit *yaw_out_line;
    QWidget *verticalLayoutWidget_3;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout;
    QPushButton *clear_button;
    QPushButton *scan_button;
    QPushButton *dragon_prog;
    QHBoxLayout *horizontalLayout_3;
    QComboBox *pand_combobox;
    QPushButton *pand_button;
    QHBoxLayout *horizontalLayout_2;
    QLineEdit *ip_line;
    QPushButton *ip_button;
    QHBoxLayout *horizontalLayout_4;
    QLineEdit *sub_pro_lineEdit;
    QPushButton *sub_start;
    QPushButton *ctrl_C;
    QWidget *verticalLayoutWidget_4;
    QVBoxLayout *verticalLayout_5;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_7;
    QLineEdit *lineEdit_gamma;
    QPushButton *pushButton;
    QHBoxLayout *horizontalLayout_8;
    QLabel *label_8;
    QLabel *label_15;
    QLabel *label_10;
    QLabel *label_9;
    QLabel *label_16;
    QHBoxLayout *horizontalLayout_9;
    QLineEdit *lineEdit_a1;
    QLineEdit *lineEdit_a2;
    QLineEdit *lineEdit_a3;
    QLineEdit *lineEdit_a4;
    QPushButton *pushButton_2;
    QPushButton *PB_EMER;
    QWidget *verticalLayoutWidget_7;
    QVBoxLayout *verticalLayout_7;
    QTextEdit *textEdit;
    QLineEdit *lineEdit;
    QLineEdit *lineEdit_2;
    QWidget *tab_6;
    QWidget *gridLayoutWidget;
    QGridLayout *gl_Layout;
    QCustomPlot *customPlot_1;
    QWidget *formLayoutWidget_9;
    QFormLayout *formLayout_9;
    QLabel *label_17;
    QLineEdit *roll_out_line_3;
    QLabel *label_20;
    QLineEdit *pitch_out_line_3;
    QLabel *label_21;
    QLineEdit *yaw_out_line_3;
    QCheckBox *checkBox;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout_15;
    QPushButton *pushButton_5;
    QPushButton *pushButton_6;
    QPushButton *pushButton_7;
    QPushButton *PB_EMER_2;
    QStatusBar *statusBar;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1188, 820);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tabWidget->setGeometry(QRect(0, -10, 1171, 771));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(tabWidget->sizePolicy().hasHeightForWidth());
        tabWidget->setSizePolicy(sizePolicy1);
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        label_26 = new QLabel(tab);
        label_26->setObjectName(QStringLiteral("label_26"));
        label_26->setGeometry(QRect(10, 530, 67, 17));
        customPlot = new QCustomPlot(tab);
        customPlot->setObjectName(QStringLiteral("customPlot"));
        customPlot->setGeometry(QRect(50, 30, 1081, 391));
        formLayoutWidget_6 = new QWidget(tab);
        formLayoutWidget_6->setObjectName(QStringLiteral("formLayoutWidget_6"));
        formLayoutWidget_6->setGeometry(QRect(750, 430, 371, 201));
        formLayout_6 = new QFormLayout(formLayoutWidget_6);
        formLayout_6->setSpacing(6);
        formLayout_6->setContentsMargins(11, 11, 11, 11);
        formLayout_6->setObjectName(QStringLiteral("formLayout_6"));
        formLayout_6->setContentsMargins(0, 0, 0, 0);
        label_11 = new QLabel(formLayoutWidget_6);
        label_11->setObjectName(QStringLiteral("label_11"));
        QFont font;
        font.setPointSize(30);
        label_11->setFont(font);

        formLayout_6->setWidget(0, QFormLayout::LabelRole, label_11);

        roll_out_line = new QLineEdit(formLayoutWidget_6);
        roll_out_line->setObjectName(QStringLiteral("roll_out_line"));
        roll_out_line->setFont(font);
        roll_out_line->setReadOnly(false);

        formLayout_6->setWidget(0, QFormLayout::FieldRole, roll_out_line);

        label_18 = new QLabel(formLayoutWidget_6);
        label_18->setObjectName(QStringLiteral("label_18"));
        label_18->setFont(font);

        formLayout_6->setWidget(1, QFormLayout::LabelRole, label_18);

        pitch_out_line = new QLineEdit(formLayoutWidget_6);
        pitch_out_line->setObjectName(QStringLiteral("pitch_out_line"));
        pitch_out_line->setFont(font);
        pitch_out_line->setReadOnly(false);

        formLayout_6->setWidget(1, QFormLayout::FieldRole, pitch_out_line);

        label_13 = new QLabel(formLayoutWidget_6);
        label_13->setObjectName(QStringLiteral("label_13"));
        label_13->setFont(font);

        formLayout_6->setWidget(2, QFormLayout::LabelRole, label_13);

        yaw_out_line = new QLineEdit(formLayoutWidget_6);
        yaw_out_line->setObjectName(QStringLiteral("yaw_out_line"));
        yaw_out_line->setFont(font);
        yaw_out_line->setReadOnly(false);

        formLayout_6->setWidget(2, QFormLayout::FieldRole, yaw_out_line);

        verticalLayoutWidget_3 = new QWidget(tab);
        verticalLayoutWidget_3->setObjectName(QStringLiteral("verticalLayoutWidget_3"));
        verticalLayoutWidget_3->setGeometry(QRect(70, 440, 311, 167));
        verticalLayout_3 = new QVBoxLayout(verticalLayoutWidget_3);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        clear_button = new QPushButton(verticalLayoutWidget_3);
        clear_button->setObjectName(QStringLiteral("clear_button"));

        horizontalLayout->addWidget(clear_button);

        scan_button = new QPushButton(verticalLayoutWidget_3);
        scan_button->setObjectName(QStringLiteral("scan_button"));

        horizontalLayout->addWidget(scan_button);

        dragon_prog = new QPushButton(verticalLayoutWidget_3);
        dragon_prog->setObjectName(QStringLiteral("dragon_prog"));

        horizontalLayout->addWidget(dragon_prog);


        verticalLayout_3->addLayout(horizontalLayout);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        pand_combobox = new QComboBox(verticalLayoutWidget_3);
        pand_combobox->setObjectName(QStringLiteral("pand_combobox"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(pand_combobox->sizePolicy().hasHeightForWidth());
        pand_combobox->setSizePolicy(sizePolicy2);

        horizontalLayout_3->addWidget(pand_combobox);

        pand_button = new QPushButton(verticalLayoutWidget_3);
        pand_button->setObjectName(QStringLiteral("pand_button"));

        horizontalLayout_3->addWidget(pand_button);


        verticalLayout_3->addLayout(horizontalLayout_3);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        ip_line = new QLineEdit(verticalLayoutWidget_3);
        ip_line->setObjectName(QStringLiteral("ip_line"));

        horizontalLayout_2->addWidget(ip_line);

        ip_button = new QPushButton(verticalLayoutWidget_3);
        ip_button->setObjectName(QStringLiteral("ip_button"));
        QFont font1;
        font1.setPointSize(8);
        ip_button->setFont(font1);

        horizontalLayout_2->addWidget(ip_button);


        verticalLayout_3->addLayout(horizontalLayout_2);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        sub_pro_lineEdit = new QLineEdit(verticalLayoutWidget_3);
        sub_pro_lineEdit->setObjectName(QStringLiteral("sub_pro_lineEdit"));

        horizontalLayout_4->addWidget(sub_pro_lineEdit);

        sub_start = new QPushButton(verticalLayoutWidget_3);
        sub_start->setObjectName(QStringLiteral("sub_start"));

        horizontalLayout_4->addWidget(sub_start);

        ctrl_C = new QPushButton(verticalLayoutWidget_3);
        ctrl_C->setObjectName(QStringLiteral("ctrl_C"));

        horizontalLayout_4->addWidget(ctrl_C);


        verticalLayout_3->addLayout(horizontalLayout_4);

        verticalLayoutWidget_4 = new QWidget(tab);
        verticalLayoutWidget_4->setObjectName(QStringLiteral("verticalLayoutWidget_4"));
        verticalLayoutWidget_4->setGeometry(QRect(40, 630, 363, 91));
        verticalLayout_5 = new QVBoxLayout(verticalLayoutWidget_4);
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setContentsMargins(11, 11, 11, 11);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        verticalLayout_5->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        label_7 = new QLabel(verticalLayoutWidget_4);
        label_7->setObjectName(QStringLiteral("label_7"));

        horizontalLayout_5->addWidget(label_7);

        lineEdit_gamma = new QLineEdit(verticalLayoutWidget_4);
        lineEdit_gamma->setObjectName(QStringLiteral("lineEdit_gamma"));

        horizontalLayout_5->addWidget(lineEdit_gamma);

        pushButton = new QPushButton(verticalLayoutWidget_4);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        horizontalLayout_5->addWidget(pushButton);


        verticalLayout_5->addLayout(horizontalLayout_5);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setSpacing(6);
        horizontalLayout_8->setObjectName(QStringLiteral("horizontalLayout_8"));
        label_8 = new QLabel(verticalLayoutWidget_4);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setAlignment(Qt::AlignCenter);

        horizontalLayout_8->addWidget(label_8);

        label_15 = new QLabel(verticalLayoutWidget_4);
        label_15->setObjectName(QStringLiteral("label_15"));
        label_15->setAlignment(Qt::AlignCenter);

        horizontalLayout_8->addWidget(label_15);

        label_10 = new QLabel(verticalLayoutWidget_4);
        label_10->setObjectName(QStringLiteral("label_10"));
        label_10->setAlignment(Qt::AlignCenter);

        horizontalLayout_8->addWidget(label_10);

        label_9 = new QLabel(verticalLayoutWidget_4);
        label_9->setObjectName(QStringLiteral("label_9"));
        label_9->setAlignment(Qt::AlignCenter);

        horizontalLayout_8->addWidget(label_9);

        label_16 = new QLabel(verticalLayoutWidget_4);
        label_16->setObjectName(QStringLiteral("label_16"));

        horizontalLayout_8->addWidget(label_16);


        verticalLayout_5->addLayout(horizontalLayout_8);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setSpacing(6);
        horizontalLayout_9->setObjectName(QStringLiteral("horizontalLayout_9"));
        lineEdit_a1 = new QLineEdit(verticalLayoutWidget_4);
        lineEdit_a1->setObjectName(QStringLiteral("lineEdit_a1"));

        horizontalLayout_9->addWidget(lineEdit_a1);

        lineEdit_a2 = new QLineEdit(verticalLayoutWidget_4);
        lineEdit_a2->setObjectName(QStringLiteral("lineEdit_a2"));

        horizontalLayout_9->addWidget(lineEdit_a2);

        lineEdit_a3 = new QLineEdit(verticalLayoutWidget_4);
        lineEdit_a3->setObjectName(QStringLiteral("lineEdit_a3"));

        horizontalLayout_9->addWidget(lineEdit_a3);

        lineEdit_a4 = new QLineEdit(verticalLayoutWidget_4);
        lineEdit_a4->setObjectName(QStringLiteral("lineEdit_a4"));

        horizontalLayout_9->addWidget(lineEdit_a4);

        pushButton_2 = new QPushButton(verticalLayoutWidget_4);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        QSizePolicy sizePolicy3(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(pushButton_2->sizePolicy().hasHeightForWidth());
        pushButton_2->setSizePolicy(sizePolicy3);
        pushButton_2->setIconSize(QSize(16, 16));

        horizontalLayout_9->addWidget(pushButton_2);


        verticalLayout_5->addLayout(horizontalLayout_9);

        PB_EMER = new QPushButton(tab);
        PB_EMER->setObjectName(QStringLiteral("PB_EMER"));
        PB_EMER->setGeometry(QRect(420, 660, 99, 27));
        verticalLayoutWidget_7 = new QWidget(tab);
        verticalLayoutWidget_7->setObjectName(QStringLiteral("verticalLayoutWidget_7"));
        verticalLayoutWidget_7->setGeometry(QRect(420, 440, 271, 211));
        verticalLayout_7 = new QVBoxLayout(verticalLayoutWidget_7);
        verticalLayout_7->setSpacing(6);
        verticalLayout_7->setContentsMargins(11, 11, 11, 11);
        verticalLayout_7->setObjectName(QStringLiteral("verticalLayout_7"));
        verticalLayout_7->setContentsMargins(0, 0, 0, 0);
        textEdit = new QTextEdit(verticalLayoutWidget_7);
        textEdit->setObjectName(QStringLiteral("textEdit"));
        textEdit->setInputMethodHints(Qt::ImhMultiLine);
        textEdit->setReadOnly(true);

        verticalLayout_7->addWidget(textEdit);

        lineEdit = new QLineEdit(verticalLayoutWidget_7);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));

        verticalLayout_7->addWidget(lineEdit);

        lineEdit_2 = new QLineEdit(tab);
        lineEdit_2->setObjectName(QStringLiteral("lineEdit_2"));
        lineEdit_2->setGeometry(QRect(580, 660, 113, 27));
        tabWidget->addTab(tab, QString());
        tab_6 = new QWidget();
        tab_6->setObjectName(QStringLiteral("tab_6"));
        gridLayoutWidget = new QWidget(tab_6);
        gridLayoutWidget->setObjectName(QStringLiteral("gridLayoutWidget"));
        gridLayoutWidget->setGeometry(QRect(60, 430, 391, 331));
        gl_Layout = new QGridLayout(gridLayoutWidget);
        gl_Layout->setSpacing(6);
        gl_Layout->setContentsMargins(11, 11, 11, 11);
        gl_Layout->setObjectName(QStringLiteral("gl_Layout"));
        gl_Layout->setContentsMargins(0, 0, 0, 0);
        customPlot_1 = new QCustomPlot(tab_6);
        customPlot_1->setObjectName(QStringLiteral("customPlot_1"));
        customPlot_1->setEnabled(true);
        customPlot_1->setGeometry(QRect(60, 30, 1081, 391));
        formLayoutWidget_9 = new QWidget(tab_6);
        formLayoutWidget_9->setObjectName(QStringLiteral("formLayoutWidget_9"));
        formLayoutWidget_9->setGeometry(QRect(770, 420, 371, 210));
        formLayout_9 = new QFormLayout(formLayoutWidget_9);
        formLayout_9->setSpacing(6);
        formLayout_9->setContentsMargins(11, 11, 11, 11);
        formLayout_9->setObjectName(QStringLiteral("formLayout_9"));
        formLayout_9->setContentsMargins(0, 0, 0, 0);
        label_17 = new QLabel(formLayoutWidget_9);
        label_17->setObjectName(QStringLiteral("label_17"));
        label_17->setFont(font);

        formLayout_9->setWidget(0, QFormLayout::LabelRole, label_17);

        roll_out_line_3 = new QLineEdit(formLayoutWidget_9);
        roll_out_line_3->setObjectName(QStringLiteral("roll_out_line_3"));
        roll_out_line_3->setFont(font);
        roll_out_line_3->setReadOnly(false);

        formLayout_9->setWidget(0, QFormLayout::FieldRole, roll_out_line_3);

        label_20 = new QLabel(formLayoutWidget_9);
        label_20->setObjectName(QStringLiteral("label_20"));
        label_20->setFont(font);

        formLayout_9->setWidget(1, QFormLayout::LabelRole, label_20);

        pitch_out_line_3 = new QLineEdit(formLayoutWidget_9);
        pitch_out_line_3->setObjectName(QStringLiteral("pitch_out_line_3"));
        pitch_out_line_3->setFont(font);
        pitch_out_line_3->setReadOnly(false);

        formLayout_9->setWidget(1, QFormLayout::FieldRole, pitch_out_line_3);

        label_21 = new QLabel(formLayoutWidget_9);
        label_21->setObjectName(QStringLiteral("label_21"));
        label_21->setFont(font);

        formLayout_9->setWidget(2, QFormLayout::LabelRole, label_21);

        yaw_out_line_3 = new QLineEdit(formLayoutWidget_9);
        yaw_out_line_3->setObjectName(QStringLiteral("yaw_out_line_3"));
        yaw_out_line_3->setFont(font);
        yaw_out_line_3->setReadOnly(false);

        formLayout_9->setWidget(2, QFormLayout::FieldRole, yaw_out_line_3);

        checkBox = new QCheckBox(formLayoutWidget_9);
        checkBox->setObjectName(QStringLiteral("checkBox"));

        formLayout_9->setWidget(3, QFormLayout::LabelRole, checkBox);

        horizontalLayoutWidget = new QWidget(tab_6);
        horizontalLayoutWidget->setObjectName(QStringLiteral("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(760, 640, 370, 80));
        horizontalLayout_15 = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout_15->setSpacing(6);
        horizontalLayout_15->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_15->setObjectName(QStringLiteral("horizontalLayout_15"));
        horizontalLayout_15->setContentsMargins(0, 0, 0, 0);
        pushButton_5 = new QPushButton(horizontalLayoutWidget);
        pushButton_5->setObjectName(QStringLiteral("pushButton_5"));

        horizontalLayout_15->addWidget(pushButton_5);

        pushButton_6 = new QPushButton(horizontalLayoutWidget);
        pushButton_6->setObjectName(QStringLiteral("pushButton_6"));

        horizontalLayout_15->addWidget(pushButton_6);

        pushButton_7 = new QPushButton(horizontalLayoutWidget);
        pushButton_7->setObjectName(QStringLiteral("pushButton_7"));

        horizontalLayout_15->addWidget(pushButton_7);

        PB_EMER_2 = new QPushButton(horizontalLayoutWidget);
        PB_EMER_2->setObjectName(QStringLiteral("PB_EMER_2"));

        horizontalLayout_15->addWidget(PB_EMER_2);

        tabWidget->addTab(tab_6, QString());
        MainWindow->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1188, 25));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        QWidget::setTabOrder(tabWidget, roll_out_line);
        QWidget::setTabOrder(roll_out_line, pitch_out_line);
        QWidget::setTabOrder(pitch_out_line, yaw_out_line);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        label_26->setText(QString());
        label_11->setText(QApplication::translate("MainWindow", "SENSOR", 0));
        label_18->setText(QApplication::translate("MainWindow", "STATE", 0));
        label_13->setText(QApplication::translate("MainWindow", "TARGET", 0));
        clear_button->setText(QApplication::translate("MainWindow", "clear", 0));
        scan_button->setText(QApplication::translate("MainWindow", "SCAN", 0));
        dragon_prog->setText(QApplication::translate("MainWindow", "Dragon frog", 0));
        pand_combobox->clear();
        pand_combobox->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "A0:14:3D:46:3A:73", 0)
         << QApplication::translate("MainWindow", "A0:14:3D:4F:9F:77", 0)
         << QApplication::translate("MainWindow", "A0:14:3D:6A:75:11", 0)
         << QApplication::translate("MainWindow", "A0:14:3D:50:E8:B2", 0)
         << QApplication::translate("MainWindow", "A0:14:3D:2A:4E:C8", 0)
        );
        pand_button->setText(QApplication::translate("MainWindow", "PAND", 0));
        ip_line->setText(QApplication::translate("MainWindow", "192.168.1.1", 0));
        ip_button->setText(QApplication::translate("MainWindow", "CONNECT", 0));
        sub_pro_lineEdit->setText(QApplication::translate("MainWindow", "/internal/main", 0));
        sub_start->setText(QApplication::translate("MainWindow", "START", 0));
        ctrl_C->setText(QApplication::translate("MainWindow", "SYS STOP", 0));
        label_7->setText(QApplication::translate("MainWindow", "Gamma", 0));
        lineEdit_gamma->setText(QApplication::translate("MainWindow", "0.8", 0));
        pushButton->setText(QApplication::translate("MainWindow", "SET", 0));
        label_8->setText(QApplication::translate("MainWindow", "INIT", 0));
        label_15->setText(QApplication::translate("MainWindow", "COUNT", 0));
        label_10->setText(QApplication::translate("MainWindow", "TIME", 0));
        label_9->setText(QApplication::translate("MainWindow", "TIME", 0));
        label_16->setText(QString());
        lineEdit_a1->setText(QApplication::translate("MainWindow", "3", 0));
        lineEdit_a2->setText(QApplication::translate("MainWindow", "2", 0));
        lineEdit_a3->setText(QApplication::translate("MainWindow", "2", 0));
        lineEdit_a4->setText(QApplication::translate("MainWindow", "2", 0));
        pushButton_2->setText(QApplication::translate("MainWindow", "START", 0));
        PB_EMER->setText(QApplication::translate("MainWindow", "emergency", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("MainWindow", "Mission1", 0));
        label_17->setText(QApplication::translate("MainWindow", "ROLL", 0));
        label_20->setText(QApplication::translate("MainWindow", "PITCH", 0));
        label_21->setText(QApplication::translate("MainWindow", "YAW", 0));
        checkBox->setText(QApplication::translate("MainWindow", "YAW", 0));
        pushButton_5->setText(QApplication::translate("MainWindow", "KF", 0));
        pushButton_6->setText(QApplication::translate("MainWindow", "EKF", 0));
        pushButton_7->setText(QApplication::translate("MainWindow", "START", 0));
        PB_EMER_2->setText(QApplication::translate("MainWindow", "emergency", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_6), QApplication::translate("MainWindow", "Mission3", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
