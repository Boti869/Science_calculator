#pragma once 
#include"../header_files/help.h"
#include"../header_files/constants.h"

// One term of a polynomial (e.g. 2x^2)
typedef struct {
    double coeff;     // coefficient (e.g. 2 in 2x^3)
    char var[10];     // variable name (e.g. "x")
    int exponent;     // exponent (e.g. 2 in x^2, 0 if constant)
    double constant; // constant term (5 in 4x + 5)
    char type;       // 'o' for operator and 'a' for algebraic term
    char op;         // if type == 'o'
    char algebra;   // if type == 'a' 
} Poly;

int algebraCalc();
void split(char input[], char **left, char **right);
void algebra_parser(const char input[], Poly output[], int *out_count);
void subtract_poly(Poly lhs[MAX_TOKENS], int lhs_count, Poly rhs[MAX_TOKENS], int rhs_count);