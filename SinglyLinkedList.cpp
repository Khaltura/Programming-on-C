#include "SinglyLinkedList.h" // Подключаем заголовочный файл SinglyLinkedList.h

// Конструктор узла
SinglyLinkedList::Node::Node(int value) : value(value), next(nullptr) {}

// Конструктор по умолчанию
SinglyLinkedList::SinglyLinkedList() : head(nullptr), size(0) {}

// Деструктор
SinglyLinkedList::~SinglyLinkedList() {
    while (head) {              // Пока есть элементы в списке
        Node* next = head->next; // Сохраняем указатель на следующий узел
        delete head;            // Удаляем текущий узел
        head = next;            // Переходим к следующему
    }
}

// Перемещающий конструктор
SinglyLinkedList::SinglyLinkedList(SinglyLinkedList&& rvalue) noexcept
    : head(rvalue.head), size(rvalue.size) {
    rvalue.head = nullptr;      // Обнуляем перемещенный объект
    rvalue.size = 0;
}

// Перемещающий оператор присваивания
SinglyLinkedList& SinglyLinkedList::operator=(SinglyLinkedList&& rvalue) noexcept {
    if (this != &rvalue) {      // Проверяем, что объект не присваивается сам себе
        while (head) {          // Очищаем текущий список
            Node* next = head->next;
            delete head;
            head = next;
        }
        head = rvalue.head;     // Переносим данные
        size = rvalue.size;
        rvalue.head = nullptr;  // Обнуляем перемещенный объект
        rvalue.size = 0;
    }
    return *this;               // Возвращаем текущий объект
}

// Добавление элемента в конец списка
void SinglyLinkedList::push_back(int value) {
    Node* newNode = new Node(value); // Создаем новый узел
    if (!head) {                     // Если список пуст
        head = newNode;              // Новый узел становится началом
    } else {
        Node* current = head;        // Идем до конца списка
        while (current->next) {
            current = current->next;
        }
        current->next = newNode;     // Добавляем узел в конец
    }
    ++size;                          // Увеличиваем размер списка
}

// Добавление элемента в начало списка
void SinglyLinkedList::push_front(int value) {
    Node* newNode = new Node(value); // Создаем новый узел
    newNode->next = head;            // Устанавливаем указатель на текущий head
    head = newNode;                  // Новый узел становится началом
    ++size;                          // Увеличиваем размер списка
}

// Вставка элемента по индексу
void SinglyLinkedList::insert(int index, int value) {
    if (index < 0 || index > size) { // Проверяем допустимость индекса
        throw std::out_of_range("Индекс вне допустимого диапазона");
    }
    if (index == 0) {                // Если индекс 0, вставляем в начало
        push_front(value);
        return;
    }
    if (index == size) {             // Если индекс равен size, вставляем в конец
        push_back(value);
        return;
    }
    Node* newNode = new Node(value); // Создаем новый узел
    Node* current = head;
    for (int i = 0; i < index - 1; ++i) { // Находим узел перед нужным индексом
        current = current->next;
    }
    newNode->next = current->next;   // Вставляем узел в нужное место
    current->next = newNode;
    ++size;                          // Увеличиваем размер списка
}

// Удаление элемента по индексу
void SinglyLinkedList::erase(int index) {
    if (index < 0 || index >= size) { // Проверяем допустимость индекса
        throw std::out_of_range("Индекс вне диапазона");
    }
    Node* current = head;
    if (index == 0) {                // Если индекс 0, удаляем первый узел
        head = current->next;
        delete current;
    } else {
        Node* prev = nullptr;        // Узел перед удаляемым
        for (int i = 0; i < index; ++i) {
            prev = current;
            current = current->next;
        }
        prev->next = current->next;  // Убираем узел из цепочки
        delete current;
    }
    --size;                          // Уменьшаем размер списка
}

// Получение текущего размера списка
int SinglyLinkedList::getSize() const {
    return size;
}

// Вывод содержимого списка
void SinglyLinkedList::print() const {
    Node* current = head;            // Начинаем с первого узла
    while (current) {                // Перебираем все узлы
        std::cout << current->value << " "; // Выводим значение узла
        current = current->next;     // Переходим к следующему
    }
    std::cout << std::endl;
}

// Доступ к элементу по индексу
int& SinglyLinkedList::operator[](int index) {
    if (index < 0 || index >= size) { // Проверяем допустимость индекса
        throw std::out_of_range("Индекс вне диапазона");
    }
    Node* current = head;
    for (int i = 0; i < index; ++i) { // Переходим к нужному индексу
        current = current->next;
    }
    return current->value;           // Возвращаем ссылку на значение узла
}

// Конструктор итератора
SinglyLinkedList::Iterator::Iterator(Node* ptr) : ptr(ptr) {}

// Оператор разыменования
int SinglyLinkedList::Iterator::operator*() {
    if (!ptr) {                      // Проверяем, что указатель не пустой
        throw std::out_of_range("Индекс вне диапазона");
    }
    return ptr->value;               // Возвращаем значение узла
}

// Оператор сравнения
bool SinglyLinkedList::Iterator::operator!=(const Iterator& rvalue) {
    return ptr != rvalue.ptr;        // Сравниваем указатели
}

// Переход к следующему элементу
SinglyLinkedList::Iterator& SinglyLinkedList::Iterator::operator++() {
    ptr = ptr->next;                 // Переходим к следующему узлу
    return *this;
}

// Получение текущего значения через итератор
int& SinglyLinkedList::Iterator::get() {
    if (!ptr) {                      // Проверяем, что указатель не пустой
        throw std::out_of_range("Индекс вне диапазона");
    }
    return ptr->value;               // Возвращаем ссылку на значение узла
}

// Возвращает итератор на начало списка
SinglyLinkedList::Iterator SinglyLinkedList::begin() {
    return Iterator(head);           // Указатель на первый узел
}

// Возвращает итератор на конец списка
SinglyLinkedList::Iterator SinglyLinkedList::end() {
    return Iterator(nullptr);        // Конец списка представлен указателем nullptr
}