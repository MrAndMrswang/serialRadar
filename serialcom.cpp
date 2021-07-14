#include <math.h>
#include <unistd.h>
#include <QDebug>
#include <QMutex>

#include "sdk/sdk/include/rplidar.h"

#include "serialcom.h"
#include "ui_serialcom.h"

QMutex mutexlock0;

#ifndef _countof
#define _countof(_Array) (int)(sizeof(_Array) / sizeof(_Array[0]))
#endif

using namespace rp::standalone::rplidar;

SerialCom::SerialCom(QWidget *parent, int _exportInterval)
    : ui(new Ui::SerialCom),
      _canInvoke(false),
      _exportInterval(_exportInterval),
      _exportCnt(0) {
  setParent(parent);
  _invokeThread0.start();
  init();
};

// 针对各数据框进行初始化
void SerialCom::init() {
  //
  ui->setupUi(this);
  QObject::connect(ui->OpenSerialButton, &QPushButton::clicked, this,
                   &SerialCom::onOpenSerialButtonClicked);

  //
  qDebug() << "number of available ports:"
           << QSerialPortInfo::availablePorts().size();
  // 设定可用串口
  foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
    ui->PortBox->addItem(info.portName());
    qDebug() << "check port name:" << info.portName() << endl;
  }
  ui->PortBox->addItem("/dev/ttyUSB0");
  // 设置波特率下拉菜单
  ui->BaudBox->addItem("115200", QSerialPort::Baud115200);
  ui->BaudBox->addItem("256000", 256000);
  ui->BaudBox->setCurrentIndex(0);

  // 启动定时器
  _timerId = startTimer(_exportInterval);
}

void SerialCom::onOpenSerialButtonClicked() {
  if (ui->PortBox->isEnabled()) {
    changeEditMod(false);
    emit startScan(true);
  } else {
    //恢复设置使能
    changeEditMod(true);
    emit startScan(false);
  }
}

// 改变编辑框状态
void SerialCom::changeEditMod(bool flag) {
  ui->PortBox->setEnabled(flag);
  ui->BaudBox->setEnabled(flag);
  QString tips0;
  if (flag) {
    tips0 = "打开串口";
    _canInvoke = false;
    stopInvoke();
  } else {
    tips0 = "关闭串口";
    _canInvoke = true;
    startInvoke();
  }
  ui->OpenSerialButton->setText(tips0);
}

//
void SerialCom::startInvoke() {
  _invokeThread0.setPortName(ui->PortBox->currentText());
  _invokeThread0.setBaudRate(ui->BaudBox->currentData().toUInt());
  _invokeThread0.startInvoke();
}

//
void SerialCom::stopInvoke() {
  _invokeThread0.stopInvoke();
  //  ui->text0->clear();
}

//
void SerialCom::timerEvent(QTimerEvent *event) {
  int currentId = event->timerId();
  if (_timerId != currentId || !_canInvoke) {
    return;
  }
  // 获取点信息
  QList<LidarPoint> list0 = _invokeThread0.getPoints();
  _exportCnt++;
  showPoints(list0);

  // 导出点信息
  emit exportPoints(list0);
  // 刷新点
  if (_exportCnt % 20 == 0) {
    _invokeThread0.clearPoints();
  }
}

void SerialCom::showPoints(const QList<LidarPoint> &list0) {
  //
  QString s0;
  for (int i = 0; i < list0.size(); ++i) {
    QString status = QString("angle: %1, dist: %2, x: %3, y: %4\n")
                         .arg(list0.at(i).angle())
                         .arg(list0.at(i).dist())
                         .arg(list0.at(i).x())
                         .arg(list0.at(i).y());
    s0 += status;
  }
  ui->text0->setText(s0);
}

// InvokeThread 类定义

void SerialCom::InvokeThread::run() {
  while (1) {
    if (!_startInvoke) {
      continue;
    }
    doInvoke();
  }
}

void SerialCom::InvokeThread::savePoints(QList<LidarPoint> list0) {
  mutexlock0.lock();
  for (int i = 0; i < list0.size(); ++i) {
    bool equal(false);
    for (int j = 0; j < _points.size(); ++j) {
      if (_points[j].x() == list0[i].x() && _points[j].y() == list0[i].y()) {
        equal = true;
        break;
      }
    }
    if (!equal) {
      _points.append(list0[i]);
    }
  }
  mutexlock0.unlock();
}

void SerialCom::InvokeThread::clearPoints() {
  mutexlock0.lock();
  _points.clear();
  mutexlock0.unlock();
}

