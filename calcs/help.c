#include"../header_files/calc.h"
#include"../header_files/initLaws.h"
#include"../header_files/constants.h"
#include"../header_files/help.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <locale.h>
#include <wchar.h>

int precedence(char op) {
    switch(op) {
        case '+': case '-': return 1;
        case '*': case '/': return 2;
        case '^':           return 3;
    }
    return 0;
}

int is_right_assoc(char op) {
    return op == '^';
}
void shunting_yard(const char *input, Token output[], int *out_count) {
    char stack[MAX_TOKENS];
    int stack_top = -1;
    *out_count = 0;

    int i = 0;
    while (input[i] != '\0') {
        if (isspace(input[i])) {
            i++;
            continue;
        }

        // Number
        if (isdigit(input[i]) || input[i] == '.') {
            char num[32];
            int j = 0;
            while (isdigit(input[i]) || input[i] == '.') {
                num[j++] = input[i++];
            }
            num[j] = '\0';
            output[(*out_count)].type = 'n';
            output[(*out_count)].value = atof(num);
            (*out_count)++;
            continue;
        }

        // Operator
        if (strchr("+-*/^", input[i])) {
            char op = input[i];
            while (stack_top >= 0 && strchr("+-*/^", stack[stack_top]) &&
                   ((precedence(stack[stack_top]) > precedence(op)) ||
                    (precedence(stack[stack_top]) == precedence(op) && !is_right_assoc(op)))) {
                output[(*out_count)].type = 'o';
                output[(*out_count)].op = stack[stack_top--];
                (*out_count)++;
            }
            stack[++stack_top] = op;
            i++;
            continue;
        }

        // Left parenthesis
        if (input[i] == '(') {
            stack[++stack_top] = input[i];
            i++;
            continue;
        }

        // Right parenthesis
        if (input[i] == ')') {
            while (stack_top >= 0 && stack[stack_top] != '(') {
                output[(*out_count)].type = 'o';
                output[(*out_count)].op = stack[stack_top--];
                (*out_count)++;
            }
            if (stack_top >= 0 && stack[stack_top] == '(') {
                stack_top--; // pop '('
            }
            i++;
            continue;
        }

        wprintf(L"Error: invalid character '%c'\n", input[i]);
        exit(1);
    }

    while (stack_top >= 0) {
        if (stack[stack_top] == '(' || stack[stack_top] == ')') {
            wprintf(L"Error: mismatched parentheses\n");
            exit(1);
        }
        output[(*out_count)].type = 'o';
        output[(*out_count)].op = stack[stack_top--];
        (*out_count)++;
    }
}

double evaluate_postfix(Token output[], int out_count) {
    double stack[MAX_TOKENS];
    int top = -1;

    for (int i = 0; i < out_count; i++) {
        if (output[i].type == 'n') {
            stack[++top] = output[i].value;
        } else if (output[i].type == 'o') {
            if (top < 1) {
                wprintf(L"Error: insufficient operands\n");
                exit(1);
            }
            double b = stack[top--];
            double a = stack[top--];
            switch (output[i].op) {
                case '+': stack[++top] = a + b; break;
                case '-': stack[++top] = a - b; break;
                case '*': stack[++top] = a * b; break;
                case '/': stack[++top] = a / b; break;
                case '^': stack[++top] = pow(a, b); break;
            }
        }
    }

    if (top != 0) {
        wprintf(L"Error: invalid expression\n");
        exit(1);
    }
    return stack[0];
}

double pretty_print(double num, wchar_t unit) {
    int mult = 0;
    int sign = (num < 0) ? -1 : 1;
    num = (num < 0) ? -num : num;

    while (num >= 1000.0 || num < 1.0 && num != 0.0) {
        if (num >= 1000.0) { num /= 1000.0; mult++; }
        else if (num < 1.0) { num *= 1000.0; mult--; }
    }

    num *= sign;

    setlocale(LC_ALL, "");
    const wchar_t *prefixes[] = { L"f", L"p", L"n", L"µ", L"m", L"", L"k", L"M", L"G", L"T", L"P" };
    int index = mult + 5;
    if (index >= 0 && index <= 10) {
       wprintf(L"%.5f%ls%lc", num, prefixes[index], unit);
    } else {
        wprintf(L"%.5fe%d%lc", num, mult * 3, unit);
        
    }   
    return num;
}
