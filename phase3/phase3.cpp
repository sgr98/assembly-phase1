#include <iostream>
#include <fstream>
#include <string>

#include "./Extras/functions.h"
#include "./Components/LexicalAnalyser/lexical_analyser.h"
#include "./Components/Encoder/encoder.h"
#include "./Components/Processor/processor.h"
using namespace std;

//  COMPILE THIS:
//  g++ .\phase3.cpp .\Extras\functions.cpp .\Components\Processor\Memory\memory.cpp .\Components\Processor\processor.cpp .\Components\LexicalAnalyser\lexical_analyser.cpp .\Components\Encoder\encoder.cpp -o phase3
//  EXECUTE THIS:
//  ./phase3 ./Sample/cache_specs.txt ./Sample/bubble_sort.asm

int main(int argc, char** argv) {
    ifstream obj;
    //  Getting the cache_specs.txt
    char *specis = new char[100];
    obj.open(argv[1], ios::in);
    obj.getline(specis, 100);
    int i = 0;
    for(i = 0; specis[i] != '\0'; i++);
    specis[i] = '\n';
    obj.read((specis + i + 1), 900);
    obj.close();

    //  Getting cache specifications in integer format
    int *cacheSpecis = new int[9];
    getInts(cacheSpecis, specis);
    delete[] specis;


    Processor processor;
    processor.initialiseRegisters();


    //  Getting the bubble_sort.asm
    char *arr = new char[1500];
    obj.open(argv[2], ios::in);         // Enter filename: ./Testing/t.asm  |   ./bubble_sort.asm   |   ./Testing/rigorous.asm
    obj.getline(arr, 100);
    i = 0;
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
    processor.initialiseMemory(LexAnl.memSetups, cacheSpecis[3], cacheSpecis[0], cacheSpecis[4], cacheSpecis[1], cacheSpecis[5], cacheSpecis[2]);
    processor.memory.setLatency(cacheSpecis[8], cacheSpecis[6], cacheSpecis[7]);
    processor.memory.printSpecifications();

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