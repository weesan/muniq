#include <iostream>
#include <iomanip>
#include <fstream>
#include "muniq.h"

class CountTask : public Task {
private:
    Muniq &_muniq;
    string _filename;
    
public:
    CountTask(Muniq &muniq, const string  &filename) :
        _muniq(muniq),
        _filename(filename) {
    }
    void run(void) {
        ifstream ifs(_filename);
        if (!ifs) { 
            cerr << "Failed to open " << _filename << endl;
            return;
        } else {
            string line;

            while (getline(ifs, line)) {
                if (line.empty()) {
                    continue;
                }
                _muniq.incFreq(line);
            }
        }
    }
};

Muniq::Muniq(int parallel = 0) :
    ThreadPool(parallel) {
    for (int i = 0; i < 101; i++) {
        _freqs.push_back(FreqTable());
    }
}

// Multi-threaded version.
void Muniq::process (const string &filename)
{
    addTask(new CountTask(*this, filename));
}

// Single-threaded version reading from stdin.
void Muniq::process (istream &is)
{
    string line;

    while (getline(is, line)) {
        if (line.empty()) {
            continue;
        }
        _freq[line]++;
    }
}

void Muniq::aggregate(void) {
    // Signal the thread pool that we are done queuing the tasks and
    // wait for the tasks to finish.
    if (size()) {
        done();
        wait();
    }
}

void Muniq::output (bool display_count = false, bool display_count_after = false)
{
    // Single-threaded version.
    for (auto itr = _freq.begin(); itr != _freq.end(); ++itr) {
        if (display_count) {
            if (display_count_after) {
                cout << itr->first << '#' << itr->second << endl;
            } else {
                cout << setw(7) << itr->second << " " << itr->first << endl;
            }
        } else {
            cout << itr->first << endl;
        }
    }

    // Multi-threaded version.
    for (int i = 0; i < _freqs.size(); i++) {
        for (auto itr = _freqs[i].begin(); itr != _freqs[i].end(); ++itr) {
            if (display_count) {
                if (display_count_after) {
                    cout << itr->first << '#' << itr->second << endl;
                } else {
                    cout << setw(7) << itr->second << " " << itr->first << endl;
                }
            } else {
                cout << itr->first << endl;
            }
        }
    }
}
