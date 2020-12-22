#include "acck.h"

enum token identify_token(const char *string)
{
    if (!string) {
        fprintf(stderr, "Received NULL in identify_token(). Aborting\n");
        abort();
    }

    if (strlen(string) == 1) {
        switch (*string) {
            case 'f':
                return FLOATDCL;

            case 'i':
                return INTDCL;

            case 'p':
                return PRINT;

            case '=':
                return ASSIGN;

            case '+':
                return PLUS;

            case '-':
                return MINUS;
        }

        if (*string >= 'a' && *string <= 'z')
            return ID;
    }

    int inum_flag = 1;
    for (int i = 0; string[i]; i++) {
        if (!isdigit(string[i])) {
            inum_flag = 0;
            break;
        }
    }

    if (inum_flag)
        return INUM;

    int fnum_flag = 1;
    int dot_flag = 0;
    int dig_flag = 0;
    int f_flag = 0;
    for (int i = 0; string[i]; i++) {
        if (isdigit(string[i])) {
            if (dot_flag) f_flag = 1;
            dig_flag = 1;
        }
        else if (string[i] == '.' && !dot_flag && dig_flag) {
            dot_flag = 1;
        } else {
            fnum_flag = 0;
            break;
        }
    }

    if (fnum_flag && f_flag)
        return FNUM;

    for (int i = 0; string[i]; i++) {
        if (string[i] != ' ') {
            return INVALID;
        }
    }

    return BLANK;
}



















