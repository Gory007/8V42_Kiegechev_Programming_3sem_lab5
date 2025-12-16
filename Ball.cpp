#include "Ball.hpp"
#include "Painter.h"
#include <cmath>

/**
* Задает скорость объекта
* @param velocity новое значение скорости
*/
Ball::Ball(const Point& center,
           const Velocity& velocity,
           double radius,
           const Color& color,
           bool isCollidable)
    : center{center}
    , velocity{velocity}
    , radius{radius}
    , color{color}
    , collidable{isCollidable} {}

/**
* Задает скорость объекта
* @param velocity новое значение скорости
*/
void Ball::setVelocity(const Velocity& velocity) {

    // TODO: место для доработки
    this->velocity = velocity;

}

/**
* @return скорость объекта
*/
Velocity Ball::getVelocity() const {

    // TODO: место для доработки
    return velocity;

}

/**
* @brief Выполняет отрисовку объекта
* @details объект Ball абстрагирован от конкретного
* способа отображения пикселей на экране. Он "знаком"
* лишь с интерфейсом, который предоставляет Painter
* Рисование выполняется путем вызова painter.draw(...)
* @param painter контекст отрисовки
*/
void Ball::draw(Painter& painter) const {

    // TODO: место для доработки
    painter.draw(center, radius, color);

}

/**
* Задает координаты центра объекта
* @param center новый центр объекта
*/
void Ball::setCenter(const Point& center) {

    // TODO: место для доработки
    this->center = center;

}

/**
* @return центр объекта
*/
Point Ball::getCenter() const {

    // TODO: место для доработки
    return center;

}

/**
* @brief Возвращает радиус объекта
* @details обратите внимание, что метод setRadius()
* не требуется
*/
double Ball::getRadius() const {

    // TODO: место для доработки
    return radius;

}

/**
* @brief Возвращает массу объекта
* @details В нашем приложении считаем, что все шары
* состоят из одинакового материала с фиксированной
* плотностью. В этом случае масса в условных единицах
* эквивалентна объему: PI * radius^3 * 4. / 3.
*/
double Ball::getMass() const {

    // TODO: место для доработки
    const double pi = std::acos(-1.0);
    return pi * radius * radius * radius * 4.0 / 3.0;

}

bool Ball::isCollidable() const {
    return collidable;
}
