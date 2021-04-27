#include <iostream>
#include <vector>
#include <bitset>
#include "./../../../Extras/functions.h"
#include "./memory.h"
// #include "./../processor.h"
// #include "./../../Encoder/encoder.h"
// #include "./../../LexicalAnalyser/lexical_analyser.h"
using namespace std;

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//  RAM
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
RAM::RAM() {
    
}

void RAM::initialise(int rSize, int blkSize) {
    this->ramSize = rSize;

    this->blockSize = blkSize;
    this->numberBlocks = rSize / blkSize;

    this->rdata = new int[rSize];
    for(int i = 0; i < rSize; i++)
        this->rdata[i] = 0;
}

void RAM::setMemory(int index, int value) {
    this->rdata[index] = value;
}

void RAM::setLatency(int ltc) {
    this->rlatency = ltc;
}


////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//  Cache
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
Cache::Cache() {
    
}

void Cache::initialise(int cSize, int assoc, int blkSize) {
    this->cacheSize = cSize;

    this->numSets = assoc;
    this->setSize = cSize / assoc;

    this->numBlocks = this->setSize / blkSize;
    this->blockSize = blkSize;
    
    this->cdata = new int[cSize];
    for(int i = 0; i < cSize; i++)
        this->cdata[i] = 0;
}

void Cache::setLatency(int ltc) {
    this->clatency = ltc;
}


////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//  Memory
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
Memory::Memory() {

}

void Memory::initialise(int rSize, int cSize1, int assoc1, int cSize2, int assoc2, int blkSize) {
    this->ram.initialise(rSize, blkSize);
    this->L1.initialise(cSize1, assoc1, blkSize);
    this->L2.initialise(cSize2, assoc2, blkSize);
}

void Memory::setLatency(int rltc, int cltc1, int cltc2) {
    this->ram.setLatency(rltc);
    this->L1.setLatency(cltc1);
    this->L2.setLatency(cltc2);
}

void Memory::printSpecifications() {
    cout << "\n\nxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n";
    cout << "RAM Block Size =>\t\t" << this->ram.blockSize << endl;
    cout << "L1 Block Size =>\t\t" << this->L1.blockSize << endl;
    cout << "L2 Block Size =>\t\t" << this->L2.blockSize << endl;
    
    cout << "RAM Size =>\t\t" << this->ram.ramSize << endl;
    cout << "L1 Size =>\t\t" << this->L1.cacheSize << endl;
    cout << "L2 Size =>\t\t" << this->L2.cacheSize << endl;

    cout << "Number of Blocks in RAM =>\t\t" << this->ram.numberBlocks << endl;
    cout << "Number of Blocks in a set in L1 =>\t\t" << this->L1.numBlocks << endl;
    cout << "Number of Blocks in a set in L2 =>\t\t" << this->L2.numBlocks << endl;

    cout << "L1 set Size =>\t\t" << this->L1.setSize << endl;
    cout << "L1 number of Sets =>\t\t" << this->L1.numSets << endl;
    cout << "L2 set Size =>\t\t" << this->L2.setSize << endl;
    cout << "L2 number of Sets =>\t\t" << this->L2.numSets << endl;

    cout << "RAM Latency =>\t\t" << this->ram.rlatency << endl;
    cout << "L1 Latency =>\t\t" << this->L1.clatency << endl;
    cout << "L2 Latency =>\t\t" << this->L2.clatency << endl;
    cout << "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n\n";
}