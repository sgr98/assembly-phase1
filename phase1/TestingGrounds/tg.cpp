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

int main() {
    char arr[1000] = "ADDI R1, R2, 2\nADD R4, R5, R6\nSUB R7, R8, R9\nBNE R10, R11, 4\nJUMP 6\nLD R12, A1\nST R13, A2\n";
    // cout << arr;

    vector<struct instruction> instructions;
    instructions = getInstructions(arr);
    printInstructions(instructions);

    vector<struct bitIns> encodedInstructions = encodeAll(instructions);
    printEncodeInstructions(encodedInstructions);

    return 0;
}