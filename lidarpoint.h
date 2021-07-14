#ifndef LIDAR_INFO_H
#define LIDAR_INFO_H

// 激光雷达返回信息
class LidarPoint {
 public:
  LidarPoint(double angle, double dist, int x, int y)
      : _angle(angle), _dist(dist), _x(x), _y(y) {}

  double angle() const { return _angle; }
  double dist() const { return _dist; }
  int x() const { return _x; }
  int y() const { return _y; }

 private:
  double _angle;
  double _dist;
  int _x;
  int _y;
};

#endif  // LIDAR_INFO_H
