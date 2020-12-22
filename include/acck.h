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
#define MAX_DIGIT_TOK 32

#ifdef DEBUG
#   define drintf printf
#   define duts puts
#else
void nop(){}
#   define drintf nop
#   define duts nop
#endif

// Tokens
enum token {
    FLOATDCL, INTDCL, PRINT, ID, ASSIGN, PLUS, MINUS, INUM, FNUM, BLANK, INVALID
};

// Runs all startup tests
void run_startup_tests(void);

// Derive token
enum token identify_token(const char *string);

// Get token
char *get_token(const char **stream);

// Get digit token
char *get_digit_token(const char **stream);

#endif
