#include <iostream>
#include <vector>
#include <bitset>
#include "./processor.h"
#include "./../Encoder/encoder.h"
#include "./../LexicalAnalyser/lexical_analyser.h"
using namespace std;

int bitToInt(struct bitIns in, int i, int j) {
    int n = 0;
    int m = 1;
    int k = j;
    while(k >= i) {
        m *= 2;
        n *= 2;
        n += in.word[k];
        k--;
    }
    if(!(j - i == 5 || j - i == 4) && in.word[j] == 1) {
        n = n - m;
    }
    return n;
}

//////////////////////////////////////////////////////
//  INITIALIZE
//////////////////////////////////////////////////////

void Processor::initialiseRegisters() {
    for(int i = 0; i < 32; i++) {
        this->registers[i] = 0;
    }
}

void Processor::initialiseMemory() {
    for(int i = 0; i < max_memory_size; i++) {
        this->memory[i] = 0;
    }
}

//////////////////////////////////////////////////////
//  SET
//////////////////////////////////////////////////////

void Processor::setRegister(int index, int value) {
    this->registers[index] = value;
}

void Processor::setMemory(int index, int value) {
    this->memory[index] = value;
}

//////////////////////////////////////////////////////
//  EXECUTE TYPES
//////////////////////////////////////////////////////

void Processor::add(int dr, int sr1, int sr2) {
    this->registers[dr] =  this->registers[sr1] + this->registers[sr2];
}

void Processor::sub(int dr, int sr1, int sr2) {
    this->registers[dr] =  this->registers[sr1] - this->registers[sr2];
}

void Processor::addi(int aa, int bb, int immediate) {
    this->registers[aa] =  this->registers[bb] + immediate;
}

void Processor::bne(int aa, int bb, int immediate) {
    if(this->registers[aa] != this->registers[bb]) {
        this->registers[0] += immediate;
    }
    else {
        this->registers[0]++;
    }
}

void Processor::beq(int aa, int bb, int immediate) {
    if(this->registers[aa] == this->registers[bb]) {
        this->registers[0] += immediate;
    }
    else {
        this->registers[0]++;
    }
}

void Processor::jump(int immediate) {
    this->registers[0] += immediate;
}

void Processor::ld(int dr, int Ain, int inc) {
    int index = Ain + this->registers[inc];
    // cout << "LD" << endl;
    // cout << "dr = " << dr << endl;
    // cout << "Ain = " << Ain << endl;
    // cout << "inc = " << inc << endl;
    this->registers[dr] = this->memory[index];
}

void Processor::st(int dr, int Ain, int inc) {
    int index = Ain + this->registers[inc];
    // cout << "ST" << endl;
    // cout << "dr = " << dr << endl;
    // cout << "Ain = " << Ain << endl;
    // cout << "inc = " << inc << endl;
    this->memory[index] = this->registers[dr];
}

//////////////////////////////////////////////////////
//  EXECUTE
//////////////////////////////////////////////////////

void Processor::execute(struct bitIns inst) {
    int op = bitToInt(inst, 26, 31);
    //  R-TYPE
    if(inst.word[31] == 0 && inst.word[30] == 0) {
        int dr = bitToInt(inst, 21, 25);
        int sr1 = bitToInt(inst, 16, 20);
        int sr2 = bitToInt(inst, 11, 15);

        if(op == 1) {
            add(dr, sr1, sr2);
            this->registers[0]++;
        }
        else if(op == 2) {
            sub(dr, sr1, sr2);
            this->registers[0]++;
        }
    }
    //  I-TYPE
    else if(inst.word[31] == 0 && inst.word[30] == 1) {
        int aa = bitToInt(inst, 21, 25);
        int bb = bitToInt(inst, 16, 20);
        int immediate = bitToInt(inst, 0, 15);

        if(op == 17) {
            addi(aa, bb, immediate);
            this->registers[0]++;
        }
        else if(op == 18) {
            bne(aa, bb, immediate);
        }
        else if(op == 19) {
            beq(aa, bb, immediate);
        }
    }
    //  J-TYPE
    else if(inst.word[31] == 1 && inst.word[30] == 0) {
        int immediate = bitToInt(inst, 0, 15);
        if(op == 33) {
            jump(immediate);
        }
    }
    //  L-TYPE
    else if(inst.word[31] == 1 && inst.word[30] == 1) {
        int dr = bitToInt(inst, 21, 25);
        int Ain = bitToInt(inst, 16, 20);
        int inc = bitToInt(inst, 11, 15);

        if(op == 49) {
            ld(dr, Ain, inc);
            this->registers[0]++;
        }
        else if(op == 50) {
            st(dr, Ain, inc);
            this->registers[0]++;
        }
    }
}

void Processor::executeAll(vector<struct bitIns> encodedIns) {
    int size = encodedIns.size();
    while(this->registers[0] < size) {
        // cout << "EXECUTE => " << this->registers[0] << endl;
        execute(encodedIns[this->registers[0]]);
    }
}

//////////////////////////////////////////////////////
//  PRINT
//////////////////////////////////////////////////////

void Processor::printRegisters() {
    int i = 0;
    while(i < 16) {
        cout << "R" << i << " = " << this->registers[i] << "\t\t\t";
        cout << "R" << (16 + i) << " = " << this->registers[16 + i];
        cout << endl;
        i++;
    }
}

void Processor::printMemory() {
    int i = 0;
    while(i < max_memory_size) {
        cout << "A" << i << " = " << this->memory[i] << "\t\t\t";
        i++;
        cout << "A" << i << " = " << this->memory[i] << "\t\t\t";
        i++;
        cout << "A" << i << " = " << this->memory[i] << "\t\t\t";
        i++;
        cout << "A" << i << " = " << this->memory[i];
        i++;
        cout << endl;
    }
}