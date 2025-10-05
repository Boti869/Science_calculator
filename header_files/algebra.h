#pragma once 
#include"../header_files/help.h"

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

typedef struct {
    char left[256];
    char right[256];
} Sides;

int algebraCalc();
void split(char input[], char **left, char **right);
void algebra_parser(const char input[], Poly output[], int *out_count);
Poly collect_terms(Poly *p, Poly terms[], int n);
Sides subtract_poly(Poly lhs, int lhs_count, Poly rhs, int rhs_count);