#include <iostream>
#include <vector>
#include <bitset>
using namespace std;

#define max_memory_size 100
#define initialValue -1

class Processor {
    public:
        int registers[32];
        int *memory = new int[max_memory_size];

        int my_clock = 0;
        int totalStalls = 0;
        vector<int> stallInstructionIndex;
        int totalInstructions = 0;

        // INITIALIZE
        void initialiseRegisters();
        void initialiseMemory(vector<struct instruction> memSetups);

        // SET
        void setRegister(int index, int value);
        void setMemory(int index, int value);

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