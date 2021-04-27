#include<iostream>
using namespace std;
#include "./../Components/Processor/Memory/memory.h"

int main() {
    int n;
    cin >> n;

    Memory memory;
    memory.initialise(1024, 64, 2, 256, 4, 4);
    
    memory.ram.rdata[9] = 8;
    cout << memory.ram.rdata[9];
    RAM ram;
    ram.initialise(1024, 4);


    return 0;
}