void SerialCom::InvokeThread::setPortName(QString portName) {
  _portName = portName;
}
void SerialCom::InvokeThread::setBaudRate(uint32_t baudRate) {
  _baudRate = baudRate;
}
void SerialCom::InvokeThread::startInvoke() { _startInvoke = true; }
void SerialCom::InvokeThread::stopInvoke() { _startInvoke = false; }

QList<LidarPoint> SerialCom::InvokeThread::getPoints() {
  return QList<LidarPoint>(_points);
}

//
bool checkRPLIDARHealth(RPlidarDriver *drv) {
  u_result op_result;
  rplidar_response_device_health_t healthinfo;

  op_result = drv->getHealth(healthinfo);
  if (IS_OK(op_result)) {  // the macro IS_OK is the preperred way to judge
    // whether the operation is succeed.
    qDebug() << "RPLidar health status :" << healthinfo.status << endl;
    if (healthinfo.status == RPLIDAR_STATUS_ERROR) {
      qDebug() << "Error, rplidar internal error detected. Please reboot the "
                  "device to retry."
               << endl;
      // enable the following code if you want rplidar to be reboot by software
      //      drv->reset();
      return false;
    } else {
      return true;
    }

  } else {
    qDebug() << "Error, cannot retrieve the lidar health code: " << op_result
             << endl;
    return false;
  }
}

//
void SerialCom::InvokeThread::doInvoke() {
  //    create the driver instance
  RPlidarDriver *drv = RPlidarDriver::CreateDriver(DRIVER_TYPE_SERIALPORT);
  if (!drv) {
    qDebug() << "insufficent memory, exit" << endl;
    return;
  }

  //
  rplidar_response_device_info_t devinfo;
  u_result op_result;
  char *p0 = const_cast<char *>(_portName.toStdString().c_str());
  qDebug() << "start connect|" << p0 << "|" << _baudRate << endl;

  if (!drv) drv = RPlidarDriver::CreateDriver(DRIVER_TYPE_SERIALPORT);
  op_result = drv->connect(p0, _baudRate);
  if (!IS_OK(op_result)) {
    qDebug() << "ERROR|connect failed|res:" << op_result << endl;
    return;
  }

  op_result = drv->getDeviceInfo(devinfo);
  if (!IS_OK(op_result)) {
    delete drv;
    drv = NULL;
    qDebug() << "ERROR|getDeviceInfo failed|res:" << op_result << endl;
    return;
  }

  if (!checkRPLIDARHealth(drv)) {
    qDebug() << "ERROR|checkRPLIDARHealth failed!!!" << endl;
    return;
  }

  // start
  op_result = drv->startMotor();
  if (!IS_OK(op_result)) {
    qDebug() << "ERROR|startMotor|res:" << op_result << endl;
  }
  op_result = drv->startScan(0, 1);
  if (!IS_OK(op_result)) {
    qDebug() << "ERROR|startScan|res:" << op_result << endl;
  }

  //
  double angle = 0, dist = 0, theta = 0, rho = 0;
  int x = 0, y = 0;

  while (1) {
    if (!_startInvoke) {
      qDebug() << "doInvoke() stop invoke" << endl;
      break;
    }
    rplidar_response_measurement_node_hq_t nodes[8192];
    size_t count = _countof(nodes);
    op_result = drv->grabScanDataHq(nodes, count);
    if (!IS_OK(op_result)) {
      qDebug() << "ERROR|grabScanDataHq failed|res:" << op_result << endl;
      continue;
    }
    qDebug() << "grabScanDataHq|count:" << count << endl;

    op_result = drv->ascendScanData(nodes, count);
    if (!IS_OK(op_result)) {
      qDebug() << "ERROR|ascendScanData|res:" << op_result << endl;
    }
    qDebug() << "ascendScanData|count:" << count << endl;

    QList<LidarPoint> list0;
    for (int pos = 0; pos < (int)count; ++pos) {
      if (nodes[pos].dist_mm_q2 / 4.0f == 0) {
        continue;
      }

      angle = (nodes[pos].angle_z_q14 * 90.f / (1 << 14));
      dist = (nodes[pos].dist_mm_q2 / 4.0f);

      theta = angle * M_PI / 180;
      rho = dist;
      x = (int)(rho * cos(theta));
      y = (int)(rho * sin(theta));
      qDebug() << "angle:" << angle << " dist:" << dist << " x:" << x
               << " y:" << y << endl;
      list0.append(LidarPoint(angle, dist, x, y));
    }
    qDebug() << "savePoints|list size:" << list0.size() << endl;

    savePoints(list0);
  }
  drv->stop();
  drv->stopMotor();

  RPlidarDriver::DisposeDriver(drv);
  drv = NULL;
}
