#include <iostream>
#include <iomanip>
#include <fstream>
#include "muniq.h"

#define MAX_QUEUE_SIZE    1000
#define MAX_BATCH_SIZE    1000

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
        _muniq.files(pthread_self()).process(_filename);
    }
};

Muniq::Muniq(int parallel = 0) :
    ThreadPool(parallel) {
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
    done();
    wait();

    // Aggregate the counts from each thread.
    for (auto itr = _files.begin(); itr != _files.end(); ++itr) {
        if (itr == _files.begin()) {
            // Simply copy over the first one.
            _freq = itr->second;
        } else {
            _freq.merge(itr->second);
        }
        // Clear the source count.
        itr->second.clear();
    }
}

void Muniq::output (bool display_count = false, bool display_count_after = false)
{
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
}
