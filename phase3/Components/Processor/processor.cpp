#include <iostream>
#include <vector>
#include <bitset>
#include "./../../Extras/functions.h"
#include "./processor.h"
#include "./../Encoder/encoder.h"
#include "./../LexicalAnalyser/lexical_analyser.h"
// #include "./Memory/memory.h"
using namespace std;

//////////////////////////////////////////////////////
//  INITIALIZE
//////////////////////////////////////////////////////
void Processor::init(int rSize, int cSize1, int assoc1, int cSize2, int assoc2, int blkSize) {
    this->memory.initialise(rSize, cSize1, assoc1, cSize2, assoc2, blkSize);
}

void Processor::initialiseRegisters() {
    for(int i = 0; i < 32; i++) {
        this->registers[i] = 0;
    }
}

void Processor::initialiseMemory(vector<struct instruction> memInits, int rSize, int cSize1, int assoc1, int cSize2, int assoc2, int blkSize) {
    init(rSize, cSize1, assoc1, cSize2, assoc2, blkSize);
    int size = memInits.size();
    for(int i = 0; i < size; i++) {
        int index = strToInt(memInits[i].lexeme[0].lexes);
        int value = strToInt(memInits[i].lexeme[2].lexes);
        this->memory.ram.setMemory(index, value);
    }
}

//////////////////////////////////////////////////////
//  SET
//////////////////////////////////////////////////////

void Processor::setRegister(int index, int value) {
    this->registers[index] = value;
}

//////////////////////////////////////////////////////
//  EXECUTE TYPES
//////////////////////////////////////////////////////

int Processor::add(int sr1, int sr2) {
    return sr1 + sr2;
}

int Processor::sub(int sr1, int sr2) {
    return sr1 - sr2;
}

bool Processor::bne(int aa, int bb) {
    if(this->registers[aa] != this->registers[bb]) {
        return true;
    }
    return false;
}

bool Processor::beq(int aa, int bb) {
    if(this->registers[aa] == this->registers[bb]) {
        return true;
    }
    return false;
}

bool Processor::bgt(int aa, int bb) {
    if(this->registers[aa] > this->registers[bb]) {
        return true;
    }
    return false;
}

bool Processor::bge(int aa, int bb) {
    if(this->registers[aa] >= this->registers[bb]) {
        return true;
    }
    return false;
}

//////////////////////////////////////////////////////
//  EXECUTE
//////////////////////////////////////////////////////

