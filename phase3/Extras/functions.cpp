#include <iostream>
#include <vector>
#include "./functions.h"
#include "./../Components/LexicalAnalyser/lexical_analyser.h"
#include "./../Components/Encoder/encoder.h"
#include "./../Components/Processor/processor.h"
using namespace std;


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

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

int strToInt(char a[10]) {
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
    if(!(j - i == 5 || j - i == 4) && in.word[j] == 1) {
        n = n - m;
    }
    return n;
}

