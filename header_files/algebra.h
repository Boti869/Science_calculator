#pragma once 
#include"../header_files/help.h"

typedef struct {
    double coeff;    // coefficient (4 in 4x)
    char var[10];        // variable name 'x'
    double constant; // constant term (5 in 4x + 5)
    int exponent;    // exponent (2 in 4x^2)
    char type;       // 'n' for number, 'o' for operator and 'a' for variable
    char op;         // if type == 'o'
    char algebra;   // if type == 'a' 
} Term;

typedef struct {
    double coeff;     // coefficient (e.g. 2 in 2x^2)
    char var[10];     // variable name (e.g. "x")
    int exponent;     // exponent (e.g. 2 in x^2)
    int is_const;     // 1 if it's just a constant, 0 if it has variable
} Poly;

int algebraCalc();
int split(char input[], char **left, char **right);
void algebra_parser(const char input[], Term output[], int *out_count);
int collect_terms(Poly polys[], int *polyCount, Term t);
Poly make_const(double c);
Poly make_var(double coeff, const char *var, int exp);
Poly add_poly(Poly a, Poly b);
Poly mul_poly(Poly a, Poly b);
Poly pow_poly(Poly base, Poly exp);
Poly evaluate_algebra(Term terms[], int n);
void solve_equation(Poly left, Poly right);