void Processor::execute(vector<struct bitIns> encodedIns) {
    int dataFor;
    cout << "\nxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n";
    cout << "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n";
    cout << "Enter:\n";
    cout << "0: To perform execution without Data Forwarding\n";
    cout << "<Any other number>: To perform execution with Data Forwarding\n";
    cin >> dataFor;
    cout << "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n";
    cout << "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n\n";
    if(dataFor == 0) {
        execute_noForwarding(encodedIns);
    }
    else {
        execute_Forwarding(encodedIns);
    }
}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
// WITHOUT FORWARDING
void Processor::execute_noForwarding(vector<struct bitIns> encodedIns) {
    int size = encodedIns.size();
    struct bitIns currInstruction;

    bool WBoper = false;
    bool MEMoper = false;
    bool EXoper = false;
    bool IRoper = false;
    bool IFoper = true;
    
    int opCode = initialValue;
    int registerSource1 = initialValue;
    int registerSource2 = initialValue;
    int registerDestination = initialValue;
    int immediate = -2;

    int EXopCode = initialValue;
    int destinationRegisterEX = initialValue;
    int EXResult = initialValue;

    int MEMopCode = initialValue;
    int destinationRegisterMEM = initialValue;
    int MEMResult = initialValue;

    int destinationRegisterWB = initialValue;

    int stallType = 0;
    bool branchDec= false;

    bool uniqueCase1 = false;

    while(IFoper || IRoper || EXoper || MEMoper || WBoper) {
        my_clock++;

        ////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////
        // WB Stage
        ////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////
        if(WBoper) {
            this->totalInstructions++;
            WBoper = false;
            destinationRegisterWB = destinationRegisterMEM;
            if(MEMopCode >= 0 && MEMopCode <= 15) {
                if(MEMopCode == 1) {
                    setRegister(destinationRegisterWB, MEMResult);
                }
                else if(MEMopCode == 2) {
                    setRegister(destinationRegisterWB, MEMResult);
                }
            }
            else if(MEMopCode >= 16 && MEMopCode <= 31) {
                if(MEMopCode == 17) {
                    setRegister(destinationRegisterWB, MEMResult);
                }
            }

            if(stallType > 0 && !MEMoper) {
                if(stallType == 1) {
                    destinationRegisterMEM = -1;
                    destinationRegisterWB = -1;
                }
                stallType--;
                continue;
            }
        }

        ////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////
        // MEM Stage
        ////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////
        if(MEMoper) {
            WBoper = true;
            MEMoper = false;
            MEMopCode = EXopCode;
            destinationRegisterMEM = destinationRegisterEX;
            MEMResult = EXResult;

            if(MEMopCode >= 48 && MEMopCode <= 63) {
                if(MEMopCode == 49) {
                    setRegister(destinationRegisterMEM, this->memory.ram.rdata[MEMResult]);
                }
                else if(MEMopCode == 50) {
                    this->memory.ram.setMemory(MEMResult, this->registers[destinationRegisterMEM]);
                }
            }

            if(stallType > 0) {
                if(EXoper) {
                    uniqueCase1 = true;
                }
                destinationRegisterEX = -1;
                stallType--;
                continue;
            }
        }

        ////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////
        // EX Stage
        ////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////
        if(EXoper) {
            MEMoper = true;
            EXoper = false;
            EXopCode = opCode;
            destinationRegisterEX = registerDestination;
            
            if(EXopCode >= 0 && EXopCode <= 15) {
                if(EXopCode == 1) {
                    EXResult = add(this->registers[registerSource1], this->registers[registerSource2]);
                }
                else if(EXopCode == 2) {
                    EXResult = sub(this->registers[registerSource1], this->registers[registerSource2]);
                }
            }
            else if(EXopCode >= 16 && EXopCode <= 31) {
                if(EXopCode == 17) {
                    EXResult = add(this->registers[registerSource1], immediate);
                }
                else if(EXopCode == 18) {
                    if(bne(registerDestination, registerSource1))
                        this->registers[0] += immediate;
                    else
                        this->registers[0]++;
                    stallInstructionIndex.push_back(this->registers[0]);
                }
                else if(EXopCode == 19) {
                    if(beq(registerDestination, registerSource1))
                        this->registers[0] += immediate;
                    else
                        this->registers[0]++;
                    stallInstructionIndex.push_back(this->registers[0]);
                }
                else if(EXopCode == 20) {
                    if(bgt(registerDestination, registerSource1))
                        this->registers[0] += immediate;
                    else
                        this->registers[0]++;
                    stallInstructionIndex.push_back(this->registers[0]);
                }
                else if(EXopCode == 21) {
                    if(bge(registerDestination, registerSource1))
                        this->registers[0] += immediate;
                    else
                        this->registers[0]++;
                    stallInstructionIndex.push_back(this->registers[0]);
                }
            }
            else if(EXopCode >= 32 && EXopCode <= 47) {
                if(EXopCode == 33) {
                    this->registers[0] += immediate;
                }
                stallInstructionIndex.push_back(this->registers[0]);
            }
            else if(EXopCode >= 48 && EXopCode <= 63) {
                if(EXopCode == 49) {
                    EXResult = add(registerSource1, this->registers[registerSource2]);
                }
                else if(EXopCode == 50) {
                    EXResult = add(registerSource1, this->registers[registerSource2]);
                }
            }
        }

        ////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////
        // ID/RF Stage
        ////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////
        if(IRoper) {
            opCode = bitToInt(currInstruction, 26, 31);

            if(opCode >= 0 && opCode <= 15) {
                registerDestination = bitToInt(currInstruction, 21, 25);
                registerSource1 = bitToInt(currInstruction, 16, 20);
                registerSource2 = bitToInt(currInstruction, 11, 15);

                if(destinationRegisterEX == registerSource1 || destinationRegisterEX == registerSource2) {
                    stallInstructionIndex.push_back(this->registers[0]);
                    if(WBoper && !MEMoper) {
                        totalStalls += 2;
                        stallType = 1;
                        destinationRegisterEX = -1;
                    }
                    else {
                        totalStalls += 3;
                        stallType = 2;
                    }
                    continue;
                }
                else if(destinationRegisterMEM == registerSource1 || destinationRegisterMEM == registerSource2) {
                    stallInstructionIndex.push_back(this->registers[0]);
                    totalStalls += 2;
                    stallType = 1;
                    destinationRegisterEX = -1;
                    continue;
                }
                else if(destinationRegisterWB == registerSource1 || destinationRegisterWB == registerSource2) {
                    stallInstructionIndex.push_back(this->registers[0]);
                    totalStalls += 1;
                    continue;
                }
                this->registers[0]++;
            }

            else if(opCode >= 16 && opCode <= 31) {
                registerDestination = bitToInt(currInstruction, 21, 25);
                registerSource1 = bitToInt(currInstruction, 16, 20);
                immediate = bitToInt(currInstruction, 0, 15);

                if(opCode >= 17 && opCode <= 17) {
                    if(destinationRegisterEX == registerSource1) {
                        stallInstructionIndex.push_back(this->registers[0]);
                        if(WBoper && !MEMoper) {
                            totalStalls += 2;
                            stallType = 1;
                            destinationRegisterEX = -1;
                        }
                        else {
                            totalStalls += 3;
                            stallType = 2;
                        }
                        continue;
                    }
                    else if(destinationRegisterMEM == registerSource1) {
                        stallInstructionIndex.push_back(this->registers[0]);
                        totalStalls += 2;
                        stallType = 1;
                        destinationRegisterEX = -1;
                        continue;
                    }
                    else if(destinationRegisterWB == registerSource1) {
                        stallInstructionIndex.push_back(this->registers[0]);
                        totalStalls += 1;
                        continue;
                    }
                    this->registers[0]++;
                }
                else {
                    if(destinationRegisterEX == registerSource1 || destinationRegisterEX == registerDestination) {
                        stallInstructionIndex.push_back(this->registers[0]);
                        if(WBoper && !MEMoper) {
                            totalStalls += 2;
                            stallType = 1;
                            destinationRegisterEX = -1;
                        }
                        else {
                            totalStalls += 3;
                            stallType = 2;
                        }
                        continue;
                    }
                    else if(destinationRegisterMEM == registerSource1 || destinationRegisterMEM == registerDestination) {
                        stallInstructionIndex.push_back(this->registers[0]);
                        totalStalls += 2;
                        stallType = 1;
                        continue;
                    }
                    else if(destinationRegisterWB == registerSource1 || destinationRegisterWB == registerDestination) {
                        stallInstructionIndex.push_back(this->registers[0]);
                        totalStalls += 1;
                        continue;
                    }
                    else {
                        branchDec = true;
                    }
                }
                
            }

            else if(opCode >= 32 && opCode <= 47) {
                immediate = bitToInt(currInstruction, 0, 15);
                branchDec = true;
            }

            else if(opCode >= 48 && opCode <= 63) {
                registerDestination = bitToInt(currInstruction, 21, 25);
                registerSource1 = bitToInt(currInstruction, 16, 20);
                registerSource2 = bitToInt(currInstruction, 11, 15);

                if(opCode == 49) {
                    if(destinationRegisterEX == registerSource2) {
                        stallInstructionIndex.push_back(this->registers[0]);
                        if(WBoper && !MEMoper) {
                            totalStalls += 2;
                            stallType = 1;
                            destinationRegisterEX = -1;
                        }
                        else {
                            totalStalls += 3;
                            stallType = 2;
                        }
                        continue;
                    }
                    else if(destinationRegisterMEM == registerSource2) {
                        stallInstructionIndex.push_back(this->registers[0]);
                        totalStalls += 2;
                        stallType = 1;                   
                        continue;
                    }
                    else if(destinationRegisterWB == registerSource2) {
                        stallInstructionIndex.push_back(this->registers[0]);
                        totalStalls += 1;
                        continue;
                    }
                }
                else if(opCode == 50) {
                    if(destinationRegisterEX == registerDestination || destinationRegisterEX == registerSource2) {
                        stallInstructionIndex.push_back(this->registers[0]);
                        if(WBoper && !MEMoper) {
                            totalStalls += 2;
                            stallType = 1;
                            destinationRegisterEX = -1;
                        }
                        else {
                            totalStalls += 3;
                            stallType = 2;
                        }
                        continue;
                    }
                    else if(destinationRegisterEX == registerDestination || destinationRegisterMEM == registerSource2) {
                        stallInstructionIndex.push_back(this->registers[0]);
                        totalStalls += 2;
                        stallType = 1;
                        continue;
                    }
                    else if(destinationRegisterEX == registerDestination || destinationRegisterWB == registerSource2) {
                        stallInstructionIndex.push_back(this->registers[0]);
                        totalStalls += 1;
                        continue;
                    }
                }
                this->registers[0]++;
            }

            EXoper = true;
            IRoper = false;
        }

        ////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////
        // IF Stage
        ////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////
        if(IFoper) {
            if(this->registers[0] < size) {
                currInstruction = IF(encodedIns, this->registers[0]);
                IRoper = true;
                if(branchDec) {
                    totalStalls++;
                    IRoper = false;
                    branchDec = false;
                    continue;
                }
            }
            else {
                IFoper = false;
            }
        }
    }
}


