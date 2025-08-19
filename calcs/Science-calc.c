//Forgot to mention: This was made with AI help
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

double sciCalc() {
    char expr[256];
    wprintf(L"Enter expression: ");
    fgets(expr, sizeof(expr), stdin);
    expr[strcspn(expr, "\n")] = '\0';

    Token output[MAX_TOKENS];
    int out_count;
    shunting_yard(expr, output, &out_count);

    double result = evaluate_postfix(output, out_count);
    pretty_print(result, '\0');

    return 0;
}
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
        if (isspace(input[i])) { i++; continue; }

        // Number (integer, decimal, or scientific notation)
        if (isdigit(input[i]) || input[i] == '.' || input[i] == '+' || input[i] == '-') {
            char num[64];
            int j = 0;

            // optional leading sign
            if (input[i] == '+' || input[i] == '-') num[j++] = input[i++];

            // digits before decimal
            while (isdigit(input[i])) num[j++] = input[i++];

            // optional decimal and digits after
            if (input[i] == '.') {
                num[j++] = input[i++];
                while (isdigit(input[i])) num[j++] = input[i++];
            }

            // optional exponent
            if (input[i] == 'e' || input[i] == 'E') {
                num[j++] = input[i++];
                if (input[i] == '+' || input[i] == '-') num[j++] = input[i++];
                if (!isdigit(input[i])) {
                    wprintf(L"Error: invalid scientific notation\n");
                    exit(1);
                }
                while (isdigit(input[i])) num[j++] = input[i++];
            }

            num[j] = '\0';
            output[(*out_count)].type = 'n';
            output[(*out_count)].value = atof(num);
            (*out_count)++;
            continue;
        }

        // Operator
        if (strchr("+-*/^", input[i])) {
            char op = input[i++];
            while (stack_top >= 0 && strchr("+-*/^", stack[stack_top]) &&
                  ((precedence(stack[stack_top]) > precedence(op)) ||
                   (precedence(stack[stack_top]) == precedence(op) && !is_right_assoc(op)))) {
                output[(*out_count)].type = 'o';
                output[(*out_count)].op = stack[stack_top--];
                (*out_count)++;
            }
            stack[++stack_top] = op;
            continue;
        }

        // Parentheses
        if (input[i] == '(') { stack[++stack_top] = input[i++]; continue; }
        if (input[i] == ')') {
            while (stack_top >= 0 && stack[stack_top] != '(') {
                output[(*out_count)].type = 'o';
                output[(*out_count)].op = stack[stack_top--];
                (*out_count)++;
            }
            if (stack_top >= 0 && stack[stack_top] == '(') stack_top--;
            else { wprintf(L"Error: mismatched parentheses\n"); exit(1); }
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
            if (top < 1) { wprintf(L"Error: insufficient operands\n"); exit(1); }
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

    if (top != 0) { wprintf(L"Error: invalid expression\n"); exit(1); }
    return stack[0];
}
