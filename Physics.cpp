#include "Physics.h"  // Подключение заголовочного файла Physics.h, который определяет класс Physics

// Функция для вычисления скалярного произведения двух векторов (точек)
double dot(const Point& lhs, const Point& rhs) {
    return lhs.x * rhs.x + lhs.y * rhs.y;  // Возвращает сумму произведений соответствующих координат
}

// Конструктор класса Physics, инициализирует объект с заданным временем на тик
Physics::Physics(double timePerTick) : timePerTick{timePerTick} {}  // Инициализация переменной timePerTick

// Метод для установки границ мира
void Physics::setWorldBox(const Point& topLeft, const Point& bottomRight) {
    this->topLeft = topLeft;           // Установка верхнего левого угла области мира
    this->bottomRight = bottomRight;   // Установка нижнего правого угла области мира
}

// Метод для обновления состояния шаров и частиц пыли в зависимости от количества тиков
void Physics::update(std::vector<Ball>& balls, std::vector<Dust>& dusts, const size_t ticks) {
    for (size_t i = 0; i < ticks; ++i) {  // Цикл по количеству тиков
        move(balls);                       // Перемещение шаров
        moveDusts(dusts);                  // Перемещение частиц пыли
        collideWithBox(balls, dusts);      // Проверка столкновений с границами мира
        collideBalls(balls, dusts);        // Проверка столкновений между шарами
    }
}

// Метод для обработки столкновений между шарами
void Physics::collideBalls(std::vector<Ball>& balls, std::vector<Dust>& dusts) const {
    for (auto a = balls.begin(); a != balls.end(); ++a) {  // Внешний цикл по всем шарам
        for (auto b = std::next(a); b != balls.end(); ++b) {  // Внутренний цикл по всем шарам после a
            if (a->ifCollidable() && b->ifCollidable()) {  // Проверка, могут ли шары сталкиваться
                const double distanceBetweenCenters2 = distance2(a->getCenter(), b->getCenter());  // Вычисление квадратного расстояния между центрами шаров
                const double collisionDistance = a->getRadius() + b->getRadius();  // Вычисление расстояния столкновения
                const double collisionDistance2 = collisionDistance * collisionDistance;  // Квадрат расстояния столкновения

                if (distanceBetweenCenters2 < collisionDistance2) {  // Проверка на столкновение
                    processCollision(*a, *b, distanceBetweenCenters2);  // Обработка столкновения между шарами

                    for (int i = 0; i < 10; i++) {  // Генерация частиц пыли при столкновении
                        double angle = (i * 2 * M_PI / 10);  // Вычисление угла для каждой частицы
                        Velocity velocity(100 * i, angle);   // Создание скорости для частицы пыли
                        Dust dust(velocity, a->getCenter(), 8, a->getColor(), false, 0.8);  // Создание объекта Dust с заданными параметрами
                        dusts.push_back(dust);   // Добавление частицы пыли в вектор dusts
                    }
                }
            }
        }
    }
}

// Метод для проверки столкновений шаров с границами мира
void Physics::collideWithBox(std::vector<Ball>& balls, std::vector<Dust>& dusts) const {
    for (Ball& ball : balls) {  // Цикл по всем шарам в векторе balls
        const Point p = ball.getCenter();   // Получение текущего центра шара
        const double r = ball.getRadius();   // Получение радиуса шара

        // Определяет, находится ли v в диапазоне (lo, hi) (не включая границы)
        auto isOutOfRange = [](double v, double lo, double hi) {
            return v < lo || v > hi;   // Возвращает true, если v выходит за пределы диапазона [lo, hi]
        };

        if (isOutOfRange(p.x, topLeft.x + r, bottomRight.x - r)) {  // Проверка выхода за пределы по оси x
            Point vector = ball.getVelocity().vector();   // Получение текущей скорости шара
            vector.x = -vector.x;                          // Инвертирование компоненты скорости по оси x
            ball.setVelocity(vector);                       // Установка новой скорости шара
            
            if (ball.ifCollidable()) {                     // Проверка на возможность столкновения шара
                for (int i = 0; i < 10; i++) {            // Генерация частиц пыли при столкновении со стеной
                    double angle = (i * 2 * M_PI / 10);   // Вычисление угла для каждой частицы пыли
                    Velocity velocity(100 * i, angle);    // Создание скорости для частицы пыли
                    Dust dust(velocity, ball.getCenter(), 8, ball.getColor(), true, 0.8);  // Создание объекта Dust с заданными параметрами
                    dusts.push_back(dust);                 // Добавление частицы пыли в вектор dusts 
                }
            }
        } else if (isOutOfRange(p.y, topLeft.y + r, bottomRight.y - r)) {  // Проверка выхода за пределы по оси y
            Point vector = ball.getVelocity().vector();   // Получение текущей скорости шара
            vector.y = -vector.y;                          // Инвертирование компоненты скорости по оси y
            ball.setVelocity(vector);                       // Установка новой скорости шара
            
            if (ball.ifCollidable()) {                     // Проверка на возможность столкновения шара 
                for (int i = 0; i < 10; i++) {            // Генерация частиц пыли при столкновении со стеной 
                    double angle = (i * 2 * M_PI / 10);   // Вычисление угла для каждой частицы пыли 
                    Velocity velocity(100 * i, angle);    // Создание скорости для частицы пыли 
                    Dust dust(velocity, ball.getCenter(), 8, ball.getColor(), true, 0.8);  // Создание объекта Dust с заданными параметрами 
                    dusts.push_back(dust);                 // Добавление частицы пыли в вектор dusts 
                }
            }
        }
    }
}

// Метод для перемещения шаров на основе их скорости и времени на тик
void Physics::move(std::vector<Ball>& balls) const {
    for (Ball& ball : balls) {   // Цикл по всем шарам в векторе balls 
        Point newPos =
            ball.getCenter() + ball.getVelocity().vector() * timePerTick;   // Вычисление новой позиции шара на основе его скорости и времени на тик 
        ball.setCenter(newPos);   // Установка новой позиции шара 
    }
}

// Метод для обработки столкновения между двумя шарами 
void Physics::processCollision(Ball& a, Ball& b, double distanceBetweenCenters2) const {
    const Point normal =(b.getCenter() - a.getCenter()) / std::sqrt(distanceBetweenCenters2);   // Нормализованный вектор столкновения

    const Point aV = a.getVelocity().vector();   // Получение скорости первого шара в виде вектора 
    const Point bV = b.getVelocity().vector();   // Получение скорости второго шара в виде вектора 

    if (a.ifCollidable() == true && b.ifCollidable() == true) {   // Проверка возможности столкновения обоих шаров 
        const double p = 2 * (dot(aV, normal) - dot(bV, normal)) / (a.getMass() + b.getMass());   // Коэффициент p учитывает скорость обоих мячей

        a.setVelocity(Velocity(aV - normal * p * a.getMass()));   // Установка новой скорости первого шара после столкновения 
        b.setVelocity(Velocity(bV + normal * p * b.getMass()));   // Установка новой скорости второго шара после столкновения 
    }
}

// Метод для перемещения частиц пыли на основе их скорости и времени на тик 
void Physics::moveDusts(std::vector<Dust>& dusts) const {
    for (Dust& dust : dusts) {   // Цикл по всем частицам пыли в векторе dusts 
        if (dust.ifAlive()) {     // Проверка: жива ли частица пыли 
            Point newPos = dust.getCenter() + dust.getVelocity().vector() * timePerTick;   // Вычисление новой позиции частицы на основе ее скорости и времени на тик 
            dust.setCenter(newPos);   // Установка новой позиции частицы пыли 
        }
    }
}