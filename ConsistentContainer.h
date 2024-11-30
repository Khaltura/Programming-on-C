#ifndef CONSISTENT_CONTAINER_H    // Проверяем, что макрос CONSISTENT_CONTAINER_H не определен
#define CONSISTENT_CONTAINER_H    // Определяем макрос, чтобы предотвратить повторное включение файла

#include <iostream>               // Для работы с вводом/выводом в консоль
#include <stdexcept>              // Для обработки исключений (например, out_of_range)
#include <utility>                // Для работы с функцией std::move

// Класс, реализующий динамический массив
class ConsistentContainer {
private:
    int* data;    // Указатель на массив элементов
    int size;     // Текущее количество элементов в массиве
    int maxsize;  // Максимальная емкость массива (выделенная память)

    // Приватные методы для изменения размера массива
    void moresize();     // Увеличивает емкость массива
    void shrinkToFit();  // Уменьшает емкость массива до текущего размера

public:
    // Конструкторы и деструктор
    ConsistentContainer();                                // Конструктор по умолчанию
    ConsistentContainer(ConsistentContainer&& rvalue) noexcept; // Перемещающий конструктор
    ConsistentContainer& operator=(ConsistentContainer&& rvalue) noexcept; // Перемещающий оператор присваивания
    ~ConsistentContainer();                               // Деструктор для освобождения памяти

    // Методы для работы с элементами массива
    void push_back(int value);                            // Добавляет элемент в конец массива
    void push_front(int value);                           // Добавляет элемент в начало массива
    void insert(int index, int value);                    // Вставляет элемент по указанному индексу
    void erase(int index);                                // Удаляет элемент по указанному индексу

    // Методы для получения информации о массиве
    int getSize() const;                                  // Возвращает текущее количество элементов
    int getmaxsize() const;                               // Возвращает максимальную емкость массива
    void print() const;                                   // Выводит содержимое массива в консоль

    // Перегрузка оператора [] для доступа к элементам массива
    int& operator[](int index);                          // Доступ к элементу по индексу с проверкой

    // Итератор для перебора элементов массива
    struct Iterator {
        int* ptr;                                         // Указатель на текущий элемент массива

        Iterator(int* ptr);                               // Конструктор итератора
        int& operator*();                                 // Оператор разыменования (доступ к значению)
        int& get();                                       // Получение текущего значения
        bool operator!=(const Iterator& rvalue);          // Оператор сравнения (для проверки конца итерации)
        Iterator& operator++();                           // Переход к следующему элементу
    };

    // Методы для получения итераторов
    Iterator begin();                                     // Возвращает итератор на начало массива
    Iterator end();                                       // Возвращает итератор на позицию за концом массива
};

#endif // Завершение защиты от повторного включения