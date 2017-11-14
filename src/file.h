#ifndef FILE_H
#define FILE_H

#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>

using namespace std;

class File : public unordered_map<string, size_t> {
private:
    string _name;
    size_t _total_byte;
    size_t _total_word;
    size_t _total_line;

private:
    void process(istream &is);
    
public:
    File(const string &name) :
        _name(name),
        _total_byte(0),
        _total_word(0),
        _total_line(0) {
        ifstream ifs(name);
        if (!ifs) {
            cerr << "Failed to open " << name << endl;
            return;
        } else {
            process(ifs);
        }
    }
    File(istream &is) :
        _name(""),
        _total_byte(0),
        _total_word(0),
        _total_line(0) {
        process(is);
    }
    void output(bool uniq_count);
};

#endif // FILE_H
