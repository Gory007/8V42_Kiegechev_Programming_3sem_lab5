#pragma once
#include "Ball.hpp"
#include <cstddef>
#include <vector>

class Physics {
  public:
    Physics(double timePerTick = 0.001);
    void setWorldBox(const Point& topLeft, const Point& bottomRight);
    void update(std::vector<Ball>& balls, size_t ticks) const;

    // Возвращает список точек столкновений, накопленных после последнего update(),
    // и очищает внутренний список.
    std::vector<Point> takeCollisions() const;

  private:
    void collideBalls(std::vector<Ball>& balls) const;
    void collideWithBox(std::vector<Ball>& balls) const;
    void move(std::vector<Ball>& balls) const;
    void processCollision(Ball& a, Ball& b,
                          double distanceBetweenCenters2) const;

  private:
    Point topLeft;
    Point bottomRight;
    double timePerTick;

    // Список столкновений за последний вызов update().
    // mutable — потому что update/processCollision объявлены const в шаблоне.
    mutable std::vector<Point> collisions;
};
