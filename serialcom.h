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
  void exportPoints(QList<QPoint>);
  void startScan(bool);

 private:
  QSerialPort *serial;
  Ui::SerialCom *ui;
  bool _canInvoke;

  void init();
  void onOpenSerialButtonClicked();
  void changeEditMod(bool);
  void invokeRPlidarDriver();
};

#endif  // SERIALCOM_H
