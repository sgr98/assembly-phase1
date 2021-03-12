#include <iostream>
#include <bitset>
#include "./encoder.h"
#include "./../LexicalAnalyser/lexical_analyser.h"
using namespace std;

Encoder::Encoder(vector<struct instruction> instructions) {
    this->encodedInstructions = encodeAll(instructions);
}

int Encoder::stringToInt(char a[10]) {
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

struct bitIns Encoder::encode(struct instruction ins) {
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
    // //  SIZE = 3 (2 ARGS)
    // else if(size == 3) {
    //     int i = 0;
    //     int d = 1;
    //     int n = stringToInt(ins.lexeme[1].lexes);
    //     while(i < 5) {
    //         bin.word[21 + i] = n & d;
    //         d = d << 1;
    //         i++;
    //     }
    //     i = 0;
    //     d = 1;
    //     n = stringToInt(ins.lexeme[2].lexes);
    //     while(i < 5) {
    //         bin.word[16 + i] = n & d;
    //         d = d << 1;
    //         i++;
    //     }

    //     bin.word[30] = 1;
    //     bin.word[31] = 1;
    //     if(ins.lexeme[0].lexes[0] == 'L' && ins.lexeme[0].lexes[1] == 'D') {
    //         bin.word[26] = 1;
    //     }
    //     else if(ins.lexeme[0].lexes[0] == 'S' && ins.lexeme[0].lexes[1] == 'T') {
    //         bin.word[27] = 1;
    //     }
    // }
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
        || ins.lexeme[0].lexes[0] == 'B' && ins.lexeme[0].lexes[1] == 'N' && ins.lexeme[0].lexes[2] == 'E'
        || ins.lexeme[0].lexes[0] == 'B' && ins.lexeme[0].lexes[1] == 'E' && ins.lexeme[0].lexes[2] == 'Q') {
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
            else if(ins.lexeme[0].lexes[0] == 'B' && ins.lexeme[0].lexes[1] == 'E' && ins.lexeme[0].lexes[2] == 'Q') {
                bin.word[27] = 1;
                bin.word[26] = 1;
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

            else if(ins.lexeme[0].lexes[0] == 'L' && ins.lexeme[0].lexes[1] == 'D') {
                bin.word[31] = 1;
                bin.word[30] = 1;
                bin.word[26] = 1;
            }
            else if(ins.lexeme[0].lexes[0] == 'S' && ins.lexeme[0].lexes[1] == 'T') {
                bin.word[31] = 1;
                bin.word[30] = 1;
                bin.word[27] = 1;
            }
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