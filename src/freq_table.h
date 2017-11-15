#ifndef FREQ_TABLE_H
#define FREQ_TABLE_H

#include <string>
#include <unordered_map>

using namespace std;

class FreqTable : public unordered_map<string, size_t> {
public:
    void merge(const FreqTable &freq);
};

#endif // FREQ_TABLE_H
