#ifndef DOCUMENTCONTROLLER_H
#define DOCUMENTCONTROLLER_H

#include <memory>
#include <iostream>
#include "Document.h"

class DocumentController {
public:
    // Создание нового документа
    void createDocument() {
        std::cout << "Введите имя нового документа: ";
        std::string name;
        std::cin >> name;
        document = std::make_shared<Document>(name);
        std::cout << "Создан документ " << document->getName() << std::endl;
    }

    // Импорт документа из файла
    void importDocument(const std::string& filename) {
        if (document) {
            document->loadFromFile(filename);
        } else {
            std::cout << "Документ не создан." << std::endl;
        }
    }

    // Экспорт документа в файл
    void exportDocument(const std::string& filename) {
        if (document) {
            document->saveToFile(filename);
        } else {
            std::cout << "Документ не создан." << std::endl;
        }
    }

    // Добавление графического примитива в документ
    void addPrimitive(const std::string& type) {
        if (document) {
            std::cout << "Введите имя примитива: ";
            std::string name;
            std::cin >> name;

            if (type == "rectangle") {
                double width, height;
                std::cout << "Введите ширину: ";
                std::cin >> width;
                std::cout << "Введите высоту: ";
                std::cin >> height;
                document->addPrimitive(std::make_shared<Rectangle>(name, width, height));
            } else if (type == "circle") {
                double radius;
                std::cout << "Введите радиус: ";
                std::cin >> radius;
                document->addPrimitive(std::make_shared<Circle>(name, radius));
            } else {
                std::cout << "Неподдерживаемый тип примитива.\n";
            }
        } else {
            std::cout << "Документ не создан.\n";
        }
    }

    // Удаление графического примитива из документа
    void removePrimitive(const std::string& primitiveName, const std::string& primitiveType) {
        if (document) {
            document->removePrimitive(primitiveName, primitiveType);
        } else {
            std::cout << "Документ не создан." << std::endl;
        }
    }

    // Проверка на существование документа
    bool documentExists() const {
        return document != nullptr; // Проверка на существование документа
    }

private:
    std::shared_ptr<Document> document; // Указатель на текущий документ
};

#endif // DOCUMENTCONTROLLER_H