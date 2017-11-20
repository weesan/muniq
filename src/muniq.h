#ifndef MUNIQ_H
#define MUNIQ_H

#include <string>
#include <unordered_map>
#include "thread/src/thread.h"
#include "freq_table.h"
#include "file.h"

using namespace std;

class Muniq : public ThreadPool {
private:
    FreqTable _freq;
    Mutex _freq_mutex;
    Files _files;
    Mutex _files_mutex;
    
public:
    Muniq(int parallel);
    File &files(pthread_t pid) {
        _files_mutex.lock();
        File &file = _files[pid];
        _files_mutex.unlock();
        return file;
    }
    void incFreq(const string &line) {
        _freq_mutex.lock();
        _freq[line]++;
        _freq_mutex.unlock();
    }
    void process(const string &filename);
    void process(istream &is);
    void aggregate(void);
    void output(bool display_count, bool display_count_after);
};

#endif // MUNIQ_H
