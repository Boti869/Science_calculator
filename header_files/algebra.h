#pragma once 
#include"../header_files/help.h"

typedef struct {
    double coeff;   // coefficient (4 in 4x)
    char var;       // variable name 'x'
    double constant; // for standalone numbers
} Term;

int algebraCalc();
char* split(char input[], int iter);
void algebra_parser(const char input[], Token output[], int *out_count);
double evaluate_algebra(Token output[], int out_count);