#ifndef ACCK_H
#define ACCK_H

#define DEBUG

#include <getopt.h>
#include <stdio.h>
#include <string.h>

#ifdef DEBUG
#   define drintf printf
#   define duts puts
#else
void nop(){}
#   define drintf nop
#   define duts nop
#endif

enum token {
    FLOATDCL, INTDCL, PRINT, ID, ASSIGN, PLUS, MINUS, INUM, FNUM, BLANK,
};

// Runs all startup tests
void run_startup_tests(void);

#endif
