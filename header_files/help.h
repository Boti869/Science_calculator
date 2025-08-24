#pragma once
#include <wchar.h>

typedef struct {
    char type;    // 'n' for number, 'o' for operator
    double value; // if type == 'n'
    char op;      // if type == 'o'
} Token;

typedef struct {
    double voltage;
    double resistance;
    double current;
    double power;
} Type;

typedef struct{
    double V, f, R, L, Xl, Z, I, S, Q, P, phi, C, Xc;
}Inputs;

typedef Inputs Outputs;

int precedence(char op);
int is_right_assoc(char op);
void shunting_yard(const char *input, Token output[], int *out_count);
double evaluate_postfix(Token output[], int out_count);
double pretty_print(double num, wchar_t unit);
void add_number(const char *input, int *i, Token *output, int *out_count);
Type value_of(char buffer[]);
Inputs RLsCalc(char buffer[]);
void print_results(Outputs *out, int type);
Inputs RCsCalc(char buffer[]);