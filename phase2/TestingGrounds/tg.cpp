// Testing Grounds
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <bitset>
using namespace std;

struct lex {
    char lexes[10];
};

struct instruction {
    struct lex lexeme[4];
    char type;   // Currently R = R-TYPE and I = I-TYPE
    int size = 0;
};

struct bitIns {
    bitset<32> word; 
};

bool isVisibleCharacter(char c) {
    if( c > (char) 32)
        return true;
    return false;
}

vector<struct instruction> getInstructions(char arr[1000]) {
    vector<struct instruction> instructions;
    char temp[10];
    int i = 0;
    
    while(arr[i] != '\0') {
        struct instruction instruction;
        int j = 0;
        int n = 0;
        int k = 0;
        while(arr[i] != '\n') {
            if(isVisibleCharacter(arr[i]) && arr[i] != ',') {
                instruction.lexeme[j].lexes[k] = arr[i];
                k++;
                n = j;
            }
            else {
                if(k != 0) {
                    instruction.lexeme[j].lexes[k] = '\0';
                    j++;
                }
                k = 0;
            }
            i++;
        }
        instruction.lexeme[n].lexes[k] = '\0';
        instruction.size = n + 1;
        instructions.push_back(instruction);
        i++;
    }

    return instructions;
}

void printInstructions(vector<struct instruction> instructions) {
    int size = instructions.size();
    for(int i = 0; i < size; i++) {
        int n = instructions[i].size;
        for(int j = 0; j < n; j++) {
            cout << instructions[i].lexeme[j].lexes;
            cout << "\t";
        }
        cout << endl;
    }
}




int stringToInt(char a[10]) {
    int n = 0;
    if(a[0] == '-') {
        for(int i = 1; a[i] != '\0'; i++) {
            n *= 10;
            int d = ((int) a[i]) - 48;
            n += d;
        }
        n *= -1;
    }
    else if( !((a[0] >= (char) 48) && (a[0] <= (char) 57)) ) {
        for(int i = 1; a[i] != '\0'; i++) {
            n *= 10;
            int d = ((int) a[i]) - 48;
            n += d;
        }
    }
    else {
        for(int i = 0; a[i] != '\0'; i++) {
            n *= 10;
            int d = ((int) a[i]) - 48;
            // cout << d << endl;
            n += d;
        }
    }
    return n;
}

struct bitIns encode(struct instruction ins) {
    struct bitIns bin;
    
    int size = ins.size;
    //  SIZE = 2 (1 ARGS)
    if(size == 2) {
        int i = 0;
        int d = 1;
        int n = stringToInt(ins.lexeme[1].lexes);
        while(i < 16) {
            bin.word[i] = n & d;
            d = d << 1;
            i++;
        }

        bin.word[26] = 1;
        bin.word[31] = 1;
    }
    //  SIZE = 3 (2 ARGS)
    else if(size == 3) {
        int i = 0;
        int d = 1;
        int n = stringToInt(ins.lexeme[1].lexes);
        while(i < 5) {
            bin.word[21 + i] = n & d;
            d = d << 1;
            i++;
        }
        i = 0;
        d = 1;
        n = stringToInt(ins.lexeme[2].lexes);
        while(i < 5) {
            bin.word[16 + i] = n & d;
            d = d << 1;
            i++;
        }

        bin.word[30] = 1;
        bin.word[31] = 1;
        if(ins.lexeme[0].lexes[0] == 'L' && ins.lexeme[0].lexes[1] == 'D') {
            bin.word[26] = 1;
        }
        else if(ins.lexeme[0].lexes[0] == 'S' && ins.lexeme[0].lexes[1] == 'T') {
            bin.word[27] = 1;
        }
    }
    //  SIZE = 4 (3 ARGS)
    else if(size == 4) {
        int i = 0;
        int d = 1;
        int n = stringToInt(ins.lexeme[1].lexes);
        while(i < 5) {
            bin.word[21 + i] = n & d;
            d = d << 1;
            i++;
        }
        i = 0;
        d = 1;
        n = stringToInt(ins.lexeme[2].lexes);
        while(i < 5) {
            bin.word[16 + i] = n & d;
            d = d << 1;
            i++;
        }

        if(ins.lexeme[0].lexes[0] == 'A' && ins.lexeme[0].lexes[1] == 'D' && ins.lexeme[0].lexes[2] == 'D' && ins.lexeme[0].lexes[3] == 'I'
        || ins.lexeme[0].lexes[0] == 'B' && ins.lexeme[0].lexes[1] == 'N' && ins.lexeme[0].lexes[2] == 'E') {
            i = 0;
            d = 1;
            n = stringToInt(ins.lexeme[3].lexes);
            while(i < 16) {
                bin.word[i] = n & d;
                d = d << 1;
                i++;
            }
            bin.word[30] = 1;
            if(ins.lexeme[0].lexes[0] == 'A' && ins.lexeme[0].lexes[1] == 'D' && ins.lexeme[0].lexes[2] == 'D' && ins.lexeme[0].lexes[3] == 'I') {
                bin.word[26] = 1;
            }
            else if(ins.lexeme[0].lexes[0] == 'B' && ins.lexeme[0].lexes[1] == 'N' && ins.lexeme[0].lexes[2] == 'E') {
                bin.word[27] = 1;
            }

        }
        else {
            i = 0;
            d = 1;
            n = stringToInt(ins.lexeme[3].lexes);
            while(i < 5) {
                bin.word[11 + i] = n & d;
                d = d << 1;
                i++;
            }
            if(ins.lexeme[0].lexes[0] == 'A' && ins.lexeme[0].lexes[1] == 'D' && ins.lexeme[0].lexes[2] == 'D') {
                bin.word[26] = 1;
            }
            else if(ins.lexeme[0].lexes[0] == 'S' && ins.lexeme[0].lexes[1] == 'U' && ins.lexeme[0].lexes[2] == 'B') {
                bin.word[27] = 1;
            }
        }
    }

