#include "Physics.h"

#include <cmath>
#include <iterator>

double dot(const Point& lhs, const Point& rhs) {
    return lhs.x * rhs.x + lhs.y * rhs.y;
}

Physics::Physics(double timePerTick) : timePerTick{timePerTick} {}

void Physics::setWorldBox(const Point& topLeft, const Point& bottomRight) {
    this->topLeft = topLeft;
    this->bottomRight = bottomRight;
}

std::vector<Point> Physics::takeCollisions() const {
    std::vector<Point> result = collisions;
    collisions.clear();
    return result;
}

void Physics::update(std::vector<Ball>& balls, const size_t ticks) const {

    // Начинаем новый "кадр" коллизий
    collisions.clear();
    
    for (size_t i = 0; i < ticks; ++i) {
        move(balls);
        collideWithBox(balls);
        collideBalls(balls);
    }
}

void Physics::collideBalls(std::vector<Ball>& balls) const {
    for (auto a = balls.begin(); a != balls.end(); ++a) {
        for (auto b = std::next(a); b != balls.end(); ++b) {
            // Если хотя бы один шар не участвует в столкновениях,
            // пересечение не считаем коллизией
            if (!a->isCollidable() || !b->isCollidable()) {
                continue;
            }
            const double distanceBetweenCenters2 =
                distance2(a->getCenter(), b->getCenter());
            const double collisionDistance = a->getRadius() + b->getRadius();
            const double collisionDistance2 =
                collisionDistance * collisionDistance;

            if (distanceBetweenCenters2 < collisionDistance2) {
                processCollision(*a, *b, distanceBetweenCenters2);
            }
        }
    }
}

void Physics::collideWithBox(std::vector<Ball>& balls) const {
    for (Ball& ball : balls) {
        const Point p = ball.getCenter();
        const double r = ball.getRadius();
        Point vector = ball.getVelocity().vector();
        // определяет, находится ли v в диапазоне (lo, hi) (не включая границы)
        auto isOutOfRange = [](double v, double lo, double hi) {
            return v < lo || v > hi;
        };

        const double minX = topLeft.x + r;
        const double maxX = bottomRight.x - r;
        const double minY = topLeft.y + r;
        const double maxY = bottomRight.y - r;

        if (isOutOfRange(p.x, minX, maxX)) {

            if ((p.x < minX && vector.x < 0) || (p.x > maxX && vector.x > 0)) {
                vector.x = -vector.x;
                ball.setVelocity(vector);
            }

        } else if (isOutOfRange(p.y, minY, maxY)) {

            if ((p.y < minY && vector.y < 0) || (p.y > maxY && vector.y > 0)) {
                vector.y = -vector.y;
                ball.setVelocity(vector);
            }

        }

    }

}

void Physics::move(std::vector<Ball>& balls) const {
    for (Ball& ball : balls) {
        Point newPos =
            ball.getCenter() + ball.getVelocity().vector() * timePerTick;
        ball.setCenter(newPos);
    }
}

void Physics::processCollision(Ball& a, Ball& b,
                               double distanceBetweenCenters2) const {
    // нормированный вектор столкновения
    const Point normal =
        (b.getCenter() - a.getCenter()) / std::sqrt(distanceBetweenCenters2);

    // получаем скорость в векторном виде
    const Point aV = a.getVelocity().vector();
    const Point bV = b.getVelocity().vector();

    // коэффициент p учитывает скорость обоих мячей
    const double p =
        2 * (dot(aV, normal) - dot(bV, normal)) / (a.getMass() + b.getMass());

    // задаем новые скорости мячей после столкновения
    a.setVelocity(Velocity(aV - normal * p * a.getMass()));
    b.setVelocity(Velocity(bV + normal * p * b.getMass()));

    // Для визуального эффекта фиксируем точку столкновения
    collisions.push_back((a.getCenter() + b.getCenter()) / 2.0);
}
