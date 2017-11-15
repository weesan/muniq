#ifndef MUNIQ_H
#define MUNIQ_H

#include <string>
#include <unordered_map>
#include "thread/src/thread.h"

using namespace std;

class Muniq : public ThreadPool {
private:
    unordered_map<string, size_t> _freq;
    Mutex _mutex_freq;
    
public:
    Muniq(int parallel = 0) :
        ThreadPool(parallel) {
    }
    void incFreq(const string &line) {
        _mutex_freq.lock();
        _freq[line]++;
        _mutex_freq.unlock();
    }
    void process(const string &filename);
    void process(istream &is);
    void output(bool display_count);
};

#endif // MUNIQ_H
