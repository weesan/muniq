#ifndef FILE_H
#define FILE_H

#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include "thread/src/thread.h"

using namespace std;

class File : public ThreadPool {
private:
    string _name;
    unordered_map<string, size_t> _freq;
    Mutex _mutex_freq;
    int _parallel;
    size_t _total_byte;
    size_t _total_word;
    size_t _total_line;

private:
    void process(istream &is);
    
public:
    File(const string &name, int parallel = 0) :
        ThreadPool(parallel),
        _name(name),
        _parallel(parallel),
        _total_byte(0),
        _total_word(0),
        _total_line(0) {
        ifstream ifs(name);
        if (!ifs) { 
            cerr << "Failed to open " << name << endl;
            return;
        } else {
            process(ifs);
        }
    }
    File(istream &is, int parallel = 0) :
        ThreadPool(parallel),
        _name(""),
        _parallel(parallel),
        _total_byte(0),
        _total_word(0),
        _total_line(0) {
        process(is);
    }
    void incFreq(const string &line) {
        _mutex_freq.lock();
        _freq[line]++;
        _mutex_freq.unlock();
    }
    void output(bool uniq_count);
};

#endif // FILE_H
