#include <iostream>
#include <vector>
#include "./lexical_analyser.h"

bool isVisibleCharacter(char c) {
    if( c > (char) 32)
        return true;
    return false;
}

bool compare(char *arr, char *brr) {
    int i = 0;
    while(brr[i] != '\0' && brr[i] == *(arr + i)) {
        i++;
    }
    if(brr[i] == '\0')
        return true;
    return false;
}

LexicalAnalyser::LexicalAnalyser(char a[1500]) {
    extract(a);
    this->memSetups = getInstructions(this->Initialise);
    this->instructions = getInstructions(this->Program);
    assignTypes();
}

void LexicalAnalyser::extract(char arr[1500]) {
    char pmain[] = "&$MAIN:";
    char pend[] = "&$END:";
    int i = 0;
    int j = 0;
    bool NOTencMain = true;
    while(NOTencMain && arr[i] != '\0') {
        NOTencMain = !compare(&arr[i], pmain);
        if(!NOTencMain) {
            break;
        }
        else {
            this->Initialise[j] = arr[i];
            j++;
        }
        i++;
    }
    this->Initialise[j] = '\0';

    i += 8;     // For the length of &$MAIN:\n
    j = 0;
    while(!NOTencMain && arr[i] != '\0') {
        if(compare(&arr[i], pend)) {
            break;
        }
        else {
            this->Program[j] = arr[i];
            j++;
        }
        i++;
    }
    this->Program[j] = '\0';  
}

vector<struct instruction> LexicalAnalyser::getInstructions(char arr[1000]) {
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

void LexicalAnalyser::assignTypes() {
    // R-TYPES (3 args)     ////(0 - 15)
    // ADD   :   00 0001    (1)
    // SUB   :   00 0010    (2)

    // I-TYPES (3 args)     ////(16 - 31)
    // ADDI  :   01 0001    (17)
    // BNE   :   01 0010    (18)
    // BEQ   :   01 0011    (19)
    // BGT   :   01 0100    (20)
    // BGE   :   01 0101    (21)

    // J-TYPES (1 args)     ////(32 - 47)
    // JUMP  :   10 0001    (33)

    // L-TYPES (3 args)     ////(48 - 63)
    // LD    :   11 0001    (49)
    // ST    :   11 0010    (50)

    // cout << "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx" << endl;
    int size = this->instructions.size();
    for(int i = 0; i < size; i++) {
        if(this->instructions[i].lexeme[0].lexes[0] == 'A' && this->instructions[i].lexeme[0].lexes[1] == 'D' && this->instructions[i].lexeme[0].lexes[2] == 'D' && this->instructions[i].lexeme[0].lexes[3] == 'I'
        || this->instructions[i].lexeme[0].lexes[0] == 'B' && this->instructions[i].lexeme[0].lexes[1] == 'N' && this->instructions[i].lexeme[0].lexes[2] == 'E'
        || this->instructions[i].lexeme[0].lexes[0] == 'B' && this->instructions[i].lexeme[0].lexes[1] == 'E' && this->instructions[i].lexeme[0].lexes[2] == 'Q'
        || this->instructions[i].lexeme[0].lexes[0] == 'B' && this->instructions[i].lexeme[0].lexes[1] == 'G' && this->instructions[i].lexeme[0].lexes[2] == 'T'
        || this->instructions[i].lexeme[0].lexes[0] == 'B' && this->instructions[i].lexeme[0].lexes[1] == 'G' && this->instructions[i].lexeme[0].lexes[2] == 'E') {
            this->instructions[i].type = 'I';
        }
        else if(this->instructions[i].lexeme[0].lexes[0] == 'A' && this->instructions[i].lexeme[0].lexes[1] == 'D' && this->instructions[i].lexeme[0].lexes[2] == 'D'
        || this->instructions[i].lexeme[0].lexes[0] == 'S' && this->instructions[i].lexeme[0].lexes[1] == 'U' && this->instructions[i].lexeme[0].lexes[2] == 'B') {
            this->instructions[i].type = 'R';
        }
        else if(this->instructions[i].lexeme[0].lexes[0] == 'J' && this->instructions[i].lexeme[0].lexes[1] == 'U' && this->instructions[i].lexeme[0].lexes[2] == 'M' && this->instructions[i].lexeme[0].lexes[3] == 'P') {
            this->instructions[i].type = 'J';
        }
        else if(this->instructions[i].lexeme[0].lexes[0] == 'L' && this->instructions[i].lexeme[0].lexes[1] == 'D'
        || this->instructions[i].lexeme[0].lexes[0] == 'S' && this->instructions[i].lexeme[0].lexes[1] == 'T') {
            this->instructions[i].type = 'L';
        }
        // cout << this->instructions[i].type << endl;
    }
    // cout << "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx" << endl;

}

void LexicalAnalyser::printInstructions(vector<struct instruction> ins) {
    int size = ins.size();
    for(int i = 0; i < size; i++) {
        int n = ins[i].size;
        for(int j = 0; j < n; j++) {
            cout << ins[i].lexeme[j].lexes;
            cout << "\t";
        }
        cout << endl;
    }
}