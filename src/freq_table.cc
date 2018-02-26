#include <iomanip>
#include "freq_table.h"

ostream &operator<<(ostream &os, const FreqTable &freq_table)
{
    return freq_table.print(os);
}

void FreqTable::merge (const FreqTable &freq)
{
    for (auto itr = freq.begin(); itr != freq.end(); ++itr) {
        (*this)[itr->first] += itr->second;
    }
}

ostream &FreqTable::print(ostream &os) const {
    for (auto itr = begin(); itr != end(); ++itr) {
        if (_display_count) {
            if (_display_count_after) {
                os << itr->first << '#' << itr->second << endl;
            } else {
                os << setw(7) << itr->second << " " << itr->first << endl;
            }
        } else {
            os << itr->first << endl;
        }
    }
    return os;
}

// Copied from
// https://github.com/facebookresearch/fastText/blob/master/src/dictionary.cc
uint32_t FreqTables::hash (const string &str) const {
    uint32_t h = 2166136261;
    for (size_t i = 0; i < str.size(); i++) {
        h = h ^ (uint32_t)str[i];
        h = h * 16777619;
    }
    return h;
}

void FreqTables::incFreq (const string &key)
{
    int slot = hash(key) % size();
    (*this)[slot].incFreq(key);
}
