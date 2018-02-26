#ifndef FREQ_TABLE_H
#define FREQ_TABLE_H

#include <string>
#include <vector>
#include <ostream>
#include <unordered_map>
#include "thread.h"

using namespace std;

class FreqTable : public unordered_map<string, size_t> {
private:
    Mutex _mutex;
    bool _display_count;
    bool _display_count_after;

public:
    FreqTable(bool display_count = false, bool display_count_after = false) :
        _display_count(display_count),
        _display_count_after(display_count_after) {
    }
    void incFreq(const string &key) {
        _mutex.lock();
        (*this)[key]++;
        _mutex.unlock();
    }
    void merge(const FreqTable &freq);
    ostream &print(ostream &os) const;
};

class FreqTables : public vector<FreqTable> {
private:
    uint32_t hash(const string &str) const;
    
public:
    void incFreq(const string &key);
};

extern ostream &operator<<(ostream &os, const FreqTable &freq_table);

#endif // FREQ_TABLE_H
