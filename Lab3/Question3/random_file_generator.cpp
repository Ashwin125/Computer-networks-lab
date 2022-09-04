#include <iostream>
#include <fstream>
using namespace std;

char getRandomChar() {
    return char('a' + rand() % 26);
}

int main(void) {
    srand(time(NULL));
    ofstream fout;
    
    // size in bytes
    int size = 10'000'000;

    fout.open("large_file.txt");
    
    // write 10 Mega Bytes of data
    
    for (int i = 0; i < size; ++i) {
        fout << getRandomChar();
    }

    fout << endl;
    fout.close();
}
