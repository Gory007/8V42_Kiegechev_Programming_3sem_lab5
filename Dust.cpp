#include "Dust.h"

#include "Painter.h"

Dust::Dust(const Point& center,
           const Velocity& velocity,
           double radius,
           const Color& color,
           double timeToLive)
    : center{center}
    , velocity{velocity}
    , radius{radius}
    , color{color}
    , timeToLive{timeToLive} {}

void Dust::update(double time) {
    // Для простоты игнорируем любые коллизии:
    // просто двигаем частицу по скорости и уменьшаем время жизни.
    center = center + velocity.vector() * time;
    timeToLive -= time;
}

void Dust::draw(Painter& painter) const {
    painter.draw(center, radius, color);
}

bool Dust::isAlive() const {
    return timeToLive > 0.;
}
