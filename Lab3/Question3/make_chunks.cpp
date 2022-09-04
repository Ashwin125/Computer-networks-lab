#include <iostream>
#include <fstream>
#include <sstream>
#include <array>

using namespace std;

/**
 * open file 'large_file.txt'
 * and divide the file into total 10 parts
 * and return those 10 different parts
 */
array<string, 10> load_and_divide_file() {
    // load the file into variable and divide it into 10 parts
    
    ifstream fin("large_file.txt");
    array<string, 10> chunks;

    if (!fin) {
        cout << "Error opening the file.\n" << endl;
        exit(1);
    }
    
    // get the size of the file
    // go to the end
    fin.seekg(0, ios::end);
    
    // get the size
    int size = fin.tellg();
    
    // go back to beginning
    fin.seekg(0, ios::beg);

    // store file in array of size 10
    int chunksize = size / 10;

    for (int i = 0; i < 9; ++i) {
        // get those chunks
        char buffer[chunksize + 1];
        fin.read(buffer, chunksize);

        chunks[i] = string(buffer, chunksize);
    }

    // get the last chunk
    int rest = size - 9 * chunksize;
    char buffer[rest + 1];
    fin.read(buffer, rest);

    chunks[9] = string(buffer, rest);

    fin.close();
    return chunks;
}


