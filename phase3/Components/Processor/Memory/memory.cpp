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

void RAM::getBlockData(int *blockData, int blockNum, int blockSize) {
    int k = blockNum * blockSize;
    int j = 0;
    for(int i = k; i < k + blockSize; i++) {
        blockData[j] = this->rdata[i];
        j++;
    }
}


////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//  Cache
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
Cache::Cache() {
    
}

void Cache::initialise(int cSize, int assoc, int blkSize, int addressBits) {
    this->cacheSize = cSize;
    this->numLines = cSize / blkSize;

    this->numSets = this->numLines / assoc;
    this->numLinesPerSet = assoc;
    
    this->indexBit = my_log2(this->numSets);
    int offset = my_log2(blkSize);
    this->tagBit = addressBits - this->indexBit - offset;

    this->cdata = new int*[cSize];
    for(int i = 0; i < cSize; i++)
        this->cdata[i] = new int[blkSize];
    for(int i = 0; i < cSize; i++)
        for(int j = 0; j < blkSize; j++)
            this->cdata[i][j] = 0;
    

    this->tag = new int[this->numLines];
    for(int i = 0; i < this->numLines; i++)
        this->tag[i] = 0;

    this->LRU = new int[this->numLines];
    for(int i = 0; i < this->numLines; i++)
        this->LRU[i] = -1;

    this->dirty = new bool[this->numLines];
    for(int i = 0; i < this->numLines; i++)
        this->dirty[i] = false;
}

void Cache::setLatency(int ltc) {
    this->clatency = ltc;
}

int Cache::search(int address, int blockSize) {
    int blockNum = address / blockSize;
    int setIndex = blockNum % this->numSets;
    int k = setIndex * this->numLinesPerSet;
    int serTag = blockNum / this->numSets;
    int in = -1;
    //  Searching serTag using setIndex
    for(int i = k; i < k + this->numLinesPerSet && this->LRU[i] != -1; i++) {
        if(this->tag[i] == serTag) {
            in = i;
            break;
        }
    }
    //  -1      :   Tag not present
    //  not -1  :   Tag present
    //  Updating LRU
    if(in != -1) {
        int lruInd = this->LRU[in];
        for(int i = k; i < k + this->numLinesPerSet && this->LRU[i] != -1; i++) {
            if(this->LRU[i] < lruInd) {
                this->LRU[i]++;
            }
        }
        this->LRU[in] = 0;
    }
    return in;
}

int Cache::insertData(int blockNum, int *blockData, int blockSize) {
    int setIndex = blockNum % this->numSets;
    int k = setIndex * this->numLinesPerSet;
    int inserTag = blockNum / this->numSets;
    int in = -1;

    // cout << "Set Index => " << setIndex << endl;
    // cout << "K => " << k << endl;
    // cout << "Inserted Tag => " << inserTag << endl;
    // cout << "Block Num => " << blockNum << endl;
    // cout << "Block Size => " << blockSize << endl;

    for(int i = k; i < k + this->numLinesPerSet; i++) {
        if(this->LRU[i] == -1) {
            in = i;
            break;
        }
    }
    
    if(in != -1) {  //  in == 1: Found an empty spcace
        for(int i = k; i <= in; i++) {
            this->LRU[i]++;
        }
        this->LRU[in] = 0;
        this->tag[in] = inserTag;
        for(int i = 0; i < blockSize; i++) {
            this->cdata[in][i] = blockData[i];
        }
        return -1;
    }
    else {    //  Have to replace a line
        int retBlockNum;
        for(int i = k; i < k + this->numLinesPerSet; i++) {
            this->LRU[i]++;
            if(this->LRU[i] == this->numLinesPerSet) {
                in = i;
            }
        }
        this->LRU[in] = 0;
        retBlockNum = this->tag[in];
        this->tag[in] = inserTag;
        for(int j = 0; j < blockSize; j++) {
            this->cdata[in][j] = blockData[j];
        }

        retBlockNum = (retBlockNum * this->numSets) + setIndex;
        return retBlockNum;
    }    

}

void Cache::printCacheData(int blockSize) {
    cout << "LIndex\t| Dirty\t| LRU\t| tag\t| cdata: \n";
    for(int i = 0; i < this->numLines; i++) {
        if(i % this->numLinesPerSet == 0) {
            cout << "*****************************************\n";
        }
        cout << i << "\t|";
        cout << this->dirty[i] << "\t| ";
        cout << this->LRU[i] << "\t| ";
        cout << this->tag[i] << "\t| ";
        for(int j = 0; j < blockSize; j++) {
            cout << this->cdata[i][j] << " ";
        }
        cout << endl;
    }
}


////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//  Memory
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
Memory::Memory() {

}

void Memory::initialise(int rSize, int cSize1, int assoc1, int cSize2, int assoc2, int blkSize) {
    this->blockSize = blkSize;
    this->addressBits = my_log2(rSize);
    this->offset = my_log2(blkSize);

    this->ram.initialise(rSize, blkSize);
    this->L1.initialise(cSize1, assoc1, blkSize, this->addressBits);
    this->L2.initialise(cSize2, assoc2, blkSize, this->addressBits);
}

