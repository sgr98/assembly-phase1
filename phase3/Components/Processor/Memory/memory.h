#include <iostream>
#include <vector>
#include <bitset>
using namespace std;

class RAM {
    public:
        int ramSize;

        int numberBlocks;

        int rlatency;

        int *rdata;

        RAM();
        void initialise(int rSize, int blkSize);
        void setLatency(int ltc);
        void setMemory(int index, int value);
        void getBlockData(int *blockData, int blockNum, int blockSize);
};

class Cache {
    public:
        int cacheSize;

        int numLines;
        int numSets;
        
        int numLinesPerSet;

        int clatency;

        int tagBit;
        int indexBit;
        
        int **cdata;
        int *tag;
        int *LRU;
        bool *dirty;

        Cache();
        void initialise(int cSize, int assoc, int blkSize, int addressBits);
        void setLatency(int ltc);

        int search(int address, int blockSize);
        int insertData(int blockNum, int *blockData, int blockSize);
        void printCacheData(int blockSize);
};


class Memory {
    public:
        RAM ram;
        Cache L1;
        Cache L2;

        int blockSize;
        int addressBits;
        int offset;

        Memory();
        void initialise(int rSize, int cSize1, int assoc1, int cSize2, int assoc2, int blkSize);
        void setLatency(int rltc, int cltc1, int cltc2);
        void printSpecifications();

        int getData(int address);
        int search(int address, bool *L1ser, bool *L2ser);
        void printCache();

        int writeMemory(int address, int value, bool *L1miss, bool *L2miss);
};