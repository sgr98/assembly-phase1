#include <iostream>
#include <vector>
#include "./lexical_analyser.h"

bool isVisibleCharacter(char c) {
    if( c > (char) 32)
        return true;
    return false;
}

LexicalAnalyser::LexicalAnalyser(char a[1000]) {
    this->instructions = getInstructions(a);
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

void LexicalAnalyser::printInstructions(vector<struct instruction>) {
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