#include "muniq.h"

void Muniq::process (const string &filename)
{
    push_back(File(filename));
}

void Muniq::process(istream &in)
{
    push_back(File(in));
}

void Muniq::output (bool display_count)
{
    for (int i = 0; i < size(); i++) {
        (*this)[i].output(display_count);
    }
}
