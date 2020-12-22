#ifndef ACCK_H
#define ACCK_H

#define DEBUG

#include <assert.h>
#include <ctype.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

#endif
