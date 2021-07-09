#ifndef SERIALCOM_H
#define SERIALCOM_H

#include <QWidget>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

namespace Ui {
class SerialCom;
}

class SerialCom : public QWidget {
  Q_OBJECT
 public:
  explicit SerialCom(QWidget *parent = nullptr);

 signals:
  void exportPoints(QList<QPoint> points);

 private slots:
  void onOpenSerialButtonClicked();
  void initData();
  void changeEditMod(bool flag);
  void invokeRPlidarDriver();

 private:
  QSerialPort *serial;
  Ui::SerialCom *ui;
  bool _canInvoke;
};

#endif  // SERIALCOM_H
