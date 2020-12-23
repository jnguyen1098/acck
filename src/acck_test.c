#include "acck.h"

const char *token_names[] = {
    "FLOATDCL", "INTDCL", "PRINT", "ID", "ASSIGN", "PLUS", "MINUS",
    "INUM", "FNUM", "BLANK", "INVALID"
};

void identifier_test(enum token expected, const char **passes, const char **fails)
{
    printf("\nTesting %s detection passes\n", token_names[expected]);
    for (int i = 0; passes[i]; i++) {
        printf("    Testing \"%s\"\n", passes[i]);
        assert(identify_token(passes[i]) == expected);
    }

    printf("\nTesting %s detection fails\n", token_names[expected]);
    for (int i = 0; fails[i]; i++) {
        printf("    Testing \"%s\"\n", fails[i]);
        assert(!(identify_token(fails[i]) == expected));
    }
}

void token_test(const char *stream, char *expected_token, enum token expected_type)
{
    printf("\nTesting token(\"%s\") -> \"%s\" (%s)\n",
            stream, expected_token, token_names[expected_type]);
    char *token = get_token(&stream);

    printf("    Asserting result (\"%s\" | \"%s\") as \"%s\"\n",
        token, stream, expected_token);
    if (expected_token) {
        assert(!strcmp(expected_token, token));
    } else {
        assert(token == expected_token);
    }

    printf("    Asserting type as %s\n", token_names[expected_type]);
    assert(identify_token(token) == expected_type);

    puts("    Packing token");
    token_t *packed = pack_token(token);

    printf("        Testing packed value (expecting \"%s\")\n", token);
    if (token) {
        assert(!strcmp(packed->data, token));
    } else {
        assert(!packed);
    }

    printf("        Texting packed type as %s\n", token_names[expected_type]);
    if (expected_type != INVALID) {
        assert(packed->type == expected_type);
    } else {
        assert(!packed);
    }

    free(packed);
    free(token);
}

void extract_test(const char **streams, char *expected_token, enum token expected_type)
{
    for (int i = 0; streams[i]; i++)
        token_test(streams[i], expected_token, expected_type);
}

int isvalid(char c)
{
    if (c >= 'a' && c <= 'z')
        return 1;

    if (c == '=' || c == '+' || c == '-')
        return 1;

    if (c >= '0' && c <= '9')
        return 1;

    if (c == ' ' || c == '.')
        return 1;

    return 0;
}

