#include "World.h"      
#include "Painter.h"    
#include <fstream>      
#include "algorithm"    

// Длительность одного тика симуляции.
// Подробнее см. update()
// Изменять не следует
static constexpr double timePerTick = 0.001;  // Константа, определяющая длительность одного тика симуляции в секундах

/// @brief читает координаты точки
/// @param stream входной поток для чтения данных
/// @return возвращает объект Point с координатами (x, y)
Point readPoint(std::istream& stream)
{
    double x, y;  // Переменные для хранения координат
    stream >> x >> y;  // Чтение координат x и y из потока 
    return Point(x, y);  // Возвращает созданный объект Point с заданными координатами   
}

/// @brief читает цвет
/// @param stream входной поток для чтения данных
/// @return возвращает объект Color с компонентами (r, g, b)
Color readColor(std::istream& stream)
{
    double r, g, b;  // Переменные для хранения компонентов цвета
    stream >> r >> g >> b;  // Чтение значений r, g и b из потока
    return Color(r, g, b);  // Возвращает созданный объект Color с заданными компонентами
}

/**
 * Конструирует объект мира для симуляции
 * @param worldFilePath путь к файлу модели мира
 */
World::World(const std::string& worldFilePath) {
    std::ifstream stream(worldFilePath);  // Открытие файла по указанному пути для чтения
    topLeft = readPoint(stream);           // Чтение верхнего левого угла области мира
    bottomRight = readPoint(stream);       // Чтение нижнего правого угла области мира
    physics.setWorldBox(topLeft, bottomRight);  // Установка границ мира в физическом движке

    double radius;                         // Переменная для хранения радиуса шара
    bool isCollidable;                     // Переменная для хранения информации о столкновении шара

    // Здесь не хватает обработки ошибок, но на текущем
    // уровне прохождения курса нас это устраивает
    while (stream.peek(), stream.good()) {  // Цикл до тех пор, пока в потоке есть данные и он хорош для чтения
        Point center = readPoint(stream);   // Чтение центра шара из потока
        Point vectorV = readPoint(stream);   // Чтение вектора скорости шара из потока
        Velocity velocity(vectorV);          // Создание объекта Velocity на основе прочитанного вектора
        
        // Читаем три составляющие цвета шара
        Color color = readColor(stream);     // Чтение цвета шара из потока
        
        // Читаем радиус шара
        stream >> radius;                    // Чтение радиуса шара из потока
        stream >> std::boolalpha >> isCollidable;  // Чтение значения столкновения (true/false) из потока
        
        Ball ball(velocity, center, radius, color, isCollidable);  // Создание объекта Ball с прочитанными параметрами
        balls.push_back(ball);               // Добавление созданного шара в вектор balls
    }
}

/// @brief Отображает состояние мира
void World::show(Painter& painter) const {
    // Рисуем белый прямоугольник, отображающий границу мира
    painter.draw(topLeft, bottomRight, Color(1, 1, 1));  // Вызов метода draw для рисования границ мира

    // Вызываем отрисовку каждого шара
    for (const Ball& ball : balls) {   // Цикл по всем шарам в векторе balls
        ball.draw(painter);             // Вызов метода draw у каждого шара для его отрисовки
    }
    
    for (const Dust& dust : dusts) {   // Цикл по всем частицам пыли в векторе dusts
        if(dust.ifAlive()){             // Проверка, жива ли частица пыли
            dust.draw(painter);          // Если жива, вызываем метод draw для ее отрисовки 
        }
    }
}

/// @brief Обновляет состояние мира
void World::update(double time) {
    // учитываем остаток времени, который мы не "доработали" при прошлом update
    time += restTime;                  // Добавление остатка времени к новому времени обновления
    
    const auto ticks = static_cast<size_t>(std::floor(time / timePerTick));  // Вычисление количества полных тиков на основе прошедшего времени
    
    restTime = time - double(ticks) * timePerTick;  // Вычисление остатка времени после учета полных тиков

    physics.update(balls, dusts, ticks);  // Обновление физики с учетом состояния шаров и частиц пыли

    // Обновление времени жизни частиц пыли 
    for (auto& dust : dusts) {              // Цикл по всем частицам пыли 
        dust.dying(timePerTick);            // Передача времени для обновления состояния частиц пыли 
    }

    // Удаление мертвых частиц (ДО обновления физики)
    dusts.erase(std::remove_if(dusts.begin(), dusts.end(), [](const Dust& dust) { 
        return !dust.ifAlive(); }),         // Условие удаления: частица мертва 
        dusts.end());                       // Удаляем мертвые частицы из вектора dusts 
}