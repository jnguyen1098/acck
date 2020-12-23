#ifndef ACCK_H
#define ACCK_H

#include <assert.h>
#include <ctype.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEBUG
#define PREWHITE
#define MAX_TOK 32

#ifdef DEBUG
#   define drintf printf
#   define duts puts
#else
void nop(){}
#   define drintf nop
#   define duts nop
#endif

// Tokens type
enum token {
    FLOATDCL, INTDCL, PRINT, ID, ASSIGN, PLUS, MINUS, INUM, FNUM, BLANK, INVALID
};

// Token struct
typedef struct token_struct {
    char data[MAX_TOK];
    enum token type;
} token_t;

// Runs all startup tests
void run_startup_tests(void);

// Derive token
enum token identify_token(const char *string);

// Get token text
char *get_token(const char **stream);

// Get digit token
char *get_digit_token(const char **stream);

// Convert token text into proper struct
token_t *pack_token(char *token_text);

#endif
