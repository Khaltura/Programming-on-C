#include <iostream>       // Подключение библиотеки для ввода-вывода
#include <map>            // Подключение библиотеки для использования std::map
#include <memory>         // Подключение библиотеки для работы с умными указателями и аллокаторами
#include <vector>         // Подключение библиотеки для работы с контейнером std::vector
#include <unordered_set>  // Подключение библиотеки для работы с контейнером std::unordered_set

// Реализация пользовательского аллокатора
template <typename T>
class CustomAllocator {
public:
    using value_type = T; // Определение типа значений, которые будет обрабатывать аллокатор

    size_t block_size; // Размер блока памяти, выделяемого за один раз

    // Конструктор с параметром block_size
    explicit CustomAllocator(size_t block_size) 
        : block_size(block_size), allocated(0) { // Инициализация размера блока и количества выделенной памяти
        expand(); // Расширяем буфер памяти при создании аллокатора
    }

    // Конструктор по умолчанию (необходим для совместимости с STL)
    CustomAllocator() : CustomAllocator(10) {} // По умолчанию размер блока равен 10

    // Конструктор копирования (необходим для совместимости с контейнерами)
    template <typename U>
    CustomAllocator(const CustomAllocator<U>& other)
        : block_size(other.block_size), allocated(0) {
        expand(); // Резервируем память для нового экземпляра аллокатора
    }

    // Метод для выделения памяти
    T* allocate(std::size_t n) {
        if (n == 0) return nullptr; // Проверяем, что запрос памяти ненулевой
        if (allocated + n * sizeof(T) > buffer.size()) { // Проверяем, хватит ли текущего буфера
            expand(); // Расширяем буфер, если памяти недостаточно
        }
        // Вычисляем адрес памяти для выделения
        T* result = reinterpret_cast<T*>(buffer.data() + allocated);
        allocated += n * sizeof(T); // Увеличиваем счётчик выделенной памяти
        
        // Добавляем указатель в множество для отслеживания выделенных объектов
        allocated_objects.insert(result);
        
        return result; // Возвращаем указатель на выделенную память
    }

    // Метод для освобождения памяти
    void deallocate(T* p, std::size_t n) {
        if (allocated_objects.find(p) != allocated_objects.end()) { // Проверяем, выделялась ли эта память
            allocated_objects.erase(p); // Убираем указатель из множества выделенных объектов
            p->~T(); // Вызываем деструктор для освобождаемого объекта
        }
    }

    // Метод rebind для поддержки аллокации памяти для других типов
    template<typename U>
    struct rebind {
        using other = CustomAllocator<U>;
    };

    // Деструктор, освобождающий все оставшиеся выделенные объекты
    ~CustomAllocator() {
        for (auto p : allocated_objects) { // Для каждого выделенного объекта
            p->~T(); // Вызываем его деструктор
        }
        buffer.clear(); // Очищаем буфер памяти
    }

private:
    // Расширение буфера памяти
    void expand() {
        buffer.resize(buffer.size() + block_size * sizeof(T)); // Увеличиваем размер буфера
    }

    size_t allocated;  // Количество памяти, выделенной из буфера
    std::vector<char> buffer; // Вектор для хранения сырой памяти
    std::unordered_set<T*> allocated_objects; // Множество для отслеживания выделенных объектов
};

// Реализация пользовательского контейнера
template <typename T, typename Alloc = std::allocator<T>>
class CustomContainer {
public:
    using allocator_type = Alloc; // Определяем тип аллокатора
    
    // Определяем типы итераторов
    using iterator = typename std::vector<T*>::iterator;
    using const_iterator = typename std::vector<T*>::const_iterator;

    // Конструктор контейнера, принимает аллокатор
    CustomContainer(Alloc alloc = Alloc()) : alloc(alloc) {}

    // Метод добавления нового элемента
    void add(const T& value) {
        T* p = alloc.allocate(1); // Выделяем память для одного элемента
        new(p) T(value); // Конструируем объект на выделенной памяти
        data.push_back(p); // Добавляем указатель на объект в контейнер
    }

    // Метод вывода всех элементов контейнера
    void display() const {
        for (auto p : data) { // Проходим по всем указателям
            std::cout << *p << " "; // Выводим значение, на которое указывает указатель
        }
        std::cout << std::endl;
    }

    // Методы для работы с итераторами
    iterator begin() { return data.begin(); }
    iterator end() { return data.end(); }
    
    const_iterator begin() const { return data.begin(); }
    const_iterator end() const { return data.end(); }

    // Метод для получения размера контейнера
    size_t size() const { return data.size(); }
    
    // Проверка, пуст ли контейнер
    bool empty() const { return data.empty(); }

    // Деструктор для освобождения всех элементов
    ~CustomContainer() {
        for (auto p : data) { // Для каждого указателя
            alloc.deallocate(p, 1); // Освобождаем память
        }
    }

private:
    Alloc alloc; // Аллокатор для управления памятью
    std::vector<T*> data; // Вектор для хранения указателей на элементы
};

// Прикладной код
int main() {
   // Создаём std::map с использованием стандартного аллокатора
   std::map<int, int> factorial_map;
   for (int i = 0; i < 10; ++i) { // Для чисел от 0 до 9
       int factorial = 1;
       for (int j = 1; j <= i; ++j) { // Вычисляем факториал
           factorial *= j;
       }
       factorial_map[i] = factorial; // Сохраняем в map
   }

   // Вывод значений из std::map
   std::cout << "std::map with default allocator:" << std::endl;
   for (const auto& pair : factorial_map) { // Для каждой пары ключ-значение
       std::cout << pair.first << " " << pair.second << std::endl; // Выводим ключ и значение
   }

   // Создаём std::map с использованием пользовательского аллокатора
   CustomAllocator<std::pair<const int, int>> custom_alloc(10);
   std::map<int, int, std::less<int>, CustomAllocator<std::pair<const int, int>>> custom_map(custom_alloc);
   
   for (int i = 0; i < 10; ++i) { // Для чисел от 0 до 9
       int factorial = 1;
       for (int j = 1; j <= i; ++j) { // Вычисляем факториал
           factorial *= j;
       }
       custom_map[i] = factorial; // Сохраняем в кастомный map
   }

   // Вывод значений из кастомного std::map
   std::cout << "\nstd::map with custom allocator:" << std::endl;
   for (const auto& pair : custom_map) {
       std::cout << pair.first << " " << pair.second << std::endl;
   }

   // Создаём пользовательский контейнер
   CustomContainer<int, CustomAllocator<int>> my_container;

   for (int i = 0; i < 10; ++i) { // Добавляем числа от 0 до 9
       my_container.add(i);
   }

   // Выводим значения контейнера
   std::cout << "\nCustom container values:" << std::endl;
   my_container.display();

   // Проверяем размер и пустоту контейнера
   std::cout << "Size of container: " << my_container.size() << std::endl;
   std::cout << "Is container empty? " << (my_container.empty() ? "Yes" : "No") << std::endl;

   // Используем итераторы для обхода
   for (auto it = my_container.begin(); it != my_container.end(); ++it) {
       std::cout << **it << " "; // Разыменовываем указатель
   }
   std::cout << std::endl;

   return 0; // Успешное завершение программы
}