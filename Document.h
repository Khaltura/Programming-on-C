#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <fstream>
#include "Primitive.h"
#include <algorithm>

class Document {
public:
    Document(const std::string& name) : name(name) {}

    void addPrimitive(std::shared_ptr<Primitive> primitive) {
        primitives.push_back(primitive);
        std::cout << "В документ " << name << " добавлен примитив " << primitive->getType() << ": " << primitive->getName() << std::endl;
    }

    void removePrimitive(const std::string& primitiveName, const std::string& primitiveType) {
        auto it = std::find_if(primitives.begin(), primitives.end(), [&](const std::shared_ptr<Primitive>& p) {
            return p->getName() == primitiveName && p->getType() == primitiveType;
        });
        if (it != primitives.end()) {
            std::cout << "Из документа " << name << " удален примитив " << (*it)->getName() << " типа " << (*it)->getType() << std::endl;
            primitives.erase(it);
        } else {
            std::cout << "Примитив с именем '" << primitiveName << "' типа '" << primitiveType << "' не найден." << std::endl;
        }
    }

    void saveToFile(const std::string& filename) {
        std::ofstream outFile(filename);
        if (outFile.is_open()) {
            outFile << name << "\n";
            for (const auto& primitive : primitives) {
                outFile << primitive->serialize() << "\n";
            }
            outFile.close();
            std::cout << "Документ " << name << " сохранен в файл " << filename << std::endl;
        } else {
            std::cout << "Не удалось открыть файл для записи." << std::endl;
        }
    }

    void loadFromFile(const std::string& filename) {
        std::ifstream inFile(filename);
        if (inFile.is_open()) {
            primitives.clear(); // Очистка существующих примитивов
            std::string line;
            while (std::getline(inFile, line)) {
                auto primitive = Primitive::deserialize(line);
                if (primitive) {
                    addPrimitive(primitive);
                }
            }
            inFile.close();
            std::cout << "Документ \"" << name << "\" загружен из файла \"" << filename << "\"." << std::endl;
            // Вывод информации о всех загруженных примитивах
            std::cout << "Содержимое документа:\n";
            for (const auto& prim : primitives) {
                std::cout << "- Примитив: " << prim->getType() 
                          << ", Имя: " << prim->getName() 
                          << "\n";
            }
        } else {
            std::cout << "Не удалось открыть файл для чтения." << std::endl;
        }
    }

    std::string getName() const {
        return name;
    }

private:
    std::string name;
    std::vector<std::shared_ptr<Primitive>> primitives;
};

#endif