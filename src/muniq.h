#ifndef MUNIQ_H
#define MUNIQ_H

#include <string>
#include <unordered_map>
#include "thread/src/thread.h"
#include "freq_table.h"

using namespace std;

class Muniq : public ThreadPool {
private:
    FreqTable _freq;
    FreqTables _freqs;
    
public:
    Muniq(int parallel);
    void incFreq(const string &key) {
        _freqs.incFreq(key);
    }
    void process(const string &filename);
    void process(istream &is);
    void aggregate(void);
    void output(bool display_count, bool display_count_after);
};

#endif // MUNIQ_H
