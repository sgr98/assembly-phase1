#include <iostream>
#include <fstream>
#include <string>

#include "./Components/LexicalAnalyser/lexical_analyser.h"
#include "./Components/Encoder/encoder.h"
#include "./Components/Processor/processor.h"
using namespace std;

//  COMPILE THIS:
//  g++ .\phase1.cpp .\Components\Processor\processor.cpp .\Components\LexicalAnalyser\lexical_analyser.cpp .\Components\Encoder\encoder.cpp -o phase1

int main() {
    Processor processor;
    processor.initialiseRegisters();


    char arr[1500];
    ifstream obj;
    obj.open("./t.asm", ios::in);
    obj.getline(arr, 100);
    int i = 0;
    for(i = 0; arr[i] != '\0'; i++);
    arr[i] = '\n';
    obj.read((arr + i + 1), 900);
    obj.close();
    
    ////////////////////////////////////////////////////////////////////////////
    // TURNING IT INTO LEXICAL UNITS
    ////////////////////////////////////////////////////////////////////////////
    LexicalAnalyser LexAnl(arr);
    cout << "INSTRUCTIONS:" << endl;
    LexAnl.printInstructions(LexAnl.instructions);
    cout << "The number of instructions: " << LexAnl.instructions.size() << endl << endl;

    //  Initialising memory according to the file
    processor.initialiseMemory(LexAnl.memSetups);

    // cout << "xxxxxxxxxxxxxxxxx" << endl;
    // LexAnl.printInstructions(LexAnl.memSetups);
    // cout << "xxxxxxxxxxxxxxxxx" << endl;

    ////////////////////////////////////////////////////////////////////////////
    //TURNING LEXICAL UNITS INTO BIT INSTRUCTIONS
    ////////////////////////////////////////////////////////////////////////////
    Encoder encoder(LexAnl.instructions);
    cout << "INSTRUCTIONS IN BITS:" << endl;
    encoder.printEncodeInstructions(encoder.encodedInstructions);
    cout << "The number of bit instructions: " << encoder.encodedInstructions.size() << endl << endl;

    ////////////////////////////////////////////////////////////////////////////
    //EXECUTING THE PROGRAM
    ////////////////////////////////////////////////////////////////////////////
    cout << "------------------------------------------------------" << endl;
    cout << "MEMORY BEFORE EXECUTION:" << endl;
    cout << "------------------------------------------------------" << endl;
    processor.printMemory();

    cout << "PROGRAM IS EXECUTED-" << endl;
    processor.executeAll(encoder.encodedInstructions);

    cout << "------------------------------------------------------" << endl;
    cout << "REGISTERS:" << endl;
    cout << "------------------------------------------------------" << endl;
    processor.printRegisters();
    cout << "------------------------------------------------------" << endl;
    cout << "MEMORY:" << endl;
    cout << "------------------------------------------------------" << endl;
    processor.printMemory();

    return 0;
}