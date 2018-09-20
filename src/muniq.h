#ifndef MUNIQ_H
#define MUNIQ_H

#include <string>
#include <unordered_map>
#include "thread.h"
#include "freq_table.h"

#define DEFAULT_NUM_OF_HASHES    101

using namespace std;

class Muniq : public ThreadPool {
private:
    FreqTable _freq;
    FreqTables _freqs;
    int _key;
    bool _display_payload;
    const string _delimiters;

private:
    string parse_delimiters (const string delimiters);
    
public:
    Muniq(int parallel = 0,
          int num_of_hashes = DEFAULT_NUM_OF_HASHES,
          int key = 0,
          const string &delimeter = "\t",
          bool display_payload = false,
          bool display_count = false,
          bool display_count_after = false);
    void incFreq(const string &key, const string &payload = "") {
        _freqs.incFreq(key, payload);
    }
    void process(const string &filename);
    void process(istream &is);
    void output(const string &output_dir);
};

#endif // MUNIQ_H
