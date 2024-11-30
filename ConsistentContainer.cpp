#include "ConsistentContainer.h" // Подключаем заголовочный файл ConsistentContainer.h

// Конструктор по умолчанию
ConsistentContainer::ConsistentContainer() : data(nullptr), size(0), maxsize(0) {}
// Инициализирует объект пустым массивом: указатель data равен nullptr, size и maxsize равны 0

// Перемещающий конструктор
ConsistentContainer::ConsistentContainer(ConsistentContainer&& rvalue) noexcept
    : data(std::move(rvalue.data)), size(rvalue.size), maxsize(rvalue.maxsize) {
    rvalue.data = nullptr;    // Обнуляем указатель у перемещенного объекта
    rvalue.size = 0;          // Обнуляем размер
    rvalue.maxsize = 0;       // Обнуляем емкость
}

// Перемещающий оператор присваивания
ConsistentContainer& ConsistentContainer::operator=(ConsistentContainer&& rvalue) noexcept {
    if (this != &rvalue) {    // Проверяем, что объект не присваивается сам себе
        delete[] data;        // Освобождаем текущую память
        data = rvalue.data;   // Переносим указатель на массив
        size = rvalue.size;   // Переносим размер
        maxsize = rvalue.maxsize; // Переносим емкость

        rvalue.data = nullptr; // Обнуляем перемещенный объект
        rvalue.size = 0;
        rvalue.maxsize = 0;
    }
    return *this; // Возвращаем текущий объект
}

// Деструктор
ConsistentContainer::~ConsistentContainer() {
    delete[] data; // Освобождаем память, выделенную для массива
}

// Увеличение емкости массива
void ConsistentContainer::moresize() {
    maxsize = maxsize == 0 ? 1 : static_cast<int>(maxsize * 1.5); // Увеличиваем емкость на 50%
    int* newData = new int[maxsize]; // Выделяем новый массив
    for (int i = 0; i < size; ++i) { // Копируем данные из старого массива в новый
        newData[i] = data[i];
    }
    delete[] data; // Освобождаем старую память
    data = newData; // Перенаправляем указатель на новый массив
}

// Уменьшение емкости массива до фактического размера
void ConsistentContainer::shrinkToFit() {
    int* newData = new int[size]; // Создаем массив размера size
    for (int i = 0; i < size; ++i) { // Копируем существующие элементы
        newData[i] = data[i];
    }
    delete[] data; // Удаляем старую память
    data = newData; // Обновляем указатель
    maxsize = size; // Обновляем емкость
}

// Добавление элемента в конец массива
void ConsistentContainer::push_back(int value) {
    if (size == maxsize) {  // Если массив заполнен, увеличиваем его емкость
        moresize();
    }
    data[size++] = value;   // Добавляем элемент и увеличиваем размер
}

// Добавление элемента в начало массива
void ConsistentContainer::push_front(int value) {
    if (size == maxsize) {  // Если массив заполнен, увеличиваем его емкость
        moresize();
    }
    for (int i = size; i > 0; --i) { // Сдвигаем все элементы вправо
        data[i] = data[i - 1];
    }
    data[0] = value; // Вставляем элемент в начало
    ++size;          // Увеличиваем размер
}

// Вставка элемента по индексу
void ConsistentContainer::insert(int index, int value) {
    if (index < 0 || index > size) { // Проверяем допустимость индекса
        throw std::out_of_range("Индекс вне диапазона");
    }
    if (size == maxsize) {  // Если массив заполнен, увеличиваем емкость
        moresize();
    }
    for (int i = size; i > index; --i) { // Сдвигаем элементы вправо
        data[i] = data[i - 1];
    }
    data[index] = value; // Вставляем элемент
    ++size;              // Увеличиваем размер
}

// Удаление элемента по индексу
void ConsistentContainer::erase(int index) {
    if (index < 0 || index >= size) { // Проверяем допустимость индекса
        throw std::out_of_range("Индекс вне диапазона");
    }
    for (int i = index; i < size - 1; ++i) { // Сдвигаем элементы влево
        data[i] = data[i + 1];
    }
    --size; // Уменьшаем размер
    if (size < maxsize / 2 && maxsize > 1) { // Уменьшаем емкость, если массив слишком "разрежен"
        shrinkToFit();
    }
}

// Получение текущего размера массива
int ConsistentContainer::getSize() const {
    return size;
}

// Получение текущей емкости массива
int ConsistentContainer::getmaxsize() const {
    return maxsize;
}

// Вывод содержимого массива
void ConsistentContainer::print() const {
    for (int i = 0; i < size; ++i) { // Перебираем элементы
        std::cout << data[i] << " "; // Выводим каждый элемент
    }
    std::cout << std::endl;
}

// Перегрузка оператора [] для доступа по индексу
int& ConsistentContainer::operator[](int index) {
    if (index < 0 || index >= size) { // Проверяем допустимость индекса
        throw std::out_of_range("Индекс вне диапазона");
    }
    return data[index]; // Возвращаем ссылку на элемент
}

// Конструктор итератора
ConsistentContainer::Iterator::Iterator(int* ptr) : ptr(ptr) {}
// Указатель на текущий элемент передается в итератор при создании

// Оператор разыменования
int& ConsistentContainer::Iterator::operator*() {
    if (!ptr) {                      // Проверяем, что указатель не равен nullptr
        throw std::out_of_range("Индекс вне диапазона");
    }
    return *ptr;                     // Возвращаем значение, на которое указывает ptr
}

// Получение текущего значения через итератор
int& ConsistentContainer::Iterator::get() {
    if (!ptr) {                      // Проверяем, что указатель не равен nullptr
        throw std::out_of_range("Индекс вне диапазона");
    }
    return *ptr;                     // Возвращаем значение, на которое указывает ptr
}

// Оператор сравнения
bool ConsistentContainer::Iterator::operator!=(const Iterator& rvalue) {
    return ptr != rvalue.ptr;        // Возвращаем true, если указатели не совпадают
}

// Переход к следующему элементу
ConsistentContainer::Iterator& ConsistentContainer::Iterator::operator++() {
    ++ptr;                           // Смещаем указатель на следующий элемент
    return *this;                    // Возвращаем текущий итератор
}

// Возвращает итератор на начало массива
ConsistentContainer::Iterator ConsistentContainer::begin() {
    return Iterator(data);           // Возвращаем итератор, указывающий на начало массива
}

// Возвращает итератор на конец массива
ConsistentContainer::Iterator ConsistentContainer::end() {
    return Iterator(data + size);    // Возвращаем итератор, указывающий за конец массива
}