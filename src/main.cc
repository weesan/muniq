#include <unistd.h>
#include <iostream>
#include <fstream>
#include <string>
#include "muniq.h"

using namespace std;

int main (int argc, char *argv[])
{
    int opt;
    bool display_count = false;
    bool display_count_after = false;
    bool display_payload = false;
    int parallel = 0;
    string output_dir = "";
    int key = 0;
    string delimiters = "\t";

    while ((opt = getopt(argc, argv, "acd:k:pP:o:")) != -1) {
        switch (opt) {
        case 'a':
            display_count_after = true;
        case 'c':
            display_count = true;
            break;
        case 'd':
            delimiters = optarg;
            break;
        case 'k':
            key = atoi(optarg);
            break;
        case 'p':
            display_payload = true;
            break;
        case 'P':
            parallel = atoi(optarg);
            break;
        case 'o':
            output_dir = optarg;
            break;
        }
    }

    argc -= optind;
    argv += optind;

    Muniq muniq(parallel, key, delimiters,
                display_payload, display_count, display_count_after);
    
    if (argc) {
        for (int i = 0; i < argc; i++) {
            muniq.process(argv[i]);
        }
    } else {
        muniq.process(cin);
    }

    muniq.output(output_dir);
        
    return 0;
}
