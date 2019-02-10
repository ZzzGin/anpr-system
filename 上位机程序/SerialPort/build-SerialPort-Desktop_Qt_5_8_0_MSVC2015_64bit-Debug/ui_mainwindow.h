/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout_7;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QTextEdit *textEdit;
    QHBoxLayout *horizontalLayout;
    QPushButton *clearButton;
    QPushButton *sendButton;
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout_3;
    QLabel *label_8;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_3;
    QComboBox *PortBox;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_4;
    QComboBox *BaudBox;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_5;
    QComboBox *BitNumBox;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_6;
    QComboBox *ParityBox;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_7;
    QComboBox *StopBox;
    QSpacerItem *verticalSpacer;
    QLabel *label_ifc;
    QPushButton *openButton;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(496, 311);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        layoutWidget = new QWidget(centralWidget);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(30, 10, 431, 281));
        horizontalLayout_7 = new QHBoxLayout(layoutWidget);
        horizontalLayout_7->setSpacing(6);
        horizontalLayout_7->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        horizontalLayout_7->setContentsMargins(0, 0, 0, 0);
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        label = new QLabel(layoutWidget);
        label->setObjectName(QStringLiteral("label"));

        verticalLayout->addWidget(label);

        textEdit = new QTextEdit(layoutWidget);
        textEdit->setObjectName(QStringLiteral("textEdit"));

        verticalLayout->addWidget(textEdit);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        clearButton = new QPushButton(layoutWidget);
        clearButton->setObjectName(QStringLiteral("clearButton"));

        horizontalLayout->addWidget(clearButton);

        sendButton = new QPushButton(layoutWidget);
        sendButton->setObjectName(QStringLiteral("sendButton"));

        horizontalLayout->addWidget(sendButton);


        verticalLayout->addLayout(horizontalLayout);


        horizontalLayout_7->addLayout(verticalLayout);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        label_8 = new QLabel(layoutWidget);
        label_8->setObjectName(QStringLiteral("label_8"));

        verticalLayout_3->addWidget(label_8);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        label_3 = new QLabel(layoutWidget);
        label_3->setObjectName(QStringLiteral("label_3"));

        horizontalLayout_2->addWidget(label_3);

        PortBox = new QComboBox(layoutWidget);
        PortBox->setObjectName(QStringLiteral("PortBox"));

        horizontalLayout_2->addWidget(PortBox);


        verticalLayout_3->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        label_4 = new QLabel(layoutWidget);
        label_4->setObjectName(QStringLiteral("label_4"));

        horizontalLayout_3->addWidget(label_4);

        BaudBox = new QComboBox(layoutWidget);
        BaudBox->setObjectName(QStringLiteral("BaudBox"));

        horizontalLayout_3->addWidget(BaudBox);


        verticalLayout_3->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        label_5 = new QLabel(layoutWidget);
        label_5->setObjectName(QStringLiteral("label_5"));

        horizontalLayout_4->addWidget(label_5);

        BitNumBox = new QComboBox(layoutWidget);
        BitNumBox->setObjectName(QStringLiteral("BitNumBox"));

        horizontalLayout_4->addWidget(BitNumBox);


        verticalLayout_3->addLayout(horizontalLayout_4);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        label_6 = new QLabel(layoutWidget);
        label_6->setObjectName(QStringLiteral("label_6"));

        horizontalLayout_5->addWidget(label_6);

        ParityBox = new QComboBox(layoutWidget);
        ParityBox->setObjectName(QStringLiteral("ParityBox"));

        horizontalLayout_5->addWidget(ParityBox);


        verticalLayout_3->addLayout(horizontalLayout_5);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        label_7 = new QLabel(layoutWidget);
        label_7->setObjectName(QStringLiteral("label_7"));

        horizontalLayout_6->addWidget(label_7);

        StopBox = new QComboBox(layoutWidget);
        StopBox->setObjectName(QStringLiteral("StopBox"));

        horizontalLayout_6->addWidget(StopBox);


        verticalLayout_3->addLayout(horizontalLayout_6);


        verticalLayout_2->addLayout(verticalLayout_3);

        verticalSpacer = new QSpacerItem(20, 68, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);

        label_ifc = new QLabel(layoutWidget);
        label_ifc->setObjectName(QStringLiteral("label_ifc"));

        verticalLayout_2->addWidget(label_ifc);

        openButton = new QPushButton(layoutWidget);
        openButton->setObjectName(QStringLiteral("openButton"));

        verticalLayout_2->addWidget(openButton);


        horizontalLayout_7->addLayout(verticalLayout_2);

        MainWindow->setCentralWidget(centralWidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", Q_NULLPTR));
        label->setText(QApplication::translate("MainWindow", "\346\266\210\346\201\257\347\252\227\345\217\243", Q_NULLPTR));
        clearButton->setText(QApplication::translate("MainWindow", "\346\270\205\347\251\272\346\266\210\346\201\257", Q_NULLPTR));
        sendButton->setText(QApplication::translate("MainWindow", "\346\265\213\350\257\225\350\277\236\351\200\232\346\200\247", Q_NULLPTR));
        label_8->setText(QString());
        label_3->setText(QApplication::translate("MainWindow", "\347\253\257\345\217\243\345\217\267\357\274\232", Q_NULLPTR));
        label_4->setText(QApplication::translate("MainWindow", "\346\263\242\347\211\271\347\216\207\357\274\232", Q_NULLPTR));
        BaudBox->clear();
        BaudBox->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "1200", Q_NULLPTR)
         << QApplication::translate("MainWindow", "2400", Q_NULLPTR)
         << QApplication::translate("MainWindow", "4800", Q_NULLPTR)
         << QApplication::translate("MainWindow", "9600", Q_NULLPTR)
         << QApplication::translate("MainWindow", "19200", Q_NULLPTR)
         << QApplication::translate("MainWindow", "115200", Q_NULLPTR)
        );
        label_5->setText(QApplication::translate("MainWindow", "\346\225\260\346\215\256\344\275\215\357\274\232", Q_NULLPTR));
        BitNumBox->clear();
        BitNumBox->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "8", Q_NULLPTR)
        );
        label_6->setText(QApplication::translate("MainWindow", "\346\240\241\351\252\214\344\275\215\357\274\232", Q_NULLPTR));
        ParityBox->clear();
        ParityBox->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "0", Q_NULLPTR)
        );
        label_7->setText(QApplication::translate("MainWindow", "\345\201\234\346\255\242\344\275\215\357\274\232", Q_NULLPTR));
        StopBox->clear();
        StopBox->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "1", Q_NULLPTR)
         << QApplication::translate("MainWindow", "2", Q_NULLPTR)
        );
        label_ifc->setText(QApplication::translate("MainWindow", "Not Connected", Q_NULLPTR));
        openButton->setText(QApplication::translate("MainWindow", "\346\211\223\345\274\200\344\270\262\345\217\243", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
