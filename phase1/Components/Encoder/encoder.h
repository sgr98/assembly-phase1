#include <iostream>
#include <bitset>
#include <vector>
using namespace std;

struct bitIns {
    bitset<32> word; 
};

// struct lex {
//     char lexes[10];
// };

// struct instruction {
//     struct lex lexeme[4];
//     char type;   // Currently R = R-TYPE and I = I-TYPE
//     int size = 0;
// };

class Encoder {
    // R-TYPES (3 args)
    // ADD   :   00 0001
    // SUB   :   00 0010
    // I-TYPES (3 args)
    // ADDI  :   01 0001
    // BNE   :   01 0010
    // J-TYPES (1 args)
    // JUMP  :   10 0001
    // L-TYPES (2 args)
    // LD    :   11 0001
    // ST    :   11 0010
    
    public:
        vector<struct bitIns> encodedInstructions;

        Encoder(vector<struct instruction> instructions);
        vector<struct bitIns> encodeAll(vector<struct instruction> instructions);
        struct bitIns encode(struct instruction ins);
        void printEncodeInstructions(vector<struct bitIns> bins);
};