#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include "muniq.h"

class CountTask : public Task {
private:
    Muniq &_muniq;
    string _filename;
    int _key;
    
public:
    CountTask(Muniq &muniq, const string  &filename, int key) :
        _muniq(muniq),
        _filename(filename),
        _key(key) {
    }
    bool run(void) {
        ifstream ifs(_filename);
        if (!ifs) { 
            cerr << "Failed to open " << _filename << endl;
            return true;
        } else {
            string line;

            while (getline(ifs, line)) {
                if (line.empty()) {
                    continue;
                }
                if (_key == 0) {
                    // Consider the whole line.
                    _muniq.incFreq(line);
                } else {
                    // Split the line until the key is found.
                    string token;
                    istringstream is(line);
                    for (int i = 0; getline(is, token, '\t'); i++) {
                        if (_key == (i + 1)) {
                            _muniq.incFreq(token);
                            break;
                        }
                    }
                }
            }
        }
        return true;
    }
};

class OutputTask : public Task {
private:
    const FreqTable &_freq_table;
    int _part;
    string _output_dir;

public:
    OutputTask(const FreqTable &freq_table, const string &output_dir, int part) :
        _freq_table(freq_table),
        _output_dir(output_dir),
        _part(part) {
    }
    bool run(void) {
        stringstream ss;
        ss << _output_dir << "/part" << setw(4) << setfill('0') << _part;
        ofstream ofs(ss.str());
        if (!ofs) {
            cerr << "Failed to open " << ss.str() << endl;
            return true;
        } else {
            ofs << _freq_table;
        }
        return true;
    }
};

Muniq::Muniq(int parallel,
             int key,
             bool display_count,
             bool display_count_after) :
    ThreadPool(parallel),
    _key(key),
    _freq(FreqTable(display_count, display_count_after)) {
    for (int i = 0; i < 101; i++) {
        _freqs.push_back(FreqTable(display_count, display_count_after));
    }
}

// Multi-threaded version.
void Muniq::process (const string &filename)
{
    addTask(new CountTask(*this, filename, _key));
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

void Muniq::output (const string &output_dir)
{
    // Single-threaded version will always output to stdout if any.
    cout << _freq;

    // Multi-threaded version.
    if (size()) {
        // Wait for the processing tasks to finish before outputting.
        wait();

        if (output_dir.empty()) {
            // Output to stdout.
            for (int i = 0; i < _freqs.size(); i++) {
                cout << _freqs[i];
            }
        } else {
            // Since we kill all the threads calling wait() above,
            // reset the threadpool here for outputting tasks.
            reset();
            
            // Output to the output_dir. ie. output_dir/part0000, ...
            for (int i = 0; i < _freqs.size(); i++) {
                addTask(new OutputTask(_freqs[i], output_dir, i));
            }

            // Wait for the outputting tasks to finish.
            wait();
        }
    }
}
