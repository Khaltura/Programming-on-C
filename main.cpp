#include <iostream>
#include <map>
#include <memory>
#include <vector>
#include <unordered_set>

// Реализация пользовательского аллокатора
template <typename T>
class CustomAllocator {
public:
    using value_type = T;

    size_t block_size;

    // Конструктор с параметром block_size
    explicit CustomAllocator(size_t block_size) : block_size(block_size), allocated(0) { //explicit предотвращает неявное преобразование типов при инициализации
        expand(); // Расширяем буфер при создании аллокатора
    }

    // Конструктор по умолчанию (необходим для совместимости с STL)
    CustomAllocator() : CustomAllocator(10) {}

    // Конструктор копирования
    template <typename U>
    CustomAllocator(const CustomAllocator<U>& other)
        : block_size(other.block_size), allocated(0) {
        expand(); // Резервируем память
    }

    // Метод для выделения памяти
    T* allocate(std::size_t n) {
        if (n == 0) return nullptr; // Защита от нулевого запроса
        if (allocated + n * sizeof(T) > buffer.size()) {  //Проверка на достаточность памяти
            expand(); 
        }
        T* result = reinterpret_cast<T*>(buffer.data() + allocated); //Используется чтобы привести указатель типа char* к типу указателя на нужный объект
        allocated += n * sizeof(T);
        
        // Отслеживаем выделенные объекты
        allocated_objects.insert(result);
        
        return result;
    }

    // Метод для освобождения памяти
    void deallocate(T* p, std::size_t n) {
        if (allocated_objects.find(p) != allocated_objects.end()) {
            // Если объект был выделен, удаляем его из отслеживаемых объектов
            allocated_objects.erase(p);
            p->~T(); // Вызываем деструктор объекта
        
        }
    }

    // Метод rebind для поддержки различных типов
    template<typename U>
    struct rebind {
        using other = CustomAllocator<U>;
    };

    ~CustomAllocator() {
        for (auto p : allocated_objects) {
            p->~T(); // Вызываем деструктор для всех оставшихся объектов
        }
        buffer.clear();
    }

private:
    void expand() {
        buffer.resize(buffer.size() + block_size * sizeof(T));
    }

    size_t allocated;  // Общее количество выделенной памяти
    std::vector<char> buffer; // Буфер для хранения выделенной памяти
    std::unordered_set<T*> allocated_objects; // Для отслеживания выделенных объектов
};

// Реализация пользовательского контейнера
template <typename T, typename Alloc = std::allocator<T>>
class CustomContainer {
public:
    using allocator_type = Alloc;
    
    using iterator = typename std::vector<T*>::iterator;
    using const_iterator = typename std::vector<T*>::const_iterator;

    CustomContainer(Alloc alloc = Alloc()) : alloc(alloc) {}

    void add(const T& value) {
        T* p = alloc.allocate(1);
        new(p) T(value); // Конструируем объект на выделенной памяти
        data.push_back(p);
    }

    void display() const {
        for (auto p : data) {
            std::cout << *p << " ";
        }
        std::cout << std::endl;
    }

    // Итераторы
    iterator begin() { return data.begin(); }
    iterator end() { return data.end(); }
    
    const_iterator begin() const { return data.begin(); }
    const_iterator end() const { return data.end(); }

    // Вспомогательные методы
    size_t size() const { return data.size(); }
    
    bool empty() const { return data.empty(); }

    ~CustomContainer() {
        for (auto p : data) {
            alloc.deallocate(p, 1); // Освобождаем память
        }
    }

private:
    Alloc alloc;
    std::vector<T*> data; // Хранит указатели на выделенные элементы
};

// Прикладной код
int main() {
   // Создание экземпляра std::map<int, int> и заполнение его
   std::map<int, int> factorial_map;
   for (int i = 0; i < 10; ++i) {
       int factorial = 1;
       for (int j = 1; j <= i; ++j) {
           factorial *= j;
       }
       factorial_map[i] = factorial;
   }

   // Вывод значений из std::map
   std::cout << "std::map with default allocator:" << std::endl;
   for (const auto& pair : factorial_map) {
       std::cout << pair.first << " " << pair.second << std::endl;
   }

   // Создание экземпляра std::map с новым аллокатором
   CustomAllocator<std::pair<const int, int>> custom_alloc(10);
   
   std::map<int, int, std::less<int>, CustomAllocator<std::pair<const int, int>>> custom_map(custom_alloc);
   
   // Заполнение кастомной карты
   for (int i = 0; i < 10; ++i) {
       int factorial = 1;
       for (int j = 1; j <= i; ++j) {
           factorial *= j;
       }
       custom_map[i] = factorial;
   }

   // Вывод значений из кастомной карты
   std::cout << "\nstd::map with custom allocator:" << std::endl;
   
   for (const auto& pair : custom_map) {
       std::cout << pair.first << " " << pair.second << std::endl;
   }

   // Создание экземпляра своего контейнера для хранения int
   CustomContainer<int, CustomAllocator<int>> my_container;

   // Заполнение контейнера элементами от 0 до 9
   for (int i = 0; i < 10; ++i) {
       my_container.add(i);
   }

   // Вывод значений из своего контейнера
   std::cout << "\nCustom container values:" << std::endl;

   my_container.display();

   // Использование методов size и empty
   std::cout << "Size of container: " << my_container.size() << std::endl;
   std::cout << "Is container empty? " << (my_container.empty() ? "Yes" : "No") << std::endl;

   // Использование итераторов для обхода контейнера
   for (auto it = my_container.begin(); it != my_container.end(); ++it) {
       std::cout << **it << " ";
   }
   std::cout << std::endl;

   return 0;
}