#include "World.h"
#include "Painter.h"
#include <cmath>
#include <algorithm>
#include <random>
#include <fstream>

// Длительность одного тика симуляции.
// Подробнее см. update()
// Изменять не следует
static constexpr double timePerTick = 0.001;

// Настройки визуального эффекта "пыли"
static constexpr size_t dustCountPerCollision = 14;
static constexpr double dustLifeTime = 0.35;
static constexpr double dustRadius = 10.0;

/**
 * Конструирует объект мира для симуляции
 * @param worldFilePath путь к файлу модели мира
 */
World::World(const std::string& worldFilePath) {

    std::ifstream stream(worldFilePath);
    /**
     * TODO: хорошее место для улучшения.
     * Чтение границ мира из модели
     * Обратите внимание, что здесь и далее мы многократно
     * читаем в объект типа Point, последовательно
     * заполняя координаты x и у. Если что-то делаем
     * многократно - хорошо бы вынести это в функцию
     * и не дублировать код...
     */
    stream >> topLeft >> bottomRight;
    physics.setWorldBox(topLeft, bottomRight);

    /**
     * TODO: хорошее место для улучшения.
     * (x, y) и (vx, vy) - составные части объекта, также
     * как и (red, green, blue). Опять же, можно упростить
     * этот код, научившись читать сразу Point, Color...
     */

    Point center;
    Point velocityVector;
    Color color;
    double radius;
    bool isCollidable;

    // Здесь не хватает обработки ошибок, но на текущем
    // уровне прохождения курса нас это устраивает
    while (stream >> center >> velocityVector >> color >> radius >> std::boolalpha >> isCollidable) {


        // TODO: место для доработки.
        // Здесь не хватает самого главного - создания
        // объекта класса Ball со свойствами, прочитанными
        // выше, и его помещения в контейнер balls

        // После того как мы каким-то образом
        // сконструируем объект Ball ball;
        // добавьте его в конец контейнера вызовом
        // balls.push_back(ball);
        Ball ball(center, Velocity(velocityVector), radius, color, isCollidable);
        balls.push_back(ball);
    }
}

/// @brief Отображает состояние мира
void World::show(Painter& painter) const {
    // Рисуем белый прямоугольник, отображающий границу
    // мира
    painter.draw(topLeft, bottomRight, Color(1, 1, 1));

    // Вызываем отрисовку каждого шара
    for (const Ball& ball : balls) {
        ball.draw(painter);
    }

    // Рисуем "пыль" поверх (чтобы эффект был заметнее)
    for (const Dust& d : dust) {
        d.draw(painter);
    }
}

/// @brief Обновляет состояние мира
void World::update(double time) {
    /**
     * В реальном мире время течет непрерывно. Однако
     * компьютеры дискретны по своей природе. Поэтому
     * симуляцию взаимодействия шаров выполняем дискретными
     * "тиками". Т.е. если с момента прошлой симуляции
     * прошло time секунд, time / timePerTick раз обновляем
     * состояние мира. Каждое такое обновление - тик -
     * в physics.update() перемещаем шары и обрабатываем
     * коллизии - ситуации, когда в результате перемещения
     * один шар пересекается с другим или с границей мира.
     * В общем случае время не делится нацело на
     * длительность тика, сохраняем остаток в restTime
     * и обрабатываем на следующей итерации.
     */

    // учитываем остаток времени, который мы не "доработали" при прошлом update
    time += restTime;
    const auto ticks = static_cast<size_t>(std::floor(time / timePerTick));
    restTime = time - double(ticks) * timePerTick;

    // Обновляем физику шаров
    physics.update(balls, ticks);

    // Время, которое реально "отсимулировали" в этом update
    const double simulatedTime = double(ticks) * timePerTick;

    // Обновляем пыль (движение + уменьшение времени жизни)
    for (Dust& d : dust) {
        d.update(simulatedTime);
    }

    // Удаляем частицы, у которых вышло время отображения
    dust.erase(std::remove_if(dust.begin(), dust.end(),
                              [](const Dust& d) { return !d.isAlive(); }),
               dust.end());

    // Генерируем новую "пыль" из списка коллизий, который собрал Physics
    const std::vector<Point> collisionPoints = physics.takeCollisions();

    // Небольшой детерминированный рандом, чтобы "пыль" выглядела живо,
    // но при этом поведение было воспроизводимым.
    static std::mt19937 rng(42);
    std::uniform_real_distribution<double> angleDist(0.0, 2.0 * std::acos(-1.0));
    std::uniform_real_distribution<double> speedDist(250.0, 900.0);

    for (const Point& p : collisionPoints) {
        for (size_t i = 0; i < dustCountPerCollision; ++i) {
            const double angle = angleDist(rng);
            const double speed = speedDist(rng);

            Dust d(p,
                   Velocity(speed, angle),
                   dustRadius,
                   Color(1.0, 0.9, 0.3),
                   dustLifeTime);

            dust.push_back(d);
        }
    }
}