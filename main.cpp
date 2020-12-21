#include <iostream>
#include "tritset.h"

int main() {
    Tritset set{Trit::True, Trit::False, Trit::Unknown, Trit::True};
    std::cout << set << std::endl;
    testes();
    return 0;
}
