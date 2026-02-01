
#include <iostream>
#include <cassert>

#include "../include/util.h"


int main() {
    // Si suma(2,2) no es 4, el programa fallará
    assert(sumar(2, 2) == 4); 
    std::cout << "TEST PASADO: 2 + 2 es 4 ✅" << std::endl;
    return 0;
}