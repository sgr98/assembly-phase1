#include <iostream>
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
        void printInstructions();

        void modifyInstructions();
        void eraseInstruction(int index);
        void replaceLabel(int insIndex, int value);
        void printStalledInstructions(vector<int> stallIndices);
};