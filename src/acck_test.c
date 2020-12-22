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
    assert(!strcmp(expected_token, token));

    printf("    Asserting type as %s\n", token_names[expected_type]);
    assert(identify_token(token) == expected_type);
}

void extract_test(const char **streams, char *expected_token, enum token expected_type)
{
    for (int i = 0; streams[i]; i++)
        token_test(streams[i], expected_token, expected_type);
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
        (const char *[]) {"f a", "f 1", "f     a", "f   df sdf ", NULL},
        "f",
        FLOATDCL
    );

    puts("\nTests complete");
}




























