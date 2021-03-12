#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <bitset>
using namespace std;

char pmain[] = "&$MAIN:";    //&$MAIN:
char pend[] = "&$END:";     //&$END:

bool compare(char *arr, char *brr) {
    int i = 0;
    while(brr[i] != '\0' && brr[i] == *(arr + i)) {
        i++;
    }
    if(brr[i] == '\0')
        return true;
    return false;
}

void extract(char arr[1000], char a[500], char b[500]) {
    int i = 0;
    int j = 0;
    bool NOTencMain = true;
    while(NOTencMain && arr[i] != '\0') {
        NOTencMain = !compare(&arr[i], pmain);
        if(!NOTencMain) {
            break;
        }
        else {
            a[j] = arr[i];
            j++;
        }
        i++;
    }
    a[j] = '\0';

    i += 8;     // For the length of &$MAIN:\n
    j = 0;
    while(!NOTencMain && arr[i] != '\0') {
        if(compare(&arr[i], pend)) {
            break;
        }
        else {
            b[j] = arr[i];
            j++;
        }
        i++;
    }
    b[j] = '\0';  
}

int main() {
    char arr[1500];

    ifstream obj;
    obj.open("./../t.asm", ios::in);
    obj.getline(arr, 100);
    int i = 0;
    for(i = 0; arr[i] != '\0'; i++);
    arr[i] = '\n';
    obj.read((arr + i + 1), 900);
    obj.close();

    cout << arr << endl;
    
    char a[500];
    char b[1000];

    extract(arr, a, b);

    cout << "-----" << endl << a << "-----" << endl;
    cout << "-----" << endl << b << "-----" << endl;

    return 0;
}