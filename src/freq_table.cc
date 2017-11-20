#include "freq_table.h"

void FreqTable::merge(const FreqTable &freq)
{
    for (auto itr = freq.begin(); itr != freq.end(); ++itr) {
        (*this)[itr->first] += itr->second;
    }
}
