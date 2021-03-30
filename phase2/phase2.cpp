#include <iostream>
#include <fstream>
#include <string>

#include "./Components/LexicalAnalyser/lexical_analyser.h"
#include "./Components/Encoder/encoder.h"
#include "./Components/Processor/processor.h"
using namespace std;

//  COMPILE THIS:
//  g++ .\phase2.cpp .\Components\Processor\processor.cpp .\Components\LexicalAnalyser\lexical_analyser.cpp .\Components\Encoder\encoder.cpp -o phase2

int main() {
    Processor processor;
    processor.initialiseRegisters();


    char *arr = new char[1500];
    ifstream obj;
    obj.open("./bubble_sort.asm", ios::in);         // Enter filename: ./Testing/t.asm  |   ./bubble_sort.asm   |   ./Testing/rigorous.asm
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
    LexAnl.printInstructions();
    cout << "The number of instructions: " << LexAnl.instructions.size() << endl << endl;
    delete[] arr;

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

    cout << "\n\nPROGRAM IS EXECUTED-" << endl;
    processor.execute(encoder.encodedInstructions);

    cout << "------------------------------------------------------" << endl;
    cout << "REGISTERS:" << endl;
    cout << "------------------------------------------------------" << endl;
    processor.printRegisters();
    cout << "------------------------------------------------------" << endl;
    cout << "MEMORY:" << endl;
    cout << "------------------------------------------------------" << endl;
    processor.printMemory();
    cout << "------------------------------------------------------" << endl;
    cout << "DETAILS OF EXECUTION:" << endl;
    cout << "------------------------------------------------------" << endl;
    processor.print_information();
    LexAnl.printStalledInstructions(processor.stallInstructionIndex);
    
    return 0;
}