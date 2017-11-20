#include <iostream>
#include "file.h"
#include "muniq.h"

void File::process (const string &filename)
{
    ifstream ifs(filename);
    if (!ifs) { 
        cerr << "Failed to open " << filename << endl;
        return;
    } else {
        process(ifs);
    }
}

void File::process (istream &is)
{
    string line;

    while (getline(is, line)) {
        if (line.empty()) {
            continue;
        }
        (*this)[line]++;
    }
}
