#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

int stringToInt(char a[10]) {
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

int main() {
    // ifstream obj;
    // obj.open("./../sample1.asm", ios::in);

    // obj.seekg(0, ios::end);
    // long filesize = obj.tellg();
    // obj.seekg(0, ios::beg);
    // cout << filesize << endl;
    // char *arr = new char[filesize];
    // obj.read(arr, filesize);
    // obj.close();
    // cout << arr << endl;
    // // for(int i = 0; arr[i] != '\0'; i++) {
    // //     cout << "->" << arr[i] << endl;
    // // }
    // // cout << "------------------------" << endl;
    
    char a[10] = "A0";
    int n = stringToInt(a);
    cout << n;

    return 0;
}