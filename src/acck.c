#include "acck.h"

int main(int argc, char *argv[])
{
    #ifdef DEBUG
    run_startup_tests();
    #endif

    int opt;
    while ((opt = getopt(argc, argv, ":o:")) != -1) {
        switch (opt) {
            case 'o':
                drintf("[TODO] Outputting file to %s\n", optarg);
                break;
            case ':':
                printf("'-%c' requires an argument.\n", optopt);
                break;
            case '?':
                printf("Ignoring unknown argument '-%c'\n", optopt);
                break;
        }
    }

    int found_file = 0;
    char file[BUFSIZ] = "";
    for (int i = optind; i < argc; i++) {
        if (!found_file) {
            strcpy(file, argv[i]);
            drintf("Will attempt to compile '%s'\n", argv[i]);
            found_file = 1;
        } else {
            drintf("Ignoring additional argument '%s'\n", argv[i]);
        }
    }

    FILE *fp;
    if (!(fp = fopen(file, "r"))) {
        printf("acck: error: %s: could not open file\n", file);
        exit(1);
    }

    return 0;
}
