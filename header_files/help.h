#pragma once

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
    double V, R, L, Xl, Z, I, P, phi;
}RLseriesType;

int precedence(char op);
int is_right_assoc(char op);
void shunting_yard(const char *input, Token output[], int *out_count);
double evaluate_postfix(Token output[], int out_count);
double pretty_print(double num, wchar_t unit);
Type value_of(char buffer[]);
RLseriesType RLsCalc(char buffer[]);