//This was made with AI help
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
    wprintf(L"'exit' or 'quit' to exit\n");
    while(1){
    wprintf(L"\nEnter expression: ");
    fgets(expr, sizeof(expr), stdin);
    expr[strcspn(expr, "\n")] = '\0';
    if (strcmp(expr, "exit") == 0 || strcmp(expr, "quit") == 0) {
        wprintf(L"Exiting...\n");
        return 0;
    }
    Token output[MAX_TOKENS];
    int out_count;
    shunting_yard(expr, output, &out_count);

    double result = evaluate_postfix(output, out_count);
    pretty_print(result, '\0');
    }
    return 0;
}

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^' || c == 'u' || c == 'p';
}

int precedence(char op) {
    switch (op) {
        case 'u': case 'p': return 4; 
        case '^': return 3;         
        case '*': case '/': return 2;
        case '+': case '-': return 1;
    }
    return 0;
}


int is_right_assoc(char op) {
    return (op == '^' || op == 'u' || op == 'p');
}

void shunting_yard(const char *input, Token output[], int *out_count) {
    char stack[MAX_TOKENS];
    int stack_top = -1;
    *out_count = 0;

    int i = 0;
    int expect_unary = 1; 

    while (input[i] != '\0') {
        if (isspace((unsigned char)input[i])) { i++; continue; }

        if (isdigit((unsigned char)input[i]) || input[i] == '.') {
            char num[64];
            int j = 0;

            // integer part
            if (isdigit((unsigned char)input[i])) {
                while (isdigit((unsigned char)input[i]) && j < (int)sizeof(num)-1) {
                    num[j++] = input[i++];
                }
            }

            // fractional
            if (input[i] == '.') {
                if (j < (int)sizeof(num)-1) num[j++] = input[i++];
                while (isdigit((unsigned char)input[i]) && j < (int)sizeof(num)-1) {
                    num[j++] = input[i++];
                }
            }

            // exponent
            if (input[i] == 'e' || input[i] == 'E') {
                if (j < (int)sizeof(num)-1) num[j++] = input[i++]; 
                if (input[i] == '+' || input[i] == '-') {
                    if (j < (int)sizeof(num)-1) num[j++] = input[i++];
                }
                if (!isdigit((unsigned char)input[i])) {
                    fprintf(stderr, "Error: invalid scientific notation (no digits after exponent)\n");
                    exit(1);
                }
                while (isdigit((unsigned char)input[i]) && j < (int)sizeof(num)-1) {
                    num[j++] = input[i++];
                }
            }

            num[j] = '\0';

            char *endptr;
            double val = strtod(num, &endptr);
            if (endptr == num) {
                fprintf(stderr, "Error: failed to parse number '%s'\n", num);
                exit(1);
            }

            output[*out_count].type = 'n';
            output[*out_count].value = val;
            (*out_count)++;
            expect_unary = 0; // next token should be binary operator or )
            continue;
        }

        // Parentheses
        if (input[i] == '(') {
            stack[++stack_top] = input[i++];
            expect_unary = 1; // after '(' a unary is allowed
            continue;
        }
        if (input[i] == ')') {
            while (stack_top >= 0 && stack[stack_top] != '(') {
                output[*out_count].type = 'o';
                output[*out_count].op = stack[stack_top--];
                (*out_count)++;
            }
            if (stack_top >= 0 && stack[stack_top] == '(') stack_top--;
            else { fprintf(stderr, "Error: mismatched parentheses\n"); exit(1); }
            i++;
            expect_unary = 0; // after ')' expect binary operator or another ')'
            continue;
        }

        // Operators (including unary + / -)
        if (strchr("+-*/^", input[i])) {
            char raw = input[i++];
            char op;
            if ((raw == '+' || raw == '-') && expect_unary) {
                // treat as unary
                op = (raw == '-') ? 'u' : 'p';
            } else {
                op = raw; // binary operator
            }

            // pop operators from stack to output while they have higher precedence
            while (stack_top >= 0 && is_operator_char(stack[stack_top]) &&
                  ((precedence(stack[stack_top]) > precedence(op)) ||
                   (precedence(stack[stack_top]) == precedence(op) && !is_right_assoc(op)))) {
                output[*out_count].type = 'o';
                output[*out_count].op = stack[stack_top--];
                (*out_count)++;
            }

            stack[++stack_top] = op;
            expect_unary = 1; // after an operator, next + or - could be unary
            continue;
        }

        fprintf(stderr, "Error: invalid character '%c'\n", input[i]);
        exit(1);
    }

    while (stack_top >= 0) {
        if (stack[stack_top] == '(' || stack[stack_top] == ')') {
            fprintf(stderr, "Error: mismatched parentheses\n");
            exit(1);
        }
        output[*out_count].type = 'o';
        output[*out_count].op = stack[stack_top--];
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
            char op = output[i].op;
            if (op == 'u' || op == 'p') {
                // unary: need one operand
                if (top < 0) { fprintf(stderr, "Error: insufficient operands for unary operator\n"); exit(1); }
                double v = stack[top--];
                if (op == 'u') stack[++top] = -v;
                else stack[++top] = +v; // unary plus: no-op
            } else {
                // binary ops
                if (top < 1) { fprintf(stderr, "Error: insufficient operands\n"); exit(1); }
                double b = stack[top--];
                double a = stack[top--];
                switch (op) {
                    case '+': stack[++top] = a + b; break;
                    case '-': stack[++top] = a - b; break;
                    case '*': stack[++top] = a * b; break;
                    case '/': stack[++top] = a / b; break;
                    case '^': stack[++top] = pow(a, b); break;
                    default:
                        fprintf(stderr, "Error: unknown operator '%c'\n", op);
                        exit(1);
                }
            }
        }
    }

    if (top != 0) { fprintf(stderr, "Error: invalid expression\n"); exit(1); }
    return stack[0];
}

void add_number(const char *input, int *i, Token *output, int *out_count) {
    char buffer[64];  

    while (isdigit(input[*i]) || input[*i] == '.' || input[*i] == 'e' || input[*i] == 'E') {
    buffer[(*i)++] = input[(*i)++];
}
    buffer[(*i)] = '\0';
    output[(*out_count)].type = 'n';
    output[(*out_count)].value = atof(buffer);   
    (*out_count)++;

}