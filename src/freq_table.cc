#include "freq_table.h"

void FreqTable::merge (const FreqTable &freq)
{
    for (auto itr = freq.begin(); itr != freq.end(); ++itr) {
        (*this)[itr->first] += itr->second;
    }
}

// From FastText.
uint32_t FreqTables::hash(const string &str) const {
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
