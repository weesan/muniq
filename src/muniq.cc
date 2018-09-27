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
    bool _display_payload;
    const string _delimiters;
    int _verbose;
    
private:
    void split(const string &s, const string &delim, vector<string> &res) {
        size_t p1 = 0, p2 = 0;
        for (p2 = s.find(delim);
             p2 != s.npos;
             p1 = p2 + 1, p2 = s.find(delim, p1)) {
            size_t len = p2 - p1;
            if (len) {
                res.push_back(s.substr(p1, len));
            }
        }
        // The last word if available.
        if (p1 < s.size()) {
            res.push_back(s.substr(p1));
        }
    }
    
public:
    CountTask(Muniq &muniq, const string  &filename,
              int key, const string &delimiters, bool display_payload,
              int verbose) :
        _muniq(muniq),
        _filename(filename),
        _key(key),
        _delimiters(delimiters),
        _display_payload(display_payload),
        _verbose(verbose) {
    }
    bool run(void) {
        if (_verbose) {
            cerr << "Processing " << _filename << " ..." << endl;
        }
        
        ifstream ifs(_filename);
        if (!ifs) { 
            cerr << "Failed to open " << _filename << endl;
            return true;
        }

        string line;
        while (getline(ifs, line)) {
            if (line.empty()) {
                continue;
            }
                
            // If no key is given, treat the whole line as the key.
            if (_key == 0) {
                _muniq.incFreq(line);
                continue;
            }

            // When a key is given, ...
            vector<string> res;
            string &key_str = line;
            string payload;

            split(line, _delimiters, res);

            // Get the correct key.
            if (_key <= res.size()) {
                key_str = res[_key - 1];
            }

            // Collect the payload when instructed.
            if (_display_payload) {
                for (int i = 0; i < res.size(); i++) {
                    if (i == _key - 1) {
                        continue;
                    }
                    if (!payload.size()) {
                        payload = res[i];
                    } else {
                        payload += string(" ") + res[i];
                    }
                }
            }

            // Count the key.
            _muniq.incFreq(key_str, payload);
        }
            
        return true;
    }
};

class OutputTask : public Task {
private:
    const FreqTable &_freq_table;
    int _part;
    string _output_dir;
    int _verbose;

public:
    OutputTask(const FreqTable &freq_table,
               const string &output_dir,
               int part,
               int verbose) :
        _freq_table(freq_table),
        _output_dir(output_dir),
        _part(part),
        _verbose(verbose) {
    }
    bool run(void) {
        stringstream ss;
        ss << _output_dir << "/part" << setw(4) << setfill('0') << _part;

        if (_verbose) {
            cerr << "Outputting " << ss.str() << " ..." << endl;
        }
        
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
             int num_of_hashes,
             int key,
             const string &delimiters,
             bool display_payload,
             bool display_count,
             bool display_count_after,
             int verbose) :
    ThreadPool(parallel),
    _key(key),
    _delimiters(parse_delimiters(delimiters)),
    _display_payload(display_payload),
    _freq(FreqTable(display_payload, display_count, display_count_after)),
    _verbose(verbose) {
    num_of_hashes = num_of_hashes == 0 ? DEFAULT_NUM_OF_HASHES : num_of_hashes;
    for (int i = 0; i < num_of_hashes; i++) {
        _freqs.push_back(FreqTable(display_payload,
                                   display_count,
                                   display_count_after));
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
    addTask(new CountTask(*this, filename,
                          _key, _delimiters, _display_payload, _verbose));
}

// Single-threaded version reading from stdin.
void Muniq::process (istream &is)
{
    string line;

    while (getline(is, line)) {
        if (line.empty()) {
            continue;
        }
        _freq[line].inc();
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
                addTask(new OutputTask(_freqs[i], output_dir, i, _verbose));
            }

            // Wait for the outputting tasks to finish.
            wait();
        }
    }
}
