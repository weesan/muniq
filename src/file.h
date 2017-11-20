#ifndef FILE_H
#define FILE_H

#include <string>
#include <fstream>
#include "freq_table.h"
#include "thread/src/thread.h"

using namespace std;

class File : public FreqTable {
public:
    void process(const string &filename);
    void process(istream &is);
};

class Files : public unordered_map<pthread_t, File> {
};

#endif // FILE_H
