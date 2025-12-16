#pragma once

#include "Color.h"
#include "Point.h"
#include "Velocity.h"

class Painter;

/**
* Dust — маленькая визуальная частица, не влияющая на физику шаров.
* Хранит координаты, скорость и оставшееся время жизни (отображения).
*/
class Dust {

public:

    Dust() = default;

    Dust(const Point& center,
         const Velocity& velocity,
         double radius,
         const Color& color,
         double timeToLive);

    void update(double time);
    void draw(Painter& painter) const;
    bool isAlive() const;

private:

    Point center{};
    Velocity velocity{};
    double radius{2.};
    Color color{1., 1., 1.};
    double timeToLive{0.};
};
