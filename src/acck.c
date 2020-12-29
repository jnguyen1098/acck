#include "acck.h"

int main(int argc, char *argv[])
{
    #ifdef DEBUG
//    run_startup_tests();
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

    if (!file[0]) {
        return printf("No file specified. Terminating.\n"), 1;
    }

    FILE *fp;
    if (!(fp = fopen(file, "r"))) {
        printf("acck: error: %s: could not open file\n", file);
        exit(1);
    }

    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    printf("%ld\n", size);
    rewind(fp);
    char *buf = malloc(size + 1);
    fread(buf, sizeof(char), size, fp);
    puts(buf);

    const char *buf2 = buf;

    char *token = NULL;
    int i = 0;
    while ((token = get_token(&buf2))) {
        i++;
        //puts(token);
    }

    token_t *tokens = calloc(i, sizeof(token_t));
    memset(tokens, 0, i);

    buf2 = buf;
    i = 0;
    while ((token = get_token(&buf2))) {
        token_t *tmp = pack_token(token);
        if (tmp == NULL) { fprintf(stderr, "uhh\n"); exit(1); }
        tokens[i++] = *tmp;
        free(tmp);
    }

	const char *token_names[] = {
        "FLOATDCL", "INTDCL", "PRINT", "ID", "ASSIGN", "PLUS", "MINUS",
        "INUM", "FNUM", "BLANK", "INVALID"
    };

    puts("#include <stdio.h>");
    puts("");
    puts("int main(void)");
    puts("{");

    for (int j = 0; j < i; j++) {
        //printf("%-10s: %s\n", token_names[tokens[j].type], tokens[j].data);
        switch (tokens[j].type) {

            case FLOATDCL:
                printf("    float ");
                break;

            case INTDCL:
                printf("    int ");
                break;

            case PRINT:
                printf("printf(\"placeholder\\n\");\n");
                break;

            case ID:
                printf("%s;\n", tokens[j].data);
                break;

            default:
                fprintf(stderr, "Abort\n");
                exit(1);
                break;
        }
    }

    return 0;
}























