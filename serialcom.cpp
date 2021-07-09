#include "serialcom.h"
#include "ui_serialcom.h"

#include "sdk/sdk/include/rplidar.h"

#include <math.h>
#include <QDebug>

#ifndef _countof
#define _countof(_Array) (int)(sizeof(_Array) / sizeof(_Array[0]))
#endif

using namespace rp::standalone::rplidar;

SerialCom::SerialCom(QWidget *parent) : ui(new Ui::SerialCom) {
  setParent(parent);
  ui->setupUi(this);
  QObject::connect(ui->OpenSerialButton, &QPushButton::clicked, this,
                   &SerialCom::onOpenSerialButtonClicked);
  initData();
};

// 针对各数据框进行初始化
void SerialCom::initData() {
  qDebug() << "number of available ports:"
           << QSerialPortInfo::availablePorts().size();
  // 设定可用串口
  foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
    QSerialPort serial;
    serial.setPort(info);
    if (serial.open(QIODevice::ReadWrite)) {
      ui->PortBox->addItem(serial.portName());
      serial.close();
    }
  }
  // 设置波特率下拉菜单
  ui->BaudBox->addItem("1200", QSerialPort::Baud1200);
  ui->BaudBox->addItem("2400", QSerialPort::Baud2400);
  ui->BaudBox->addItem("4800", QSerialPort::Baud4800);
  ui->BaudBox->addItem("9600", QSerialPort::Baud9600);
  ui->BaudBox->addItem("19200", QSerialPort::Baud19200);
  ui->BaudBox->addItem("38400", QSerialPort::Baud38400);
  ui->BaudBox->addItem("57600", QSerialPort::Baud57600);
  ui->BaudBox->addItem("115200", QSerialPort::Baud115200);
  ui->BaudBox->setCurrentIndex(6);
}

void SerialCom::onOpenSerialButtonClicked() {
  if (ui->PortBox->isEnabled()) {
    changeEditMod(false);
  } else {
    //恢复设置使能
    changeEditMod(true);
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
  } else {
    tips0 = "关闭串口";
    _canInvoke = true;
  }
  ui->OpenSerialButton->setText(tips0);
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
      // drv->reset();
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
void SerialCom::invokeRPlidarDriver() {
  // create the driver instance
  RPlidarDriver *drv = RPlidarDriver::CreateDriver(DRIVER_TYPE_SERIALPORT);
  if (!drv) {
    qDebug() << "insufficent memory, exit" << endl;
    return;
  }

  //
  rplidar_response_device_info_t devinfo;

  char *portName = ui->PortBox->currentText().toUtf8().data();
  uint32_t baudRate = ui->BaudBox->currentData().toUInt();
  u_result op_result;
  //
  if (!drv) drv = RPlidarDriver::CreateDriver(DRIVER_TYPE_SERIALPORT);

  if (!IS_OK(drv->connect(portName, baudRate))) {
    qDebug() << "connect failed!!!" << endl;
    return;
  }

  if (!IS_OK(drv->getDeviceInfo(devinfo))) {
    delete drv;
    drv = NULL;
    qDebug() << "getDeviceInfo failed!!!" << endl;
    return;
  }

  if (!checkRPLIDARHealth(drv)) {
    qDebug() << "checkRPLIDARHealth failed!!!" << endl;
    return;
  }

  // start
  drv->startMotor();
  drv->startScan(0, 1);

  //
  double angle = 0, dist = 0, theta = 0, rho = 0;
  int x = 0, y = 0;
  //
  while (1) {
    if (_canInvoke) {
      break;
    }

    rplidar_response_measurement_node_hq_t nodes[8192];
    size_t count = _countof(nodes);
    op_result = drv->grabScanDataHq(nodes, count);
    if (!IS_OK(op_result)) {
      continue;
    }

    drv->ascendScanData(nodes, count);
    QList<QPoint> list0;
    for (int pos = 0; pos < (int)count; ++pos) {
      if (nodes[pos].dist_mm_q2 / 4.0f == 0) {
        continue;
      }

      angle = (nodes[pos].angle_z_q14 * 90.f / (1 << 14));
      dist = (nodes[pos].dist_mm_q2 / 4.0f);
      if (angle > 240 && angle < 300.00) {  // change angle
        theta = angle * M_PI / 180;
        rho = dist;
        x = (int)(rho * cos(theta));
        y = (int)(rho * sin(theta));

        qDebug() << "angle:" << angle << " dist:" << dist << " x:" << x
                 << " y:" << y << endl;
        list0.append(QPoint(x, y));
      }
    }

    //
    emit exportPoints(list0);
  }
  drv->stop();
  drv->stopMotor();

  RPlidarDriver::DisposeDriver(drv);
  drv = NULL;
}