    return bin;
}

vector<struct bitIns> encodeAll(vector<struct instruction> instructions) {
    vector<struct bitIns> encodedInstructions;
    int size = instructions.size();

    for(int i = 0; i < size; i++) {
        struct bitIns bin = encode(instructions[i]);
        encodedInstructions.push_back(bin);
    }

    return encodedInstructions;
}

void printEncodeInstructions(vector<struct bitIns> bins) {
    int size = bins.size();
    for(int i = 0; i < size; i++) {
        // cout << bins[i].word << endl;
        for(int j = 31; j >= 0; j--) {
            if(j == 25 || j == 20 || j == 15)
                cout << " ";
            cout << bins[i].word[j];
        }
        cout << endl;
    }
}

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

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
    if(!(j == 31 && i == 26) && in.word[j] == 1) {
        n = n - m;
    }
    return n;
}

void add(int registers[32], int dr, int sr1, int sr2) {
    registers[dr] =  registers[sr1] + registers[sr2];
}

void sub(int registers[32], int dr, int sr1, int sr2) {
    registers[dr] =  registers[sr1] - registers[sr2];
}

void addi(int registers[32], int aa, int bb, int immediate) {
    registers[aa] =  registers[bb] + immediate;
}

void bne(int registers[32], int aa, int bb, int immediate) {
    if(registers[aa] != registers[bb]) {
        registers[0] += immediate;
    }
    else {
        registers[0]++;
    }
}

void beq(int registers[32], int aa, int bb, int immediate) {
    if(registers[aa] == registers[bb]) {
        registers[0] += immediate;
    }
    else {
        registers[0]++;
    }
}

void jump(int registers[32], int immediate) {
    registers[0] += immediate;
}

void ld(int dr, int Ain, int inc) {
    int index = Ain + inc;
    // registers[dr] = memory[index];
}

void st(int dr, int Ain, int inc) {
    int index = Ain + inc;
    // memory[index] = registers[dr];
}

void execute(struct bitIns inst, int registers[32]) {
    int op = bitToInt(inst, 26, 31);
    //  R-TYPE
    if(inst.word[31] == 0 && inst.word[30] == 0) {
        int dr = bitToInt(inst, 21, 25);
        int sr1 = bitToInt(inst, 16, 20);
        int sr2 = bitToInt(inst, 11, 15);

        if(op == 1) {
            add(registers, dr, sr1, sr2);
            registers[0]++;
        }
        else if(op == 2) {
            sub(registers, dr, sr1, sr2);
            registers[0]++;
        }
    }
    //  I-TYPE
    else if(inst.word[31] == 0 && inst.word[30] == 1) {
        int aa = bitToInt(inst, 21, 25);
        int bb = bitToInt(inst, 16, 20);
        int immediate = bitToInt(inst, 0, 15);

        if(op == 17) {
            addi(registers, aa, bb, immediate);
            registers[0]++;
        }
        else if(op == 18) {
            bne(registers, aa, bb, immediate);
        }
        else if(op == 19) {
            beq(registers, aa, bb, immediate);
        }
    }
    //  J-TYPE
    else if(inst.word[31] == 1 && inst.word[30] == 0) {
        int immediate = bitToInt(inst, 0, 15);
        if(op == 33) {
            jump(registers, immediate);
        }
    }
    //  L-TYPE
    else if(inst.word[31] == 1 && inst.word[30] == 1) {
        int dr = bitToInt(inst, 21, 25);
        int Ain = bitToInt(inst, 16, 20);
        int inc = bitToInt(inst, 11, 15);

        if(op == 48) {
            ld(dr, Ain, inc);
            registers[0]++;
        }
        else if(op == 49) {
            st(dr, Ain, inc);
            registers[0]++;
        }
    }
}

void executeAll(vector<struct bitIns> encodedIns, int registers[32]) {
    int size = encodedIns.size();
    while(registers[0] < size) {
        // cout << "EXECUTE => " << registers[0] << endl;
        execute(encodedIns[registers[0]], registers);
    }
}

void printRegisters(int registers[32]) {
    for(int i = 0; i < 32; i++) {
        cout << "R" << i << " = " << registers[i] << endl;
    }
}


int main() {
    // char arr[1000] = "ADDI R1, R2, 2\nADD R4, R5, R6\nSUB R7, R8, R9\nBNE R10, R11, 4\nJUMP 6\nLD R12, A1, R13\nST R14, A2, R15\n";
    char arr[1000] = "ADDI R1, R2, 2\nADD R4, R5, R6\nSUB R7, R8, R9\nBNE R10, R11, -1\nJUMP 1\n";
    // cout << arr;

    vector<struct instruction> instructions;
    instructions = getInstructions(arr);
    printInstructions(instructions);

    vector<struct bitIns> encodedInstructions = encodeAll(instructions);
    printEncodeInstructions(encodedInstructions);

    int registers[32] = {0};
    int memory[100] = {0};
    // int memory[100];

    registers[0] = 0;
    registers[1] = 5;
    registers[2] = 6;
    registers[4] = 7;
    registers[5] = 8;
    registers[6] = 9;
    registers[7] = 10;
    registers[8] = 11;
    registers[9] = 12;
    registers[10] = 13;
    registers[11] = 13;
    cout << encodedInstructions.size() << endl;
    executeAll(encodedInstructions, registers);
    printRegisters(registers);

    return 0;
}