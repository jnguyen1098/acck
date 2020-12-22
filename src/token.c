#include "acck.h"

char *get_token(const char **stream)
{
    while (**stream == ' ') (*stream)++;

    if (!**stream) return NULL;

    if (isdigit(**stream)) return get_digit_token(stream);

    char c = *(*stream)++;

    if (c == 'f')
        return strcpy(malloc(2), "f");

    if (c == 'i')
        return strcpy(malloc(2), "i");

    if (c == 'p')
        return strcpy(malloc(2), "p");

    if (c == '=')
        return strcpy(malloc(2), "=");

    if (c == '+')
        return strcpy(malloc(2), "+");

    if (c == '-')
        return strcpy(malloc(2), "-");

    if (!c || !isalpha(c) || isupper(c))
        return NULL;

    char tmp_tok[2];
    tmp_tok[0] = c;
    tmp_tok[1] = '\0';
    return strcpy(malloc(2), tmp_tok);
}

char *get_digit_token(const char **stream)
{
    char *token = calloc(MAX_DIGIT_TOK, sizeof(char));
    int n = 0;

    while (isdigit(**stream)) {
        token[n++] = **stream;
        (*stream)++;
    }

    if (**stream == '.') {
        token[n++] = **stream;
        (*stream)++;

        while (isdigit(**stream)) {
            token[n++] = **stream;
            (*stream)++;
        }
    }

    return token;
}

enum token identify_token(const char *string)
{
    if (!string)
        return INVALID;

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