void run_startup_tests(void)
{
    puts("Starting tests");

    // Test float declaration
    identifier_test(FLOATDCL,
        (const char *[]) { "f", NULL },
        (const char *[]) {
            "F", "i", "p", "as", "=", "+", "-", "14", "33.3", " ", NULL
        }
    );

    // Test int declaration
    identifier_test(INTDCL,
        (const char *[]) { "i", NULL },
        (const char *[]) {
            "I", "f", "p", " ", "324", "325423.245", "+", "=", NULL
        }
    );

    // Test print
    identifier_test(PRINT,
        (const char *[]) { "p", NULL },
        (const char *[]) {
            "I", "i", "P", "f", " ", "324", "32423.245", "+", "=", NULL
        }
    );

    // Test id
    identifier_test(ID,
        (const char *[]) {
            "a", "b", "c", "d", "e",
            "g", "h",
            "j", "k", "l", "m", "n", "o",
            "q", "r", "s", "t", "u", "v", "w", "x", "y", "z", NULL
        },
        (const char *[]) {
            "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K",
            "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V",
            "W", "X", "Y", "Z",
            
            "f", "i", "p", "=", "+", "-", " ", "0", "0.2", NULL
        }
    );

    // Test assign
    identifier_test(ASSIGN,
        (const char *[]) { "=", NULL },
        (const char *[]) { "f", "i", "p", "+", "-", "0", "0.0", " ", 0}
    );

    // Test plus
    identifier_test(PLUS,
        (const char *[]) { "+", NULL },
        (const char *[]) { "f", "i", "p", "=", "-", "0", "0.0", " ", 0}
    );

    // Test minus
    identifier_test(MINUS,
        (const char *[]) { "-", NULL },
        (const char *[]) { "f", "i", "p", "+", "=", "0", "0.0", " ", 0}
    );

    // Test integer
    identifier_test(INUM,
        (const char *[]) {
            "1", "2", "3", "4", "5", "6", "7", "8", "9", "0",
            "113141", "23456", "4593465", "929532523535", NULL
        },
        (const char *[]) { "f", "i", "p", "+", "=", "03.0", " ",
            "2353.235", ".2343", "234.", 0
        }
    );

    // Test float
    identifier_test(FNUM,
        (const char *[]) { "0.0", "1.1", "2.2", "235.23", "9.3", NULL },
        (const char *[]) { "3243", ".3243", "234234.", "....", NULL }
    );

    // Test blank
    identifier_test(BLANK,
        (const char *[]) { " ", "  ", "   ", "    ", "     ", NULL },
        (const char *[]) { "f", "i", "p", "a", "=", "+", "-", NULL }
    );

    // Test extract floatdcl
    extract_test(
        (const char *[]) { "f", "f a", "f 1", "f     a", "f   df sdf ", NULL },
        "f",
        FLOATDCL
    );

    // Test extract intdcl
    extract_test(
        (const char *[]) { "i", "i p", "i 2342fwwf", "i      3", NULL },
        "i",
        INTDCL
    );

    // Test extract print
    extract_test(
        (const char *[]) { "p", "p i", "p     3", NULL },
        "p",
        PRINT
    );

    // Test extract assign
    extract_test(
        (const char *[]) { "=", "= i", "=     3", NULL },
        "=",
        ASSIGN
    );

    // Test extract plus
    extract_test(
        (const char *[]) { "+", "+ i", "+     3", NULL },
        "+",
        PLUS
    );

    // Test extract minus
    extract_test(
        (const char *[]) { "-", "- i", "-     3", NULL },
        "-",
        MINUS
    );

    // Test extract ID
    {
        const char *ids[] = {
            "a", "b", "c", "d", "e", "g", "h", "j", "k", "l", "m", "n",
            "o", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z", NULL
        };

        const char *ends[] = {
            " a", " b", " c", " d", " e", " f", " g", " h", " i", " j",
            " k", " l", " m", " n", " o", " p", " q", " r", " s", " t",
            " u", " v", " w", " x", " y" , " z",
            "     1", " 1", " a", " b", "       2", NULL
        };

        for (int i = 0; ids[i]; i++) {
            for (int j = 0; ends[j]; j++) {
                char stream[19] = "";
#ifdef PREWHITE
                strcat(stream, "   ");
#endif
                strcat(stream, ids[i]);
                strcat(stream, ends[j]);

                char *stream_ptr = stream;

                const char *streams[2];
                streams[0] = stream_ptr;
                streams[1] = NULL;

                extract_test(streams, (char *)ids[i], ID);
            }
        }
    }

    // Test extract inum
    {
        const char *ints[] = {
            "0", "1", "2", "3", "4", "5", "6", "7", "8", "9",
            "02346346", "13455345", "23254534", "3235334345",
            "42352356", "53233432", "62435232", "7235849723",
            "82384333", "92385723", NULL
        };

        const char *ends[] = {
            " a", " b", " c", " d", " e", " f", " g", " h", " i", " j",
            " k", " l", " m", " n", " o", " p", " q", " r", " s", " t",
            " u", " v", " w", " x", " y" , " z",
            
            " 0", " 1", " 2", " 3", " 4", " 5", " 6", " 7", " 8", " 9",

            "     1", " 1", " a", " b", "       2", NULL
        };

        for (int i = 0; ints[i]; i++) {
            for (int j = 0; ends[j]; j++) {
                char stream[35] = "";
#ifdef PREWHITE
                strcat(stream, "   ");
#endif
                strcat(stream, ints[i]);
                strcat(stream, ends[j]);

                char *stream_ptr = stream;

                const char *streams[2];
                streams[0] = stream_ptr;
                streams[1] = NULL;

                extract_test(streams, (char *)ints[i], INUM);
            }
        }
    }

    // Test extract fnum
    {
        const char *ints[] = {
            "0", "1", "2", "3", "4", "5", "6", "7", "8", "9",
            "02346346", "13455345", "23254534", "3235334345",
            "42352356", "53233432", "62435232", "7235849723",
            "82384333", "92385723", NULL
        };

        const char *ends[] = {
            " a", " b", " c", " d", " e", " f", " g", " h", " i", " j",
            " k", " l", " m", " n", " o", " p", " q", " r", " s", " t",
            " u", " v", " w", " x", " y" , " z",
            
            " 0", " 1", " 2", " 3", " 4", " 5", " 6", " 7", " 8", " 9",

            "     1", " 1", " a", " b", "       2", NULL
        };

        for (int i = 0; ints[i]; i++) {
            for (int k = 0; ints[k]; k++) {
                for (int j = 0; ends[j]; j++) {
                    char expected[32] = "";
                    strcat(expected, ints[i]);
                    strcat(expected, ".");
                    strcat(expected, ints[k]);

                    char stream[35] = "";
#ifdef PREWHITE
                    strcat(stream, "   ");
#endif
                    strcat(stream, ints[i]);
                    strcat(stream, ".");
                    strcat(stream, ints[k]);
                    strcat(stream, ends[j]);

                    char *stream_ptr = stream;

                    const char *streams[2];
                    streams[0] = stream_ptr;
                    streams[1] = NULL;

                    extract_test(streams, expected, FNUM);
                }
            }
        }
    }

    // Test for invalid
    {
        const char *streams[3];
        for (unsigned char c = 0; c < 128; c++) {
            if (isvalid(c)) continue;

            char nowhite[16] = "";
            nowhite[0] = c;
            char white[16] = "   ";
            white[3] = c;
            white[4] = '\0';

            char *nowhite_ptr = nowhite;
            char *white_ptr = white;

            streams[0] = nowhite_ptr;
            streams[1] = white_ptr;
            streams[2] = NULL;

            extract_test(streams, NULL, INVALID);
        }
    }

    puts("\nTests complete");
}




























