#ifndef MUNIQ_H
#define MUNIQ_H

#include <string>
#include <unordered_map>
#include "freq_table.h"
#include "thread/src/thread.h"

using namespace std;

class Muniq : public ThreadPool {
private:
    FreqTable _freq;
    unordered_map<pthread_t, FreqTable> _freqs;
    Mutex _mutex_freq;
    
public:
    Muniq(int parallel);
    FreqTable &freqs(pthread_t pid) {
        return _freqs[pid];
    }
    void incFreq(const string &line) {
        _mutex_freq.lock();
        _freq[line]++;
        _mutex_freq.unlock();
    }
    void process(const string &filename);
    void process(istream &is);
    void output(bool display_count, bool display_count_after);
};

#endif // MUNIQ_H
