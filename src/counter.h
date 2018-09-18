#ifndef COUNTER_H

#include <list>
#include <string>
#include <sstream>
#include <iterator>
#include <algorithm>

using namespace std;

class Counter : public list<string> {
private:
    size_t _count;

public:
    Counter(void) : _count(0) {
    }
    size_t count(void) const {
        return _count;
    }
    const string payload (void) const {
        ostringstream os;
        copy(begin(), end(), ostream_iterator<string>(os, " "));
        return os.str();
    }
    void inc (const string &payload = "") {
        _count++;
        if (payload.size()) {
            push_back(payload);
        }
    }
    Counter &operator+=(const Counter &c) {
        _count += c.count();
        copy(c.begin(), c.end(), back_inserter(*this));
        return *this;
    }
};

#endif // COUNTER_H
