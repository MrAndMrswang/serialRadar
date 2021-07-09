#include "radar.h"
#include <math.h>
#include <QConicalGradient>
#include <QDebug>
#include <QPainter>
#include <QTimerEvent>

#define MAX_POINTS_SIZE 9999
//
Radar::Radar(QWidget *parent, int speed) : QWidget(parent), _speed(speed) {
  init();
}

// 初始化数据
void Radar::init() {
  //
  _pieRotate = 0;
  // 启动定时器
  _timerId = startTimer(_speed);
}

void Radar::paintBackground(QPainter *painter) {
  // 抗锯齿
  painter->setRenderHint(QPainter::Antialiasing);

  // 背景色
  painter->fillRect(rect(), Qt::blue);

  // 边长
  int len = _drawArea.width();

  // 底盘
  // x轴、y轴
  painter->setPen(QPen(Qt::white));
  painter->drawLine(_drawArea.topLeft() + QPoint(0, len / 2),
                    _drawArea.topRight() + QPoint(0, len / 2));
  painter->drawLine(_drawArea.topLeft() + QPoint(len / 2, 0),
                    _drawArea.bottomLeft() + QPoint(len / 2, 0));

  // 3个圆
  painter->drawEllipse(_drawArea.center(), len / 2, len / 2);
  painter->drawEllipse(_drawArea.center(), len / 3, len / 3);
  painter->drawEllipse(_drawArea.center(), len / 6, len / 6);
}

//
QPointF Radar::calcPoint(int length, int angle) {
  // 线
  qreal x =
      _drawArea.center().x() + (qreal)length / 2 * cos(-angle * M_PI / 180);
  qreal y =
      _drawArea.center().y() + (qreal)length / 2 * sin(-angle * M_PI / 180);
  return QPointF(x, y);
}

// 转动部分
void Radar::paintSector(QPainter *painter) {
  int len = _drawArea.width();
  // 线
  QPointF endPoint = calcPoint(len, _pieRotate);
  painter->setPen(QPen(Qt::white));
  painter->drawLine(_drawArea.center(), endPoint);

  // 扇形
  QConicalGradient gradient;
  gradient.setCenter(_drawArea.center());
  gradient.setAngle(_pieRotate + 180);
  // 从渐变角度开始0.5
  gradient.setColorAt(0.5, QColor(255, 255, 255, 100));
  // 0.75为扇形区域，由于Int类型计算不精确，将范围扩大到0.4-0.8
  gradient.setColorAt(1, QColor(255, 255, 255, 0));
  painter->setBrush(QBrush(gradient));
  painter->setPen(Qt::NoPen);
  painter->drawPie(_drawArea, _pieRotate * 16, 90 * 16);
}

//
void Radar::paintPoints(QPainter *painter) {
  for (int i = 0; i < _points.count(); ++i) {
    painter->setPen(QPen(QColor(255, 255, 255, 150), 3));
    painter->drawPoint(_points.at(i));
  }
}

//
void Radar::paintEvent(QPaintEvent *event) {
  QPainter painter(this);
  paintBackground(&painter);
  if (!_scanFlag) return;

  paintSector(&painter);
  paintPoints(&painter);
}

//
void Radar::resizeEvent(QResizeEvent *event) {
  // 以较短的边长作为绘制区域边长
  if (width() > height()) {
    _drawArea = QRect((width() - height()) / 2, 0, height(), height());
  } else {
    _drawArea = QRect(0, (height() - width()) / 2, width(), width());
  }
  // 留一些边框
  _drawArea.adjust(10, 10, -10, -10);
}

//
void Radar::timerEvent(QTimerEvent *event) {
  int currentId = event->timerId();
  if (_timerId == currentId) {
    _pieRotate -= 10;
  }

  update();
}

//
void Radar::setPoints(QList<QPoint> points) {
  qDebug() << "current points size:" << _points.size()
           << " add points size:" << points.size() << endl;
  if (points.size() == 0) {
    return;
  }

  // 如果现有点很多，需要删除
  if (_points.size() > MAX_POINTS_SIZE) {
    _points.clear();
  }
  _points.append(points);
}

//
void Radar::setScanFlag(bool flag) { _scanFlag = flag; }
