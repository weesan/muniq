#ifndef COUNTER_H

#include <string>
#include <sstream>
#include <iterator>
#include <algorithm>

using namespace std;

class Counter {
private:
    size_t _count;
    string _payload;

public:
    Counter(void) : _count(0) {
    }
    size_t count(void) const {
        return _count;
    }
    const string &payload (void) const {
        return _payload;
    }
    void inc (const string &payload = "") {
        _count++;
        if (!payload.size()) {
            return;
        }           
        if (!_payload.size()) {
            _payload = payload;
        } else {
            _payload += string(" ") + payload;
        }
    }
    Counter &operator+=(const Counter &c) {
        _count += c.count();
        _payload += string(" ") + c.payload();
        return *this;
    }
};

#endif // COUNTER_H
