#pragma once
#include"../header_files/help.h"

double calc();
double sciCalc();
double evaluate_postfix(Token output[], int out_count);
void shunting_yard(const char *input, Token output[], int *out_count);
int is_right_assoc(char op);
int precedence(char op);