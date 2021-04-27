#include <iostream>
#include <vector>
#include <bitset>
using namespace std;

class RAM {
    public:
        int ramSize;

        int blockSize;
        int numberBlocks;

        int rlatency;

        int *rdata;

        RAM();
        void initialise(int rSize, int blkSize);
        void setLatency(int ltc);
        void setMemory(int index, int value);
};

class Cache {
    public:
        int cacheSize;

        int numSets;
        int setSize;
        
        int numBlocks;
        int blockSize;

        int clatency;
        
        int *cdata;

        Cache();
        void initialise(int cSize, int assoc, int blkSize);
        void setLatency(int ltc);
};


class Memory {
    public:
        RAM ram;
        Cache L1;
        Cache L2;

        Memory();
        void initialise(int rSize, int cSize1, int assoc1, int cSize2, int assoc2, int blkSize);
        void setLatency(int rltc, int cltc1, int cltc2);
        void printSpecifications();
};