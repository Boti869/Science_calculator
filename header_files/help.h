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
    double V, f, R, L, Xl, Z, I, S, Q, P, phi;
}Inputs;

typedef Inputs Outputs;

enum known_mask {
    KN_V   = 1<<0,
    KN_I   = 1<<1,
    KN_R   = 1<<2,
    KN_L   = 1<<3,
    KN_f   = 1<<4,
    KN_phi = 1<<5,
    KN_P   = 1<<6,
    KN_Q   = 1<<7,
    KN_S   = 1<<8,
    KN_Xl  = 1<<9,
    KN_Z   = 1<<10
};

int precedence(char op);
int is_right_assoc(char op);
void shunting_yard(const char *input, Token output[], int *out_count);
double evaluate_postfix(Token output[], int out_count);
double pretty_print(double num, wchar_t unit);
void add_number(const char *input, int *i, Token *output, int *out_count);
Type value_of(char buffer[]);
Inputs RLsCalc(char buffer[]);