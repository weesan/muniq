#ifndef MUNIQ_H
#define MUNIQ_H

#include <string>
#include <unordered_map>
#include "thread.h"
#include "freq_table.h"

using namespace std;

class Muniq : public ThreadPool {
private:
    FreqTable _freq;
    FreqTables _freqs;
    int _key;
    const string _delimiters;

private:
    string parse_delimiters (const string delimiters);
    
public:
    Muniq(int parallel = 0,
          int key = 0,
          const string &delimeter = "\t",
          bool display_count = false,
          bool display_count_after = false);
    void incFreq(const string &key) {
        _freqs.incFreq(key);
    }
    void process(const string &filename);
    void process(istream &is);
    void output(const string &output_dir);
};

#endif // MUNIQ_H
