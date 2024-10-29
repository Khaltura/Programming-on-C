#include "lib.h"

#include <iostream>

int main (int, char **) {
    std::cout << "Hello, world! Version: 1.0." << version() << std::endl;
    std::cout << "Second string" << std::endl;
    return 0;
}
