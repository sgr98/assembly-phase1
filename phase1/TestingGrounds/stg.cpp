#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

int main() {
    ifstream obj;
    obj.open("./../sample1.asm", ios::in);

    obj.seekg(0, ios::end);
    long filesize = obj.tellg();
    obj.seekg(0, ios::beg);
    cout << filesize << endl;
    char *arr = new char[filesize];
    obj.read(arr, filesize);
    obj.close();
    cout << arr << endl;
    // for(int i = 0; arr[i] != '\0'; i++) {
    //     cout << "->" << arr[i] << endl;
    // }
    // cout << "------------------------" << endl;
    
    
    return 0;
}