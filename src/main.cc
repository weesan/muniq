#include <unistd.h>
#include <iostream>
#include <fstream>
#include "muniq.h"

using namespace std;

int main (int argc, char *argv[])
{
    int opt;
    bool display_count = false;

    while ((opt = getopt(argc, argv, "c")) != -1) {
        switch (opt) {
        case 'c':
            display_count = true;
            break;
        }
    }

    argc -= optind;
    argv += optind;

    Muniq muniq;
    
    if (argc) {
        for (int i = 0; i < argc; i++) {
            muniq.process(argv[i]);
        }
    } else {
        muniq.process(cin);
    }

    muniq.output(display_count);
        
    return 0;
}
