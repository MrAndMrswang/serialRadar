#ifndef SERIALCOM_H
#define SERIALCOM_H

#include <QWidget>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

namespace Ui {
class SerialCom;
}

class SerialCom : public QWidget {

 public:
  SerialCom(QWidget *parent);

 signals:
  void exportPoints(QList<QPoint> points);

 private slots:
  void onOpenSerialButtonClicked();
  void readData();
  void initData();
  void changeEditMod(bool flag);
  void invokeRPlidarDriver();

 private:
  QSerialPort *serial;
  Ui::SerialCom *ui;
  bool _canInvoke;
};

#endif  // SERIALCOM_H
