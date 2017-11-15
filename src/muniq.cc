#include <iostream>
#include <iomanip>
#include <fstream>
#include "muniq.h"

#define MAX_QUEUE_SIZE    1000

class CountTask : public Task {
private:
    Muniq &_muniq;
    string _line;
    
public:
    CountTask(Muniq &muniq, const string &line) :
        _muniq(muniq),
        _line(line) {
    }
    void run(void) {
        _muniq.incFreq(_line);
    }
};

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

    while (getline(is, line)) {
        if (!line.size()) {
            continue;
        }
        //cout << line << endl;
        if (size()) {
            addTask(new CountTask(*this, line));
            
            while (queueSize() >= MAX_QUEUE_SIZE) {
                sleep(0.5);
            }
        } else {
            _freq[line]++;
        }
    }

    if (size()) {
        done();
        wait();
    }
}

void Muniq::output (bool display_count)
{
    for (auto itr = _freq.begin(); itr != _freq.end(); ++itr) {
        if (display_count) {
            cout << setw(7) << itr->second << " " << itr->first << endl;
        } else {
            cout << itr->first << endl;
        }
    }
}