void Memory::setLatency(int rltc, int cltc1, int cltc2) {
    this->ram.setLatency(rltc);
    this->L1.setLatency(cltc1);
    this->L2.setLatency(cltc2);
}

void Memory::printSpecifications() {
    cout << "\nxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n";
    cout << "Total Address Bits =>  " << this->addressBits << endl;
    cout << "Total Offset Bits =>  " << this->offset << endl;
    cout << "Block Size =>  " << this->blockSize << endl;
    
    cout << "RAM Size =>  " << this->ram.ramSize << endl;
    cout << "L1 Size =>  " << this->L1.cacheSize << endl;
    cout << "L2 Size =>  " << this->L2.cacheSize << endl;

    cout << "L1 number of Lines =>  " << this->L1.numLines << endl;
    cout << "L2 number of Lines =>  " << this->L2.numLines << endl;

    cout << "Number of Blocks in RAM =>  " << this->ram.numberBlocks << endl;
    cout << "L1 number of Sets =>  " << this->L1.numSets << endl;
    cout << "L2 number of Sets =>  " << this->L2.numSets << endl;
    cout << "Number of Lines in a set in L1 =>  " << this->L1.numLinesPerSet << endl;
    cout << "Number of Lines in a set in L2 =>  " << this->L2.numLinesPerSet << endl;

    cout << "L1 Tag =>  " << this->L1.tagBit << endl;
    cout << "L2 Tag =>  " << this->L2.tagBit << endl;
    cout << "L1 Index Bits =>  " << this->L1.indexBit << endl;
    cout << "L2 Index Bits =>  " << this->L2.indexBit << endl;

    cout << "RAM Latency =>  " << this->ram.rlatency << endl;
    cout << "L1 Latency =>  " << this->L1.clatency << endl;
    cout << "L2 Latency =>  " << this->L2.clatency << endl;
    cout << "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n\n";
}

int Memory::getData(int address) {
    //  0   -   Found Element in L1
    //  1   -   Found Element in L2
    //  2   -   Element not found
    // cout << "address => " << address << endl;
    bool L1search = false;
    bool L2search = false;
    int data = search(address, &L1search, &L2search);
    // cout << "data => " << data << endl;
    if(L1search) {  // Found in L1
        return 0;
    }
    else if(!L1search && L2search) {    // Found in L2
        return 1;
    }
    else if(!L1search && !L2search) {   // Not Found
        int *blockData = new int[this->blockSize];
        int blockNum = address / this->blockSize;
        
        this->ram.getBlockData(blockData, blockNum, this->blockSize);
        blockNum = this->L1.insertData(blockNum, blockData, this->blockSize);
        if(blockNum != -1) {
            this->ram.getBlockData(blockData, blockNum, this->blockSize);
            this->L2.insertData(blockNum, blockData, this->blockSize);
        }
        return 2;
    }
    else {
        cout << "\nxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n";
        cout << "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n";
        cout << "Exceptional Error has occured\n";
        cout << "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n";
        cout << "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n\n";
        return -1;
    }
}

int Memory::search(int address, bool *L1ser, bool *L2ser) {
    *L1ser = false;
    *L2ser = false;
    int L1find = this->L1.search(address, this->blockSize);
    if(L1find != -1) {  // Found in L1
        *L1ser = true;
        int off = address % my_pow2(this->offset);
        return this->L1.cdata[L1find][off];
    }
    else {
        int L2find = this->L2.search(address, this->blockSize);
        if(L2find != -1) {  // Found in L2
            *L2ser = true;
            int off = address % my_pow2(this->offset);
            return this->L2.cdata[L2find][off];
        }
        else {  // Not Found
            return this->ram.rdata[address];
        }
    }
}

void Memory::printCache() {
    cout << "\n\nxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n";
    cout << "------------------------------------\n";
    cout << "L1:\n";
    this->L1.printCacheData(this->blockSize);
    cout << "------------------------------------\n";
    cout << "L2:\n";
    this->L2.printCacheData(this->blockSize);
    cout << "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n\n";
}

int Memory::writeMemory(int address, int value, bool *L1miss, bool *L2miss) {
    //  Write-through approach
    *L1miss = false;
    *L2miss = false;
    int latency = 0;
    int L1find = this->L1.search(address, this->blockSize);
    if(L1find != -1) {  //  Write L1 cahce if present
        int off = address % my_pow2(this->offset);
        this->L1.cdata[L1find][off] = value;
        // this->L1.dirty[L1find] = true;
    }
    else {
        *L1miss = true;
    }
    latency += this->L1.clatency;

    int L2find = this->L2.search(address, this->blockSize);
    if(L2find != -1) {  //  Write L2 cahce if present
        int off = address % my_pow2(this->offset);
        this->L2.cdata[L2find][off] = value;
        // this->L2.dirty[L1find] = true;
    }
    else {
        *L2miss = true;
    }
    latency += this->L2.clatency;

    latency += this->ram.rlatency;
    this->ram.setMemory(address, value);      //  Write Memory
    return latency;
}