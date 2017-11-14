#include <iomanip>
#include "file.h"


void File::process (istream &is)
{
    string line;

    while (getline(is, line)) {
        if (!line.size()) {
            continue;
        }
        //cout << line << endl;
        (*this)[line]++;
    }
}

void File::output (bool display_count)
{
    for (auto itr = begin(); itr != end(); ++itr) {
        if (display_count) {
            cout << setw(7) << itr->second << " " << itr->first << endl;
        } else {
            cout << itr->first << endl;
        }
    }
}
