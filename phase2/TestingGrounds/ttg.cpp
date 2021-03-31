#include<iostream>
using namespace std;

int main(int argc, char** argv) {
    cout << "You have entered " << argc
         << " arguments:" << "\n";

        cout << "--------------------------------\n";
        cout << argv[0] << "\n";
        cout << "--------------------------------\n";
        cout << argv[1] << "\n";
        cout << "--------------------------------\n";
  
    return 0;
}