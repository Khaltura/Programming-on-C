#include "Ball.hpp"  
#include <cmath>     

// Конструктор класса Ball, инициализирует объект с заданными параметрами
Ball::Ball(const Velocity velocity, const Point center, double radius, const Color color, bool isCollidable): 
     velocity_(velocity), center_(center), radius_(radius), color_(color), isCollidable_(isCollidable) {}

// Задает скорость объекта
// @param velocity новое значение скорости
void Ball::setVelocity(const Velocity& velocity) {
    this->velocity_ = velocity;  // Устанавливает новую скорость для объекта
}

// @return скорость объекта
Velocity Ball::getVelocity() const {
    // TODO: место для доработки (может быть добавлена дополнительная логика)
    return velocity_;  // Возвращает текущую скорость объекта
}

// @brief Выполняет отрисовку объекта
// @details объект Ball абстрагирован от конкретного
// способа отображения пикселей на экране. Он "знаком"
// лишь с интерфейсом, который предоставляет Painter
// Рисование выполняется путем вызова painter.draw(...)
// @param painter контекст отрисовки
void Ball::draw(Painter& painter) const {
    painter.draw(center_, radius_, color_);  // Вызывает метод draw у объекта painter для отрисовки шара
}

// Задает координаты центра объекта
// @param center новый центр объекта
void Ball::setCenter(const Point& center) {
    this->center_ = center;  // Устанавливает новый центр для объекта
}

// @return центр объекта
Point Ball::getCenter() const {
    return center_;  // Возвращает текущие координаты центра объекта
}

// @brief Возвращает радиус объекта
// @details обратите внимание, что метод setRadius()
// не требуется (радиус задается только при создании)
double Ball::getRadius() const {
    return radius_;  // Возвращает радиус шара
}

// @brief Возвращает массу объекта
// @details В нашем приложении считаем, что все шары
// состоят из одинакового материала с фиксированной
// плотностью. В этом случае масса в условных единицах
// эквивалентна объему: PI * radius^3 * 4. / 3.
double Ball::getMass() const {
    return M_PI * pow(radius_, 3) * 4. / 3.;  // Вычисляет и возвращает массу шара по формуле объема сферы
}

Color Ball::getColor() const {
    return color_;  // Возвращает цвет шара
}

bool Ball::ifCollidable() const{
    return isCollidable_;  // Возвращает true, если шар может сталкиваться с другими объектами
}