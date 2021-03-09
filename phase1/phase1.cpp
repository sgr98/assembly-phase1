#include <iostream>
#include <fstream>
#include <string>
#include "./Components/LexicalAnalyser/lexical_analyser.h"
using namespace std;

int main() {
    char arr[1000];
    ifstream obj;
    obj.open("./sample1.asm", ios::in);
    obj.getline(arr, 100);
    int i = 0;
    for(i = 0; arr[i] != '\0'; i++);
    arr[i] = '\n';
    obj.read((arr + i + 1), 900);
    obj.close();
    
    // TURNING IT INTO LEXICAL UNITS
    LexicalAnalyser LexAnl(arr);
    LexAnl.printInstructions(LexAnl.instructions);
    cout << LexAnl.instructions.size();
    return 0;
}