////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
// WITH FORWARDING
void Processor::execute_Forwarding(vector<struct bitIns> encodedIns) {
    int size = encodedIns.size();
    struct bitIns currInstruction;

    bool WBoper = false;
    bool MEMoper = false;
    bool EXoper = false;
    bool IRoper = false;
    bool IFoper = true;
    
    int opCode = initialValue;
    int registerSource1 = initialValue;
    int registerSource2 = initialValue;
    int registerDestination = initialValue;
    int immediate = -2;

    int EXopCode = initialValue;
    int destinationRegisterEX = initialValue;
    int EXResult = initialValue;

    int MEMopCode = initialValue;
    int destinationRegisterMEM = initialValue;
    int MEMResult = initialValue;

    int destinationRegisterWB = initialValue;
    
    bool branchDec= false;
    bool loadDec = false;

    cout << "WITH DATA FORWARDING\n";

    while(IFoper || IRoper || EXoper || MEMoper || WBoper) {
        my_clock++;

        ////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////
        // WB Stage
        ////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////
        if(WBoper) {
            this->totalInstructions++;
            WBoper = false;
            destinationRegisterWB = destinationRegisterMEM;
        }

        ////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////
        // MEM Stage
        ////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////
        if(MEMoper) {
            WBoper = true;
            MEMoper = false;
            MEMopCode = EXopCode;
            destinationRegisterMEM = destinationRegisterEX;
            MEMResult = EXResult;

            if(MEMopCode >= 48 && MEMopCode <= 63) {
                if(MEMopCode == 49) {
                    setRegister(destinationRegisterMEM, this->memory.ram.rdata[MEMResult]);
                }
                else if(MEMopCode == 50) {
                    this->memory.ram.setMemory(MEMResult, this->registers[destinationRegisterMEM]);
                }
            }

        }

        ////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////
        // EX Stage
        ////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////
        if(EXoper) {
            EXopCode = opCode;
            destinationRegisterEX = registerDestination;
            
            if(EXopCode >= 0 && EXopCode <= 15) {
                if(EXopCode == 1) {
                    EXResult = add(this->registers[registerSource1], this->registers[registerSource2]);
                    setRegister(destinationRegisterEX, EXResult);
                }
                else if(EXopCode == 2) {
                    EXResult = sub(this->registers[registerSource1], this->registers[registerSource2]);
                    setRegister(destinationRegisterEX, EXResult);
                }
            }
            else if(EXopCode >= 16 && EXopCode <= 31) {
                if(EXopCode == 17) {
                    EXResult = add(this->registers[registerSource1], immediate);
                    setRegister(destinationRegisterEX, EXResult);
                }
                else if(EXopCode == 18) {
                    if(bne(registerDestination, registerSource1))
                        this->registers[0] += immediate;
                    else
                        this->registers[0]++;
                    stallInstructionIndex.push_back(this->registers[0]);
                }
                else if(EXopCode == 19) {
                    if(beq(registerDestination, registerSource1))
                        this->registers[0] += immediate;
                    else
                        this->registers[0]++;
                    stallInstructionIndex.push_back(this->registers[0]);
                }
                else if(EXopCode == 20) {
                    if(bgt(registerDestination, registerSource1))
                        this->registers[0] += immediate;
                    else
                        this->registers[0]++;
                    stallInstructionIndex.push_back(this->registers[0]);
                }
                else if(EXopCode == 21) {
                    if(bge(registerDestination, registerSource1))
                        this->registers[0] += immediate;
                    else
                        this->registers[0]++;
                    stallInstructionIndex.push_back(this->registers[0]);
                }
            }
            else if(EXopCode >= 32 && EXopCode <= 47) {
                if(EXopCode == 33) {
                    this->registers[0] += immediate;
                }
                stallInstructionIndex.push_back(this->registers[0]);
            }
            else if(EXopCode >= 48 && EXopCode <= 63) {
                if(EXopCode == 49) {
                    EXResult = add(registerSource1, this->registers[registerSource2]);
                }
                else if(EXopCode == 50) {
                    EXResult = add(registerSource1, this->registers[registerSource2]);
                }
            }

            if(EXopCode == 49) {
                loadDec = true;
            }

            MEMoper = true;
            EXoper = false;
        }

        ////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////
        // ID/RF Stage
        ////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////
        if(IRoper) {
            opCode = bitToInt(currInstruction, 26, 31);

            if(opCode >= 0 && opCode <= 15) {
                registerDestination = bitToInt(currInstruction, 21, 25);
                registerSource1 = bitToInt(currInstruction, 16, 20);
                registerSource2 = bitToInt(currInstruction, 11, 15);

                if(loadDec) {
                    loadDec = false;
                    if(destinationRegisterEX == registerSource1 || destinationRegisterEX == registerSource2) {
                        stallInstructionIndex.push_back(this->registers[0]);
                        totalStalls++;
                        continue;
                    }
                }

                this->registers[0]++;
            }

            else if(opCode >= 16 && opCode <= 31) {
                registerDestination = bitToInt(currInstruction, 21, 25);
                registerSource1 = bitToInt(currInstruction, 16, 20);
                immediate = bitToInt(currInstruction, 0, 15);

                if(opCode >= 17 && opCode <= 17) {
                    if(loadDec) {
                        loadDec = false;
                        if(destinationRegisterEX == registerSource1) {
                            stallInstructionIndex.push_back(this->registers[0]);
                            totalStalls++;
                            continue;
                        }
                    }
                    this->registers[0]++;
                }
                else {
                    if(loadDec) {
                        loadDec = false;
                        if(destinationRegisterEX == registerSource1 || destinationRegisterEX == registerDestination) {
                            stallInstructionIndex.push_back(this->registers[0]);
                            totalStalls++;
                            continue;
                        }
                    }
                    branchDec = true;
                }
            }

            else if(opCode >= 32 && opCode <= 47) {
                immediate = bitToInt(currInstruction, 0, 15);
                branchDec = true;
            }

            else if(opCode >= 48 && opCode <= 63) {
                registerDestination = bitToInt(currInstruction, 21, 25);
                registerSource1 = bitToInt(currInstruction, 16, 20);
                registerSource2 = bitToInt(currInstruction, 11, 15);

                if(loadDec) {
                    loadDec = false;
                    if(opCode == 49) {
                        if(destinationRegisterEX == registerSource2) {
                            totalStalls++;
                            continue;
                        }
                    }
                    else if(opCode == 50) {
                        if(destinationRegisterEX == registerSource2 || destinationRegisterEX == registerDestination) {
                            totalStalls++;
                            continue;
                        }
                    }
                }

                this->registers[0]++;
            }

            EXoper = true;
            IRoper = false;
        }

        ////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////
        // IF Stage
        ////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////
        if(IFoper) {
            if(this->registers[0] < size) {
                currInstruction = IF(encodedIns, this->registers[0]);
                IRoper = true;
                if(branchDec) {
                    totalStalls++;
                    IRoper = false;
                    branchDec = false;
                    continue;
                }
            }
            else {
                IFoper = false;
            }
        }
    }
}

