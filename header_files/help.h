#pragma once

#include"../header_files/calc.h"
#include"../header_files/constants.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

typedef struct {
    char type;    // 'n' for number, 'o' for operator
    double value; // if type == 'n'
    char op;      // if type == 'o'
} Token;

int precedence(char op);
int is_right_assoc(char op);
void shunting_yard(const char *input, Token output[], int *out_count);
double evaluate_postfix(Token output[], int out_count);
double pretty_print(double num, wchar_t unit, char mode);