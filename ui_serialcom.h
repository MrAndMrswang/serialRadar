#ifndef UI_SERIALCOM_H
#define UI_SERIALCOM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

class Ui_SerialCom {
 public:
  QWidget *centralwidget;
  QGroupBox *groupBox;
  QPushButton *OpenSerialButton;
  QLabel *label;
  QLabel *label_2;
  QTextEdit *text0;
  QComboBox *BaudBox;
  QComboBox *PortBox;

  void setupUi(QWidget *widget) {
    if (widget->objectName().isEmpty())
      widget->setObjectName(QString::fromUtf8("widget"));
    widget->resize(800, 600);
    centralwidget = new QWidget(widget);
    centralwidget->setObjectName(QString::fromUtf8("centralwidget"));

    groupBox = new QGroupBox(centralwidget);
    groupBox->setObjectName(QString::fromUtf8("groupBox"));
    groupBox->setGeometry(QRect(30, 90, 250, 180));
    groupBox->setTitle("参数设置");
    groupBox->move(100, 100);
    //
    PortBox = new QComboBox(groupBox);
    PortBox->setObjectName(QString::fromUtf8("PortBox"));
    PortBox->setGeometry(QRect(100, 30, 133, 25));
    label = new QLabel(groupBox);
    label->setObjectName(QString::fromUtf8("label"));
    label->setGeometry(QRect(20, 30, 104, 25));
    label->setText("串口名称");

    //
    BaudBox = new QComboBox(groupBox);
    BaudBox->setObjectName(QString::fromUtf8("BaudBox"));
    BaudBox->setGeometry(QRect(100, 73, 133, 25));
    label_2 = new QLabel(groupBox);
    label_2->setObjectName(QString::fromUtf8("label_2"));
    label_2->setGeometry(QRect(20, 73, 104, 25));
    label_2->setText("波特率");

    //
    OpenSerialButton = new QPushButton(groupBox);
    OpenSerialButton->setObjectName(QString::fromUtf8("OpenSerialButton"));
    OpenSerialButton->setGeometry(QRect(20, 140, 213, 30));
    OpenSerialButton->setText("打开串口");

    //
    text0 = new QTextEdit(widget);
    text0->setObjectName(QString::fromUtf8("textEdit"));
    text0->setGeometry(QRect(30, 350, 341, 131));
    QMetaObject::connectSlotsByName(widget);
  }  // setupUi
};

namespace Ui {
class SerialCom : public Ui_SerialCom {};
}  // namespace Ui

#endif  // UI_SERIALCOM_H
