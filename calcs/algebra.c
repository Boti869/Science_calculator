#include"../header_files/algebra.h"
#include"../header_files/constants.h"
#include"../header_files/calc.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<math.h>
#include<locale.h>
#include<wchar.h>

int algebraCalc(){
    char input[MAX_TOKENS];
    int iter;

    wprintf(L"'exit' or 'quit' to exit\n");
    while(1){
        wprintf(L"\nEnter expression: ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0';
        if (strcmp(input, "exit") == 0 || strcmp(input, "quit") == 0) {
            wprintf(L"Exiting...\n");
            return 0;
        }
        char *input1 = split(input, 0);
        char *input2 = split(input, 1);
        if(!input1 || !input2){wprintf(L"Syntax error\n"); continue;}
        Token output[MAX_TOKENS];
        int out_count;
        algebra_parser(input1, output, &out_count);
        double result_L = evaluate_algebra(output, out_count);
        algebra_parser(input2, output, &out_count);
        double result_R = evaluate_algebra(output, out_count);

        if(result_L != result_R) wprintf(L"Equasion is not true\n");
        
    }

}
char* split(char input[], int iter) {
    char *equal = strchr(input, '=');
    if(!equal) {wprintf(L"Syntax error, no equal sign\n"); return NULL;}
    *equal = '\0';
    
    if(iter == 0){    
        return input; //left side
    } 
    else if(iter == 1){
       return equal + 1; //right side
    } 
    else {return NULL;} //I messed up
}

void algebra_parser(const char input[], Token output[], int *out_count) {
    char stack[MAX_TOKENS];
    int stack_top = -1;
    *out_count = 0;
    int i = 0;
    int expect_unary = 1;
    
    while (input[i] != '\0') {
        if (isspace((unsigned char)input[i])) { i++; continue; }

        // check for pi
        if (up_low(input+i, "pi") == 0) {
            output[*out_count].type = 'n';
            output[*out_count].value = PI;
            (*out_count)++;
            i += 2;      // skip the 'pi' characters
            expect_unary = 0;
            continue;
        }

        // Numbers
        if (isdigit((unsigned char)input[i]) || input[i] == '.') {
                    char num[64];
                    int j = 0;
                    double multiplier = 1.0;

                    // integer part
                    while (isdigit((unsigned char)input[i]) && j < (int)sizeof(num)-1) num[j++] = input[i++];
                    // fractional
                    if (input[i] == '.') { num[j++] = input[i++]; while (isdigit((unsigned char)input[i]) && j < (int)sizeof(num)-1) num[j++] = input[i++]; }

                    num[j] = '\0';
                    output[*out_count].type = 'n';
                    output[*out_count].value = atof(num);
                    (*out_count)++;
                    expect_unary = 0;
                    continue;
        }

        // Letters
        if (isalpha((unsigned char)input[i])) {
            output[*out_count].type = 'a';
            output[*out_count].algebra = input[i];
            i++;
            (*out_count)++;
            expect_unary = 0;
            continue;
        }
    
        // Parentheses
        if (input[i] == '(') { stack[++stack_top] = input[i++]; expect_unary = 1; continue; }
        if (input[i] == ')') {
            while (stack_top >= 0 && stack[stack_top] != '(') {
                output[*out_count].type = 'o';
                output[*out_count].op = stack[stack_top--];
                (*out_count)++;
            }
            if (stack_top < 0) { fprintf(stderr, "Error: mismatched parentheses\n"); exit(1); }
            stack_top--; // pop '('
            i++;
            expect_unary = 0;
            continue;
        }

        char prev = (i > 0) ? input[i-1] : ' ';
            if ((isalnum(prev) || prev == ')') &&
            (isalpha(input[i]) || input[i] == '(')) {
            // insert '*'
            output[*out_count].type = 'o';
            output[*out_count].op = '*';
            (*out_count)++;
        }

        // Operators
        if (strchr("+-*/^", input[i])) {
            char raw = input[i++];
            char op;
            if ((raw == '+' || raw == '-') && expect_unary) op = (raw == '-') ? 'u' : 'p';
            else op = raw;

            while (stack_top >= 0 && is_operator_char(stack[stack_top]) &&
                  ((precedence(stack[stack_top]) > precedence(op)) ||
                   (precedence(stack[stack_top]) == precedence(op) && !is_right_assoc(op)))) {
                output[*out_count].type = 'o';
                output[*out_count].op = stack[stack_top--];
                (*out_count)++;
            }
            stack[++stack_top] = op;
            expect_unary = 1;
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
double evaluate_algebra(Token output[], int out_count) {
double stack[MAX_TOKENS];
    int top = -1;

    for (int i = 0; i < out_count; i++) {
        if (output[i].type == 'n') {
            stack[++top] = output[i].value;
        } else if (output[i].type == 'a') {
            stack[++top] = output[i].algebra;
        } else if (output[i].type == 'o') {
            char op = output[i].op;
            if (op == 'u' || op == 'p') {
                if (top < 0) { fprintf(stderr, "Error: insufficient operands for unary operator\n"); exit(1); }
                double v = stack[top--];
                stack[++top] = (op == 'u') ? -v : +v;
            } else {
                if (top < 1) { fprintf(stderr, "Error: insufficient operands\n"); exit(1); }
                double b = stack[top--];
                double a = stack[top--];
                switch (op) {
                    case '+': stack[++top] = a + b; break;
                    case '-': stack[++top] = a - b; break;
                    case '*': stack[++top] = a * b; break;
                    case '/': stack[++top] = a / b; break;
                    case '^': stack[++top] = pow(a, b); break;
                    default: fprintf(stderr, "Error: unknown operator '%c'\n", op); exit(1);
                }
            }
        }
    }

    if (top != 0) { fprintf(stderr, "Error: invalid expression\n"); exit(1); }
    return stack[0];
}