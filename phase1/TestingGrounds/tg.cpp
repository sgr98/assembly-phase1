// Testing Grounds
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

struct lex {
    char lexes[10];
};

struct instruction {
    struct lex lexeme[4];
    char type;   // Currently R = R-TYPE and I = I-TYPE
    int size = 0;
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

int main() {
    char arr[1000] = "ADDI R1, R2, R3\nADD R4, R5, R6\nADD R7, R8\n";
    cout << arr;

    // char a[10] = "ADD";
    // char b[10] = "R1";
    // char c[10] = "R2";
    // char d[10] = "R3";
    vector<struct instruction> instructions;
    instructions = getInstructions(arr);
    // struct instruction ii;
    // for(int i = 0; i < 10; i++)
    //     ii.lexeme[0].lexes[i] = a[i];
    // for(int i = 0; i < 10; i++)
    //     ii.lexeme[1].lexes[i] = b[i];
    // for(int i = 0; i < 10; i++)
    //     ii.lexeme[2].lexes[i] = c[i];
    // for(int i = 0; i < 10; i++)
    //     ii.lexeme[3].lexes[i] = d[i];
    // ii.size = 4;
    // instructions.push_back(ii);

    // cout << endl << "---------------------" << endl;
    // cout << instructions.size() << endl;
    // cout << instructions[0].size << endl;
    // cout << instructions[0].lexeme[0].lexes << endl;
    // cout << instructions[0].lexeme[1].lexes << endl;
    // cout << instructions[0].lexeme[2].lexes << endl;
    // cout << instructions[0].lexeme[3].lexes << endl;
    printInstructions(instructions);

    return 0;
}