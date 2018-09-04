#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string.h>
#include "muniq.h"

class CountTask : public Task {
private:
    Muniq &_muniq;
    string _filename;
    int _key;
    const string _delimiters;
    
public:
    CountTask(Muniq &muniq, const string  &filename,
              int key, const string &delimiters) :
        _muniq(muniq),
        _filename(filename),
        _key(key),
        _delimiters(delimiters) {
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
                    char *saveptr = NULL;
                    const char *p = strtok_r((char *)line.c_str(),
                                             _delimiters.c_str(), &saveptr);
                    for (int i = 1; p;
                         p = strtok_r(NULL, _delimiters.c_str(), &saveptr),
                             i++) {
                        if (_key == i) {
                            _muniq.incFreq(p);
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
             const string &delimiters,
             bool display_count,
             bool display_count_after) :
    ThreadPool(parallel),
    _key(key),
    _delimiters(parse_delimiters(delimiters)),
    _freq(FreqTable(display_count, display_count_after)) {
    for (int i = 0; i < 101; i++) {
        _freqs.push_back(FreqTable(display_count, display_count_after));
    }
}

// Need to convert special characters escaped with \ into the real ones.
// Supported special chars are: \t, \n, \r and \.
string Muniq::parse_delimiters (const string delimiters)
{
    string res = "";
    for (int i = 0; i < delimiters.size(); i++) {
        char c;
        switch (c = delimiters[i]) {
        case '\\':
            char c2;
            switch (c2 = delimiters[++i]) {
            case 't':
                res += '\t';
                break;
            case 'n':
                res += '\n';
                break;
            case 'r':
                res += '\r';
                break;
            case '\\':
                res += '\\';
                break;
            default:
                break;
            }
            break;
        default:
            res += c;
            break;
        }
    }
    return res;
}

// Multi-threaded version.
void Muniq::process (const string &filename)
{
    addTask(new CountTask(*this, filename, _key, _delimiters));
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
