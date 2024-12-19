#include "Dust.hpp"  
#include <cmath>     

// Конструктор класса Dust, инициализирует объект с заданными параметрами
Dust::Dust(const Velocity velocity, const Point center, double radius, const Color color, bool isCollidable, double lifetime)
    : velocity_(velocity), center_(center), radius_(radius), color_(color), isCollidable_(isCollidable), lifetime_(lifetime) {}

// Задает скорость объекта
// @param velocity новое значение скорости
void Dust::setVelocity(const Velocity& velocity) {
    this->velocity_ = velocity;  // Устанавливает новую скорость для объекта
}

/**
 * @return скорость объекта
 */
Velocity Dust::getVelocity() const {
    // TODO: место для доработки (может быть добавлена дополнительная логика)
    return velocity_;  // Возвращает текущую скорость объекта
}

/**
 * @brief Выполняет отрисовку объекта
 * @details объект Dust абстрагирован от конкретного
 * способа отображения пикселей на экране. Он "знаком"
 * лишь с интерфейсом, который предоставляет Painter.
 * Рисование выполняется путем вызова painter.draw(...)
 * @param painter контекст отрисовки
 */
void Dust::draw(Painter& painter) const {
    painter.draw(center_, radius_, color_);  // Вызывает метод draw у объекта painter для отрисовки пыли
}

/**
 * Задает координаты центра объекта
 * @param center новый центр объекта
 */
void Dust::setCenter(const Point& center) {
    this->center_ = center;  // Устанавливает новый центр для объекта
}

/**
 * @return центр объекта
 */
Point Dust::getCenter() const {
    return center_;  // Возвращает текущие координаты центра объекта
}

/**
 * @brief Возвращает радиус объекта
 * @details обратите внимание, что метод setRadius()
 * не требуется (радиус задается только при создании)
 */
double Dust::getRadius() const {
    return radius_;  // Возвращает радиус пыли
}

/**
 * @brief Возвращает массу объекта
 * @details В нашем приложении считаем, что все частицы пыли
 * состоят из одинакового материала с фиксированной
 * плотностью. В этом случае масса в условных единицах
 * эквивалентна объему: PI * radius^3 * 4. / 3.
 */
double Dust::getMass() const {
    return M_PI * pow(radius_, 3) * 4. / 3.;  // Вычисляет и возвращает массу частицы пыли по формуле объема сферы
}

// Метод для проверки, жива ли частица пыли
bool Dust::ifAlive() const {
    return isAlive_;  // Возвращает true, если частица пыли жива, иначе false
}

// Метод для обновления времени жизни частицы пыли 
void Dust::dying(double time) {
    lifetime_ -= time;  // Уменьшает время жизни на переданное значение time
    if (lifetime_ <= 0) { 
        isAlive_ = false;  // Если время жизни истекло, устанавливает isAlive_ в false 
    }
}