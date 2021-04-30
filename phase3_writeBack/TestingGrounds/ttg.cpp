#include<iostream>
using namespace std;
// #include "./../Components/Processor/Memory/memory.h"

void change(bool *x, bool *y) {
    *x = 0;
    *y = 1;
}

int main() {
    bool a = 1;
    bool b = 0;

    change(&a, &b);

    cout << a << endl;
    cout << b << endl;

    return 0;
}