#ifndef RADAR_H
#define RADAR_H

#include <QPainter>
#include <QWidget>

class Radar : public QWidget {
  Q_OBJECT

 public:
  Radar(QWidget *parent, int speed = 50);
  ~Radar(){};

 protected:
  // 绘制事件
  void paintEvent(QPaintEvent *event);
  // 绘制背景
  void paintBackground(QPainter *painter);
  // 绘制扇形
  void paintSector(QPainter *painter);
  // 绘制点
  void paintPoints(QPainter *painter);
  // 大小重置事件
  void resizeEvent(QResizeEvent *event);
  // 定时器事件
  void timerEvent(QTimerEvent *event);

  QPointF calcPoint(int length, int angle);

 private:
  // 参数初始化
  void init();

  QRect _drawArea;        // 绘制区域
  int _pieRotate;         // 扇形旋转区域
  int _timerId;           // 定时器ID
  int _speed;             // 速度
  int _radius;            // 半径
  QList<QPoint> _points;  // 绘制点

 public slots:
  void setPoints(QList<QPoint> points);
};

#endif  // RADAR_H
