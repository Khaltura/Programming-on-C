#include "DoubleLinkedList.h" // Подключаем заголовочный файл DoubleLinkedList.h

// Конструктор узла
DoubleLinkedList::Node::Node(int value) : value(value), next(nullptr), prev(nullptr) {}
// Инициализирует узел с указанным значением value. Указатели next и prev устанавливаются в nullptr

// Конструктор по умолчанию
DoubleLinkedList::DoubleLinkedList() : head(nullptr), tail(nullptr), size(0) {}
// Инициализирует пустой список: голова и хвост равны nullptr, размер списка size равен 0

// Деструктор
DoubleLinkedList::~DoubleLinkedList() {
    while (head) {                   // Пока в списке есть элементы
        Node* next = head->next;     // Сохраняем указатель на следующий узел
        delete head;                 // Удаляем текущий узел
        head = next;                 // Переходим к следующему узлу
    }
}

// Перемещающий конструктор
DoubleLinkedList::DoubleLinkedList(DoubleLinkedList&& rvalue) noexcept
    : head(rvalue.head), tail(rvalue.tail), size(rvalue.size) {
    rvalue.head = nullptr;           // Обнуляем указатели в перемещенном объекте
    rvalue.tail = nullptr;
    rvalue.size = 0;
}

// Перемещающий оператор присваивания
DoubleLinkedList& DoubleLinkedList::operator=(DoubleLinkedList&& rvalue) noexcept {
    if (this != &rvalue) {           // Проверяем, что объект не присваивается сам себе
        while (head) {               // Удаляем текущие элементы списка
            Node* next = head->next;
            delete head;
            head = next;
        }
        head = rvalue.head;          // Перемещаем данные из другого объекта
        tail = rvalue.tail;
        size = rvalue.size;

        rvalue.head = nullptr;       // Обнуляем перемещенный объект
        rvalue.tail = nullptr;
        rvalue.size = 0;
    }
    return *this;                    // Возвращаем текущий объект
}

// Добавление элемента в конец списка
void DoubleLinkedList::push_back(int value) {
    Node* newNode = new Node(value); // Создаем новый узел
    if (!head) {                     // Если список пуст
        head = newNode;              // Новый узел становится головой и хвостом
        tail = newNode;
    } else {                         // Если список не пуст
        tail->next = newNode;        // Присоединяем новый узел к концу
        newNode->prev = tail;        // Указываем, что предыдущий узел - это старый хвост
        tail = newNode;              // Обновляем хвост
    }
    ++size;                          // Увеличиваем размер списка
}

// Добавление элемента в начало списка
void DoubleLinkedList::push_front(int value) {
    Node* newNode = new Node(value); // Создаем новый узел
    if (!head) {                     // Если список пуст
        head = newNode;              // Новый узел становится головой и хвостом
        tail = newNode;
    } else {                         // Если список не пуст
        newNode->next = head;        // Присоединяем новый узел перед головой
        head->prev = newNode;        // Указываем, что следующий узел ссылается на новый узел
        head = newNode;              // Обновляем голову
    }
    ++size;                          // Увеличиваем размер списка
}

// Вставка элемента по индексу
void DoubleLinkedList::insert(int index, int value) {
    if (index < 0 || index > size) { // Проверяем, что индекс допустим
        throw std::out_of_range("Индекс вне диапазона");
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
    Node* current = head;            // Начинаем с головы
    for (int i = 0; i < index; ++i) { // Переходим к узлу с заданным индексом
        current = current->next;
    }
    newNode->next = current;         // Новый узел указывает на текущий
    newNode->prev = current->prev;   // Новый узел указывает на предыдущий
    current->prev->next = newNode;   // Предыдущий узел указывает на новый
    current->prev = newNode;         // Текущий узел указывает на новый как на предыдущий
    ++size;                          // Увеличиваем размер списка
}

// Удаление элемента по индексу
void DoubleLinkedList::erase(int index) {
    if (index < 0 || index >= size) { // Проверяем, что индекс допустим
        throw std::out_of_range("Индекс вне диапазона");
    }
    Node* current = head;             // Начинаем с головы
    for (int i = 0; i < index; ++i) { // Переходим к узлу с заданным индексом
        current = current->next;
    }
    if (current->prev) {              // Если не первый узел
        current->prev->next = current->next; // Связываем предыдущий с последующим
    } else {                          // Если это первый узел
        head = current->next;         // Обновляем голову
    }
    if (current->next) {              // Если не последний узел
        current->next->prev = current->prev; // Связываем последующий с предыдущим
    } else {                          // Если это последний узел
        tail = current->prev;         // Обновляем хвост
    }
    delete current;                   // Удаляем узел
    --size;                           // Уменьшаем размер списка
}

// Получение текущего размера списка
int DoubleLinkedList::getSize() const {
    return size;                      // Возвращаем значение переменной size
}

// Вывод содержимого списка
void DoubleLinkedList::print() const {
    Node* current = head;             // Начинаем с головы
    while (current) {                 // Пока есть узлы в списке
        std::cout << current->value << " "; // Выводим значение узла
        current = current->next;      // Переходим к следующему узлу
    }
    std::cout << std::endl;           // Переходим на новую строку после вывода
}

// Доступ к элементу по индексу
int& DoubleLinkedList::operator[](int index) {
    if (index < 0 || index >= size) { // Проверяем, что индекс допустим
        throw std::out_of_range("Индекс вне диапазона");
    }
    Node* current = head;             // Начинаем с головы
    for (int i = 0; i < index; ++i) { // Переходим к узлу с заданным индексом
        current = current->next;
    }
    return current->value;            // Возвращаем ссылку на значение узла
}

// Конструктор итератора
DoubleLinkedList::Iterator::Iterator(Node* ptr) : ptr(ptr) {}

// Оператор разыменования
int DoubleLinkedList::Iterator::operator*() {
    if (!ptr) {                       // Проверяем, что указатель не пустой
        throw std::out_of_range("Индекс вне диапазона");
    }
    return ptr->value;                // Возвращаем значение текущего узла
}

// Оператор сравнения
bool DoubleLinkedList::Iterator::operator!=(const Iterator& rvalue) {
    return ptr != rvalue.ptr;         // Возвращаем true, если указатели не совпадают
}

// Переход к следующему элементу
DoubleLinkedList::Iterator& DoubleLinkedList::Iterator::operator++() {
    ptr = ptr->next;                  // Перемещаем указатель на следующий узел
    return *this;
}

// Получение текущего значения через итератор
int& DoubleLinkedList::Iterator::get() {
    if (!ptr) {                       // Проверяем, что указатель не пустой
        throw std::out_of_range("Индекс вне диапазона");
    }
    return ptr->value;                // Возвращаем ссылку на значение текущего узла
}

// Возвращает итератор на начало списка
DoubleLinkedList::Iterator DoubleLinkedList::begin() {
    return Iterator(head);            // Итератор, указывающий на первый узел
}

// Возвращает итератор на конец списка
DoubleLinkedList::Iterator DoubleLinkedList::end() {
    return Iterator(nullptr);         // Итератор, указывающий на nullptr (конец списка)
}