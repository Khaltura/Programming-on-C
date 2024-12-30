#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include <string>
#include <sstream>
#include <iostream>
#include <memory>

class Primitive {
public:
    Primitive(const std::string& name, const std::string& type) : name(name), type(type) {}

    virtual ~Primitive() = default;

    virtual void draw() = 0; // Виртуальный метод для отрисовки
    virtual std::string get() const = 0; // Чисто виртуальный метод
    virtual std::string serialize() const = 0; // Чисто виртуальный метод для сериализации

    static std::shared_ptr<Primitive> deserialize(const std::string& data);

    // Getter methods for name and type
    std::string getName() const { return name; }
    std::string getType() const { return type; }

protected:
    const std::string name;
    const std::string type;
};

class Rectangle : public Primitive {
public:
    Rectangle(const std::string& name, double width, double height)
      : Primitive(name, "rectangle"), width(width), height(height) {}

    void draw() override {}

    std::string get() const override { return ""; }

    std::string serialize() const override { 
        return type + ' ' + name + ' ' + std::to_string(width) + ' ' + std::to_string(height); 
    }

    static std::shared_ptr<Primitive> deserialize(const std::string& data) { 
        std::istringstream iss(data); 
        std::string name; 
        double width, height; 
        iss >> name >> width >> height; 
        return std::make_shared<Rectangle>(name, width, height); 
    }

private:
    double width;
    double height;
};

class Circle : public Primitive {
public:
   Circle(const std::string& name, double radius)
      : Primitive(name, "circle"), radius(radius) {}

   void draw() override {}

   std::string get() const override { return ""; }

   std::string serialize() const override { 
       return type + ' ' + name + ' ' + std::to_string(radius); 
   }

   static std::shared_ptr<Primitive> deserialize(const std::string& data) { 
       std::istringstream iss(data); 
       std::string name; 
       double radius; 
       iss >> name >> radius; 
       return std::make_shared<Circle>(name, radius); 
   }

private:
   double radius;
};

// Implementation of static method to deserialize based on type
std::shared_ptr<Primitive> Primitive::deserialize(const std::string& data) {
    if (data.find("rectangle") == 0) {
        return Rectangle::deserialize(data.substr(9)); // Удаляем тип из строки
    } else if (data.find("circle") == 0) {
        return Circle::deserialize(data.substr(6)); // Удаляем тип из строки
    }
    
    return nullptr; // Если тип не распознан
}

#endif // PRIMITIVE_H