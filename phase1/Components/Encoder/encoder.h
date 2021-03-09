#include <iostream>
#include <bitset>
#include <vector>
using namespace std;

struct bitIns {
    bitset<32> word; 
};

class Encoder {
    // R-TYPES (3 args)     ////(0 - 15)
    // ADD   :   00 0001    (1)
    // SUB   :   00 0010    (2)
    // I-TYPES (3 args)     ////(16 - 31)
    // ADDI  :   01 0001    (17)
    // BNE   :   01 0010    (18)
    // BEQ   :   01 0011    (19)
    // J-TYPES (1 args)     ////(32 - 47)
    // JUMP  :   10 0001    (33)
    // L-TYPES (3 args)     ////(48 - 63)
    // LD    :   11 0001    (49)
    // ST    :   11 0010    (50)
    
    public:
        vector<struct bitIns> encodedInstructions;

        Encoder(vector<struct instruction> instructions);
        vector<struct bitIns> encodeAll(vector<struct instruction> instructions);
        struct bitIns encode(struct instruction ins);
        void printEncodeInstructions(vector<struct bitIns> bins);
};