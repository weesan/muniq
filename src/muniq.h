#ifndef MUNIQ_H
#define MUNIQ_H

#include <string>
#include <vector>
#include "file.h"

using namespace std;

class Muniq : public vector<File> {
public:
    void process(const string &filename);
    void process(istream &in);
    void output(bool display_count);
};

#endif // MUNIQ_H
