#include <iostream>
#include <memory>
#include <string>
#include "DocumentController.h"

void printMenu() {
    std::cout << "\nВыберите действие:\n";
    std::cout << "1. Создать новый документ\n";
    std::cout << "2. Импортировать документ из файла\n";
    std::cout << "3. Экспортировать документ в файл\n";
    std::cout << "4. Создать графический примитив\n";
    std::cout << "5. Удалить графический примитив\n";
    std::cout << "6. Выход\n";
}

int main() {
    DocumentController controller;

    while (true) {
        printMenu();
        
        int choice;
        if (!(std::cin >> choice)) break; // Проверка на корректный ввод
        
        switch (choice) {
            case 1: { // Создать новый документ
                controller.createDocument();
                break;
            }

            case 2: { // Импортировать документ из файла
                std::string filename;
                std::cout << "Введите имя файла для импорта: ";
                std::cin >> filename;
                controller.importDocument(filename);
                break; 
            }
            
            case 3: { // Экспортировать документ в файл
                std::string filename;
                std::cout << "Введите имя файла для экспорта: ";
                std::cin >> filename;
                controller.exportDocument(filename);
                break; 
            }

            case 4: { // Создать графический примитив
                if (controller.documentExists()) {
                    std::cout << "Введите тип примитива (rectangle, circle): ";
                    std::string type;
                    std::cin >> type;
                    controller.addPrimitive(type);
                } else {
                    std::cout << "Документ не создан.\n";
                }
                break; 
            }
           
            case 5: { // Удалить графический примитив
                if (controller.documentExists()) {
                    std::string name, type;
                    std::cout << "Введите тип примитива для удаления (rectangle, circle): ";
                    std::cin >> type;
                    std::cout << "Введите имя примитива для удаления: ";
                    std::cin >> name;
                    controller.removePrimitive(name, type);
                } else {
                    std::cout << "Документ не создан.\n";
                }
                break; 
            }

           case 6: // Выход
               return 0;

           default:
               std::cout << "Некорректный выбор.\n";
       }
   }
}