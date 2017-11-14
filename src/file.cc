#include <iomanip>
#include "file.h"

#define FILE_MAX_QUEUE_SIZE    1000

class CountTask : public Task {
private:
    File &_file;
    string _line;
    
public:
    CountTask(File &file, const string &line) :
        _file(file),
        _line(line) {
    }
    void run(void) {
        _file.incFreq(_line);
    }
};

void File::process (istream &is)
{
    string line;

    while (getline(is, line)) {
        if (!line.size()) {
            continue;
        }
        //cout << line << endl;
        if (_parallel) {
            addTask(new CountTask(*this, line));
            
            while (queueSize() >= FILE_MAX_QUEUE_SIZE) {
                sleep(0.5);
            }
        } else {
            _freq[line]++;
        }
    }

    done();
    wait();
}

void File::output (bool display_count)
{
    for (auto itr = _freq.begin(); itr != _freq.end(); ++itr) {
        if (display_count) {
            cout << setw(7) << itr->second << " " << itr->first << endl;
        } else {
            cout << itr->first << endl;
        }
    }
}
