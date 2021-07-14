#ifndef SERIALCOM_H
#define SERIALCOM_H

#include <QThread>
#include <QWidget>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

namespace Ui {
class SerialCom;
}

class SerialCom : public QWidget {
  Q_OBJECT
  //
  class InvokeThread : public QThread {
   public:
    void run();
    void doInvoke();
    QList<QPoint> getPoints();
    void savePoints(QList<QPoint> list0);
    void setPortName(QString portName);
    void setBaudRate(uint32_t baudRate);
    void startInvoke();
    void stopInvoke();
    void clearPoints();

   private:
    QString _portName;
    uint32_t _baudRate;
    QList<QPoint> _points;  // 绘制点
    bool _startInvoke;
  };

  //
 public:
  explicit SerialCom(QWidget *parent = nullptr, int _exportInterval = 500);

 signals:
  void exportPoints(QList<QPoint>);
  void startScan(bool);

 protected:
  // 定时器事件
  void timerEvent(QTimerEvent *event);

 private:
  QSerialPort *serial;
  Ui::SerialCom *ui;
  bool _canInvoke;
  int _timerId;  // 定时器ID
  int _exportInterval;
  InvokeThread _invokeThread0;
  int _exportCnt;  // 导出数据的计数器

  void init();
  void onOpenSerialButtonClicked();
  void changeEditMod(bool);
  void startInvoke();
  void stopInvoke();
  void savePoints(QList<QPoint>);
};

#endif  // SERIALCOM_H
