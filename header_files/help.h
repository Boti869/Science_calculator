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
    double V, f, R, L, Xl, Z, I, S, Q, P, phi, C, Xc, Bl, Bc, G, Y;
}Inputs;

typedef Inputs Outputs;
typedef enum {
    SERIES_RL,
    SERIES_RC,
    SERIES_RLC,
    PARALLEL_RL,
    PARALLEL_RC,
    PARALLEL_RLC
} CircuitType;

double pretty_print(double num, wchar_t unit);
int up_low(const char *s1, const char *s2);
Type value_of(char buffer[]);
void print_results(Outputs *out, int type, int parallel);
Inputs parseInputs(char buffer[], const char *tokens[], int count, int type, int parallel);
double apply_prefix(char *str);
Inputs RLsCalc(char buffer[]);
Inputs RCsCalc(char buffer[]);
Inputs RLpCalc(char buffer[]);
Inputs RCpCalc(char buffer[]);
Inputs RLCsCalc(char buffer[]);
Inputs RLCpCalc(char buffer[]);