struct bitIns Processor::IF(vector<struct bitIns> encodedIns, int index) {
    return encodedIns[index];
}

//////////////////////////////////////////////////////
//  PRINT
//////////////////////////////////////////////////////

void Processor::cleanStallInstructions() {
    int i = 1;
    while(i < this->stallInstructionIndex.size()) {
        int j = 0;
        while(j < i) {
            if(this->stallInstructionIndex[j] == this->stallInstructionIndex[i])
                break;
            j++;
        }
        if(j < i) {
            vector<int>::iterator it;
            it = this->stallInstructionIndex.begin() + i;
            this->stallInstructionIndex.erase(it);
            i--;
        }
        i++;
    }
}

void Processor::print_information() {
    cleanStallInstructions();
    cout << "Total Number of stalls: " << this->totalStalls << endl;
    cout << "Number of clock cycles required: " << this->my_clock << endl;
    cout << "Total number of instructions: " << this->totalInstructions << endl;
    cout << "Average instruction per cycle (IPC): " << ((float) this->totalInstructions / (float) this->my_clock) << endl << endl;
}

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
    // cout << this->memory.ram.ramSize << endl;
    while(i < this->memory.ram.ramSize) {
        cout << "A" << i << " = " << this->memory.ram.rdata[i] << "\t\t\t";
        i++;
        cout << "A" << i << " = " << this->memory.ram.rdata[i] << "\t\t\t";
        i++;
        cout << "A" << i << " = " << this->memory.ram.rdata[i] << "\t\t\t";
        i++;
        cout << "A" << i << " = " << this->memory.ram.rdata[i];
        i++;
        cout << endl;
    }
}