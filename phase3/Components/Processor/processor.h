#include <iostream>
#include <vector>
#include <bitset>
#include "./Memory/memory.h"
using namespace std;
#define initialValue -1

class Processor {
    public:
        Memory memory;

        int registers[32];

        int my_clock = 0;
        int totalStalls = 0;

        int memoryAccesses = 0;
        int L1misses = 0;
        int L2misses = 0;

        vector<int> stallInstructionIndex;
        int totalInstructions = 0;

        // INITIALIZE
        void init(int rSize, int cSize1, int assoc1, int cSize2, int assoc2, int blkSize);
        void initialiseRegisters();
        void initialiseMemory(vector<struct instruction> memSetups, int rSize, int cSize1, int assoc1, int cSize2, int assoc2, int blkSize);

        // SET
        void setRegister(int index, int value);

        // R-TYPE
        int add(int sr1, int sr2);
        int sub(int sr1, int sr2);
        // I-TYPE
        bool bne(int aa, int bb);
        bool beq(int aa, int bb);
        bool bgt(int aa, int bb);
        bool bge(int aa, int bb);

        // EXECUTE
        void execute(vector<struct bitIns> encodedIns);

        // PRINT
        void printRegisters();
        void printMemory();
        void print_information();

        // PHASE2
        void execute_noForwarding(vector<struct bitIns> encodedIns);
        void execute_Forwarding(vector<struct bitIns> encodedIns);

        struct bitIns IF(vector<struct bitIns> encodedIns, int index);
        void cleanStallInstructions();
};