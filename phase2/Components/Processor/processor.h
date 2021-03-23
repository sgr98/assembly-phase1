#include <iostream>
#include <vector>
#include <bitset>
using namespace std;

#define max_memory_size 1000

class Processor {
    public:
        int registers[32];
        int *memory = new int[max_memory_size];

        // INITIALIZE
        void initialiseRegisters();
        void initialiseMemory(vector<struct instruction> memSetups);

        // SET
        void setRegister(int index, int value);
        void setMemory(int index, int value);

        // R-TYPE
        void add(int dr, int sr1, int sr2);
        void sub(int dr, int sr1, int sr2);
        // I-TYPE
        void addi(int aa, int bb, int immediate);
        void bne(int aa, int bb, int immediate);
        void beq(int aa, int bb, int immediate);
        void bgt(int aa, int bb, int immediate);
        void bge(int aa, int bb, int immediate);
        // J-TYPE
        void jump(int immediate);
        // L-TYPE
        void ld(int dr, int Ain, int inc);
        void st(int dr, int Ain, int inc);

        // EXECUTE
        void execute(struct bitIns inst);
        void executeAll(vector<struct bitIns> encodedIns);

        // PRINT
        void printRegisters();
        void printMemory();
};