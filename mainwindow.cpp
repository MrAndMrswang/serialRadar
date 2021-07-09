#include "mainwindow.h"
#include "radar.h"
#include "serialcom.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
  //
  SerialCom* serial = new SerialCom(this);
  setFixedSize(1000, 800);
  serial->show();

  //
  Radar* radar = new Radar(this, 50);
  radar->setFixedSize(400, 400);
  radar->move(400, 120);

  connect(serial, &SerialCom::exportPoints, radar, &Radar::setPoints);
}

MainWindow::~MainWindow() {}
