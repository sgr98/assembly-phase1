#include<iostream>
#include <fstream>
#include <string>
using namespace std;

int main(int argc, char** argv) {

    char *arr = new char[1500];
    ifstream obj;
    obj.open(argv[1], ios::in);
    obj.getline(arr, 100);
    int i = 0;
    for(i = 0; arr[i] != '\0'; i++);
    arr[i] = '\n';
    obj.read((arr + i + 1), 900);
    obj.close();

    cout << arr;

    return 0;
}