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

bool isVisibleCharacter(char c);
void intToString(char *s, int val);

class LexicalAnalyser {
    public:
        char Initialise[500];
        char Program[1000];
        vector<struct instruction> memSetups;
        vector<struct instruction> instructions;

        LexicalAnalyser(char a[1000]);
        void extract(char arr[1500]);
        vector<struct instruction> getInstructions(char arr[1000]);
        void assignTypes();
        void printInstructions(vector<struct instruction> ins);

        void modifyInstructions();
        void eraseInstruction(int index);
        void replaceLabel(int insIndex, int value);
};

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

void intToString(char s[10], int val) {
    int j = 0;
    if(val == 0) {
        s[0] = '0';
        s[1] = '\0';
        j++;
    }
    else if(val > 0) {
        while(val > 0) {
            int dig = val % 10;
            char c = (char) (dig + 48);
            s[j] = c;
            j++;
            val /= 10;
        }
        s[j] = '\0';
    }
    else {
        val *= -1;
        while(val > 0) {
            int dig = val % 10;
            char c = (char) (dig + 48);
            s[j] = c;
            j++;
            val /= 10;
        }
        s[j] = '-';
        j++;
        s[j] = '\0';
    }

    for(int i = 0; i < j / 2; i++) {
        char temp = s[i];
        s[i] = s[j - i - 1];
        s[j - i - 1] = temp;
    }
}

LexicalAnalyser::LexicalAnalyser(char a[1500]) {
    extract(a);
    this->memSetups = getInstructions(this->Initialise);
    this->instructions = getInstructions(this->Program);
    assignTypes();
    printInstructions(this->instructions);
    modifyInstructions();
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
        cout << i << ")\t";
        for(int j = 0; j < n; j++) {
            cout << ins[i].lexeme[j].lexes;
            cout << "\t";
        }
        cout << endl;
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
void LexicalAnalyser::modifyInstructions() {
    vector<int> labelIndex;
    vector<int> ijIndex;
    int size = this->instructions.size();
    for(int i = 0; i < size; i++) {
        if(this->instructions[i].size == 1) {
            labelIndex.push_back(i);
        }
        else if(this->instructions[i].type == 'J'
        || (this->instructions[i].type == 'I' 
        && !(this->instructions[i].lexeme[0].lexes[0] == 'A' && this->instructions[i].lexeme[0].lexes[1] == 'D' && this->instructions[i].lexeme[0].lexes[2] == 'D' && this->instructions[i].lexeme[0].lexes[3] == 'I')) ) {
            ijIndex.push_back(i);
        }
    }

    size = labelIndex.size();

    for(int i = 0; i < size; i++) {
        char label[10];
        int j = 0;
        for(j = 0; this->instructions[labelIndex[i]].lexeme[0].lexes[j] != '\0'; j++) {
            label[j] = this->instructions[labelIndex[i]].lexeme[0].lexes[j];
        }
        label[j - 1] = '\0';

        int loc = 0;
        for(j = 0; j < size; j++) {
            if(this->instructions[ijIndex[j]].size == 2) {
                if(compare(label, this->instructions[ijIndex[j]].lexeme[1].lexes)) {
                    loc = j;
                    break;
                }
            }
            else if(this->instructions[ijIndex[j]].size == 4) {
                if(compare(label, this->instructions[ijIndex[j]].lexeme[3].lexes)) {
                    loc = j;
                    break;
                }
            }
        }

        j = 0;
        while(j < size && labelIndex[j] < ijIndex[loc]) {
            j++;
        }

        int value = labelIndex[i] - ijIndex[loc] + (j - i);
        replaceLabel(ijIndex[loc], value);
    }

    // for(int i = 0; i < size; i++) {
    //     cout << labelIndex[i] << " ";
    // }
    // cout << endl;
    // for(int i = 0; i < size; i++) {
    //     cout << ijIndex[i] << " ";
    // }
    // cout << endl;

    for(int i = size - 1; i >= 0; i--) {
        eraseInstruction(labelIndex[i]);
    }
}

void LexicalAnalyser::eraseInstruction(int index) {
    vector<struct instruction>::iterator it;
    it = this->instructions.begin() + index;
    this->instructions.erase(it);
}

void LexicalAnalyser::replaceLabel(int insIndex, int value) {
    if(this->instructions[insIndex].lexeme[0].lexes[0] == 'A' && this->instructions[insIndex].lexeme[0].lexes[1] == 'D' && this->instructions[insIndex].lexeme[0].lexes[2] == 'D' && this->instructions[insIndex].lexeme[0].lexes[3] == 'I'
    || this->instructions[insIndex].lexeme[0].lexes[0] == 'B' && this->instructions[insIndex].lexeme[0].lexes[1] == 'N' && this->instructions[insIndex].lexeme[0].lexes[2] == 'E'
    || this->instructions[insIndex].lexeme[0].lexes[0] == 'B' && this->instructions[insIndex].lexeme[0].lexes[1] == 'E' && this->instructions[insIndex].lexeme[0].lexes[2] == 'Q'
    || this->instructions[insIndex].lexeme[0].lexes[0] == 'B' && this->instructions[insIndex].lexeme[0].lexes[1] == 'G' && this->instructions[insIndex].lexeme[0].lexes[2] == 'T'
    || this->instructions[insIndex].lexeme[0].lexes[0] == 'B' && this->instructions[insIndex].lexeme[0].lexes[1] == 'G' && this->instructions[insIndex].lexeme[0].lexes[2] == 'E') {
        intToString(this->instructions[insIndex].lexeme[3].lexes, value);
    }
    else if(this->instructions[insIndex].lexeme[0].lexes[0] == 'J' && this->instructions[insIndex].lexeme[0].lexes[1] == 'U' && this->instructions[insIndex].lexeme[0].lexes[2] == 'M' && this->instructions[insIndex].lexeme[0].lexes[3] == 'P') {
        intToString(this->instructions[insIndex].lexeme[1].lexes, value);
    }
}

int main() {
    char *arr = new char[1500];
    ifstream obj;
    obj.open("./../t.asm", ios::in);         // Enter filename
    obj.getline(arr, 100);
    int i = 0;
    for(i = 0; arr[i] != '\0'; i++);
    arr[i] = '\n';
    obj.read((arr + i + 1), 900);
    obj.close();

    
    ////////////////////////////////////////////////////////////////////////////
    // TURNING IT INTO LEXICAL UNITS
    ////////////////////////////////////////////////////////////////////////////
    LexicalAnalyser LexAnl(arr);
    cout << "INSTRUCTIONS:" << endl;

    // LexAnl.eraseInstruction(5);
    // LexAnl.replaceLabel(4, 1);
    // LexAnl.replaceLabel(8, 1);

    LexAnl.printInstructions(LexAnl.instructions);
    cout << "The number of instructions: " << LexAnl.instructions.size() << endl << endl;
    delete[] arr;

    return 0;
}