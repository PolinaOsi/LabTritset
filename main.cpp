#include <iostream>
#include "tritset.h"

int main(){
    Tritset set{Trit::True, Trit::False, Trit::Unknown, Trit::True};
    cout << set << endl;
    testes();

    return 0;
}
