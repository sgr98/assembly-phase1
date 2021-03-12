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