#include <iostream>
#include <bitset>
#include "./../../Extras/functions.h"
#include "./encoder.h"
#include "./../LexicalAnalyser/lexical_analyser.h"
using namespace std;

Encoder::Encoder(vector<struct instruction> instructions) {
    this->encodedInstructions = encodeAll(instructions);
}

struct bitIns Encoder::encode(struct instruction ins) {
    struct bitIns bin;
    //  //////////////////////////////////////////////////////////////
    //  J - TYPE
    //  //////////////////////////////////////////////////////////////
    if(ins.type == 'J') {
        if(ins.lexeme[0].lexes[0] == 'J' && ins.lexeme[0].lexes[1] == 'U' && ins.lexeme[0].lexes[2] == 'M' && ins.lexeme[0].lexes[3] == 'P') {
            int i = 0;
            int d = 1;
            int n = strToInt(ins.lexeme[1].lexes);
            while(i < 16) {
                bin.word[i] = n & d;
                d = d << 1;
                i++;
            }

            bin.word[26] = 1;
            bin.word[31] = 1;
        }
    }

    //  //////////////////////////////////////////////////////////////
    //  I - TYPE
    //  //////////////////////////////////////////////////////////////
    else if(ins.type == 'I') {
        int i = 0;
        int d = 1;
        int n = strToInt(ins.lexeme[1].lexes);
        while(i < 5) {
            bin.word[21 + i] = n & d;
            d = d << 1;
            i++;
        }
        i = 0;
        d = 1;
        n = strToInt(ins.lexeme[2].lexes);
        while(i < 5) {
            bin.word[16 + i] = n & d;
            d = d << 1;
            i++;
        }
        i = 0;
        d = 1;
        n = strToInt(ins.lexeme[3].lexes);
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
        else if(ins.lexeme[0].lexes[0] == 'B' && ins.lexeme[0].lexes[1] == 'E' && ins.lexeme[0].lexes[2] == 'Q') {
            bin.word[27] = 1;
            bin.word[26] = 1;
        }
        else if(ins.lexeme[0].lexes[0] == 'B' && ins.lexeme[0].lexes[1] == 'G' && ins.lexeme[0].lexes[2] == 'T') {
            bin.word[28] = 1;
        }
        else if(ins.lexeme[0].lexes[0] == 'B' && ins.lexeme[0].lexes[1] == 'G' && ins.lexeme[0].lexes[2] == 'E') {
            bin.word[28] = 1;
            bin.word[26] = 1;
        }
    }

    //  //////////////////////////////////////////////////////////////
    //  R - TYPE
    //  //////////////////////////////////////////////////////////////
    else if(ins.type == 'R') {
        int i = 0;
        int d = 1;
        int n = strToInt(ins.lexeme[1].lexes);
        while(i < 5) {
            bin.word[21 + i] = n & d;
            d = d << 1;
            i++;
        }
        i = 0;
        d = 1;
        n = strToInt(ins.lexeme[2].lexes);
        while(i < 5) {
            bin.word[16 + i] = n & d;
            d = d << 1;
            i++;
        }
        i = 0;
        d = 1;
        n = strToInt(ins.lexeme[3].lexes);
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

    //  //////////////////////////////////////////////////////////////
    //  L - TYPE
    //  //////////////////////////////////////////////////////////////
    else if(ins.type == 'L') {
        int i = 0;
        int d = 1;
        int n = strToInt(ins.lexeme[1].lexes);
        while(i < 5) {
            bin.word[21 + i] = n & d;
            d = d << 1;
            i++;
        }
        i = 0;
        d = 1;
        n = strToInt(ins.lexeme[2].lexes);
        while(i < 5) {
            bin.word[16 + i] = n & d;
            d = d << 1;
            i++;
        }
        i = 0;
        d = 1;
        n = strToInt(ins.lexeme[3].lexes);
        while(i < 5) {
            bin.word[11 + i] = n & d;
            d = d << 1;
            i++;
        }
        bin.word[31] = 1;
        bin.word[30] = 1;
        if(ins.lexeme[0].lexes[0] == 'L' && ins.lexeme[0].lexes[1] == 'D') {
            bin.word[26] = 1;
        }
        else if(ins.lexeme[0].lexes[0] == 'S' && ins.lexeme[0].lexes[1] == 'T') {       
            bin.word[27] = 1;
        }

    }

    return bin;
}

vector<struct bitIns> Encoder::encodeAll(vector<struct instruction> instructions) {
    vector<struct bitIns> encodedInstructions;
    int size = instructions.size();

    for(int i = 0; i < size; i++) {
        struct bitIns bin = encode(instructions[i]);
        encodedInstructions.push_back(bin);
    }

    return encodedInstructions;
}

void Encoder::printEncodeInstructions(vector<struct bitIns> bins) {
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