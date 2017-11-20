#ifndef FREQ_TABLE_H
#define FREQ_TABLE_H

#include <string>
#include <vector>
#include <unordered_map>
#include "thread/src/thread.h"

using namespace std;

class FreqTable : public unordered_map<string, size_t> {
private:
    Mutex _mutex;
    
public:
    void incFreq(const string &key) {
        _mutex.lock();
        (*this)[key]++;
        _mutex.unlock();
    }
    void merge(const FreqTable &freq);
};

class FreqTables : public vector<FreqTable> {
private:
    uint32_t hash(const string &str) const;
    
public:
    void incFreq(const string &key);
};

#endif // FREQ_TABLE_H
