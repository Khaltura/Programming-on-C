#ifndef DOUBLE_LINKED_LIST_H // Проверяем, что макрос DOUBLE_LINKED_LIST_H не определен
#define DOUBLE_LINKED_LIST_H // Определяем макрос, чтобы предотвратить повторное включение

#include <iostream>  // Для ввода и вывода
#include <stdexcept> // Для обработки исключений

// Класс, реализующий двусвязный список
class DoubleLinkedList {
private:
    // Узел списка
    struct Node {
        int value;       // Значение узла
        Node* next;      // Указатель на следующий узел
        Node* prev;      // Указатель на предыдущий узел

        Node(int value); // Конструктор узла
    };

    Node* head; // Указатель на первый элемент списка
    Node* tail; // Указатель на последний элемент списка
    int size;   // Текущее количество элементов в списке

public:
    DoubleLinkedList();                            // Конструктор по умолчанию
    ~DoubleLinkedList();                           // Деструктор
    DoubleLinkedList(DoubleLinkedList&& rvalue) noexcept; // Перемещающий конструктор
    DoubleLinkedList& operator=(DoubleLinkedList&& rvalue) noexcept; // Перемещающий оператор присваивания

    void push_back(int value);                    // Добавление элемента в конец
    void push_front(int value);                   // Добавление элемента в начало
    void insert(int index, int value);            // Вставка элемента по индексу
    void erase(int index);                        // Удаление элемента по индексу
    int getSize() const;                          // Получение текущего размера списка
    void print() const;                           // Вывод содержимого списка
    int& operator[](int index);                   // Доступ к элементу по индексу

    // Итератор для списка
    struct Iterator {
        Node* ptr;                                // Указатель на текущий узел

        Iterator(Node* ptr);                      // Конструктор итератора
        int operator*();                          // Оператор разыменования
        bool operator!=(const Iterator& rvalue);  // Оператор сравнения
        Iterator& operator++();                   // Переход к следующему узлу
        int& get();                               // Получение текущего значения
    };

    Iterator begin();                             // Итератор на начало списка
    Iterator end();                               // Итератор на конец списка
};

#endif // Завершаем защиту от повторного включения