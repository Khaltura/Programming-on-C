#include "ConsistentContainer.h"  // Подключение класса ConsistentContainer
#include "DoubleLinkedList.h"     // Подключение класса DoubleLinkedList
#include "SinglyLinkedList.h"     // Подключение класса SinglyLinkedList
#include <iostream>               // Для работы с вводом/выводом

int main() {
    // Создание объектов контейнеров
    ConsistentContainer vec;
    DoubleLinkedList dblList;
    SinglyLinkedList snglList;

    // Тестирование контейнера ConsistentContainer
    std::cout << "ConsistentContainer:" << std::endl;

    // Добавление элементов
    for (int i = 0; i < 10; ++i) {
        vec.push_back(i);
    }
    std::cout << "Контейнер: ";
    vec.print();
    std::cout << "Размер: " << vec.getSize() << std::endl;

    // Удаление элементов
    vec.erase(2); // Удаление третьего элемента
    vec.erase(3); // Удаление пятого элемента
    vec.erase(4); // Удаление седьмого элемента
    std::cout << "Удаление 3, 5, 7 элементов: ";
    vec.print();

    // Добавление элемента в начало
    vec.push_front(10);
    std::cout << "Добавление элемента в начало: ";
    vec.print();

    // Вставка в середину
    vec.insert(vec.getSize() / 2, 20);
    std::cout << "Добавление элемента в середину: ";
    vec.print();

    // Добавление элемента в конец
    vec.push_back(30);
    std::cout << "Добавление элемента в конец: ";
    vec.print();
    std::cout << std::endl;

    // Тестирование контейнера DoubleLinkedList
    std::cout << "DoubleLinkedList:" << std::endl;
    for (int i = 0; i < 10; ++i) {
        dblList.push_back(i);
    }
    std::cout << "Контейнер: ";
    dblList.print();
    std::cout << "Размер: " << dblList.getSize() << std::endl;

    dblList.erase(2);
    dblList.erase(3);
    dblList.erase(4);
    std::cout << "Удаление 3, 5, 7 элементов: ";
    dblList.print();

    dblList.push_front(10);
    std::cout << "Добавление элемента в начало: ";
    dblList.print();

    dblList.insert(dblList.getSize() / 2, 20);
    std::cout << "Добавление элемента в середину: ";
    dblList.print();

    dblList.push_back(30);
    std::cout << "Добавление элемента в конец: ";
    dblList.print();
    std::cout << std::endl;

    // Тестирование контейнера SinglyLinkedList
    std::cout << "SinglyLinkedList:" << std::endl;
    for (int i = 0; i < 10; ++i) {
        snglList.push_back(i);
    }
    std::cout << "Контейнер: ";
    snglList.print();
    std::cout << "Размер: " << snglList.getSize() << std::endl;

    snglList.erase(2);
    snglList.erase(3);
    snglList.erase(4);
    std::cout << "Удаление 3, 5, 7 элементов: ";
    snglList.print();

    snglList.push_front(10);
    std::cout << "Добавление элемента в начало: ";
    snglList.print();

    snglList.insert(snglList.getSize() / 2, 20);
    std::cout << "Добавление элемента в середину: ";
    snglList.print();

    snglList.push_back(30);
    std::cout << "Добавление элемента в конец: ";
    snglList.print();
    std::cout << std::endl;

    // Демонстрация семантики перемещения для ConsistentContainer
    {
        std::cout << "Демонстрация семантики перемещения для ConsistentContainer:\n";
        ConsistentContainer tempContainer;
        tempContainer.push_back(1);
        tempContainer.push_back(2);
        tempContainer.push_back(3);
        tempContainer.push_back(4);
        std::cout << "tempContainer не пуст: ";
        tempContainer.print();

        ConsistentContainer movedContainer = std::move(tempContainer);
        std::cout << "Содержимое movedContainer после перемещения: ";
        movedContainer.print();

        std::cout << "Содержимое tempContainer после перемещения: ";
        tempContainer.print();

        if (tempContainer.getSize() == 0) {
            std::cout << "tempContainer успешно перемещен.\n";
        } else {
            std::cout << "Ошибка: tempContainer существует!\n";
        }
        std::cout << std::endl;
    }

    // Аналогичная демонстрация семантики перемещения для DoubleLinkedList и SinglyLinkedList:
    {
        std::cout << "Демонстрация семантики перемещения для DoubleLinkedList:\n";
        DoubleLinkedList tempList;
        tempList.push_back(1);
        tempList.push_back(2);
        tempList.push_back(3);
        tempList.push_back(4);
        std::cout << "tempList не пуст: ";
        tempList.print();

        DoubleLinkedList movedList = std::move(tempList);
        std::cout << "Содержимое movedList после перемещения: ";
        movedList.print();

        std::cout << "Содержимое tempList после перемещения: ";
        tempList.print();

        if (tempList.getSize() == 0) {
            std::cout << "tempList успешно перемещен.\n";
        } else {
            std::cout << "Ошибка: tempList существует!\n";
        }
        std::cout << std::endl;
    }

    {
        std::cout << "Демонстрация семантики перемещения для SinglyLinkedList:\n";
        SinglyLinkedList tempList;
        tempList.push_back(1);
        tempList.push_back(2);
        tempList.push_back(3);
        tempList.push_back(4);
        std::cout << "tempList не пуст: ";
        tempList.print();

        SinglyLinkedList movedList = std::move(tempList);
        std::cout << "Содержимое movedList после перемещения: ";
        movedList.print();

        std::cout << "Содержимое tempList после перемещения: ";
        tempList.print();

        if (tempList.getSize() == 0) {
            std::cout << "tempList успешно перемещен.\n";
        } else {
            std::cout << "Ошибка: tempList существует!\n";
        }
        std::cout << std::endl;
    }

    return 0; // Завершаем выполнение программы
}