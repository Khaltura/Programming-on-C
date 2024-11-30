# Минимальная версия CMake
cmake_minimum_required(VERSION 3.10)

# Название проекта
project(ContainerExample)

# Указываем стандарт C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Добавляем исходные файлы проекта
set(SOURCES
    main.cpp
    ConsistentContainer.cpp
    DoubleLinkedList.cpp
    SinglyLinkedList.cpp
)

# Указываем заголовочные файлы (опционально для IDE)
set(HEADERS
    ConsistentContainer.h
    DoubleLinkedList.h
    SinglyLinkedList.h
)

# Создаем исполняемый файл
add_executable(ContainerExample ${SOURCES} ${HEADERS})

if (${CMAKE_SYSTEM} MATCHES "Linux")
    set(CPACK_GENERATOR DEB)
elseif (${CMAKE_SYSTEM} MATCHES "Darwin")
    set(CPACK_GENERATOR ZIP)
endif()

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)
