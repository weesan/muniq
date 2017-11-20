#include <iostream>
#include <iomanip>
#include <fstream>
#include "muniq.h"

#define MAX_QUEUE_SIZE    1000
#define MAX_BATCH_SIZE    1000

class CountTask : public Task {
private:
    Muniq &_muniq;
    vector<string> _lines;
    
public:
    CountTask(Muniq &muniq, const vector<string> &lines) :
        _muniq(muniq),
        _lines(lines) {
    }
    void run(void) {
        for (int i = 0; i < _lines.size(); i++) {
            //_muniq.incFreq(_lines[i]);
            _muniq.freqs(pthread_self())[_lines[i]]++;
        }
    }
};

Muniq::Muniq(int parallel = 0) :
    ThreadPool(parallel) {
    /*
    for (auto itr = begin(); itr != end(); ++itr) {
        //cerr << (*itr)->tid() << endl;
        _freqs[(*itr)->tid()] = FreqTable();
    }
    */
}

void Muniq::process (const string &filename)
{
    ifstream ifs(filename);
    if (!ifs) { 
        cerr << "Failed to open " << filename << endl;
        return;
    } else {
        process(ifs);
    }
}

void Muniq::process(istream &is)
{
    string line;
    vector<string> lines;

    while (getline(is, line)) {
        if (line.empty()) {
            continue;
        }
        //cout << line << endl;
        if (size()) {
            lines.push_back(line);
            
            if (lines.size() == MAX_BATCH_SIZE) {
                addTask(new CountTask(*this, lines));
                lines.clear();
            }
            
            while (queueSize() >= MAX_QUEUE_SIZE) {
                sleep(0.5);
            }

        } else {
            _freq[line]++;
        }
    }

    if (size()) {
        if (lines.size()) {
            addTask(new CountTask(*this, lines));
        }
    
        done();
        wait();
        
        // Combine the freq table per thread into the main thread.
        for (auto itr = _freqs.begin(); itr != _freqs.end(); ++itr) {
            _freq.merge(itr->second);
        }
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
