//again, AI was helpful...
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

    wprintf(L"'exit' or 'quit' to exit\n");
    while(1){
        wprintf(L"\nEnter expression: ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0';
        if (strcmp(input, "exit") == 0 || strcmp(input, "quit") == 0) {
            wprintf(L"Exiting...\n");
            return 0;
        }
        if(!strchr(input, '=')) {wprintf(L"Syntax error (no '=' sign)\n"); continue;}
        
        char *left, *right;
        split(input, &left, &right);
        
        Poly output_left[MAX_TOKENS] = {0}, output_right[MAX_TOKENS] = {0};
        int out_count_left = 0, out_count_right = 0;

        algebra_parser(left, output_left, &out_count_left);
        algebra_parser(right, output_right, &out_count_right);

        //Demo output
        for(int i = 0; i < out_count_left; i++){
            if(output_left[i].type == 'a'){
                wprintf(L"Left Term %d: Coeff: %.2f, Var: %s, Exp: %d\n", i+1, output_left[i].coeff, output_left[i].var, output_left[i].exponent);
            } else if(output_left[i].type == 'n'){
                wprintf(L"Left Term %d: Constant: %.2f\n", i+1, output_left[i].constant);
            } else if(output_left[i].type == 'o'){
                wprintf(L"Left Term %d: Operator: %c\n", i+1, output_left[i].op);
            }
        }
        for(int i = 0; i < out_count_right; i++){
            if(output_right[i].type == 'a'){
                wprintf(L"Right Term %d: Coeff: %.2f, Var: %s, Exp: %d\n", i+1, output_right[i].coeff, output_right[i].var, output_right[i].exponent);
            } else if(output_right[i].type == 'n'){
                wprintf(L"Right Term %d: Constant: %.2f\n", i+1, output_right[i].constant);
            } else if(output_right[i].type == 'o'){
                wprintf(L"Right Term %d: Operator: %c\n", i+1, output_right[i].op);
            }
        }

    }
}
void split(char input[], char **left, char **right) {
    char *equal = strchr(input, '=');
    if (!equal) {
        *left = NULL;
        *right = NULL;
        return;
    }

    *equal = '\0';
    *left = input;
    *right = equal + 1;
}


void algebra_parser(const char input[], Poly output[], int *out_count) {
    char stack[MAX_TOKENS];
    int stack_top = -1;
    *out_count = 0;
    int i = 0;
    int expect_unary = 1;

    while (input[i] != '\0') {
        if (isspace((unsigned char)input[i])) { i++; continue; }

        // Check for pi
        if (up_low(input+i, "pi") == 0) {
            output[*out_count].type = 'n';
            output[*out_count].constant = PI;
            output[*out_count].coeff = 0;
            output[*out_count].exponent = 0;
            output[*out_count].var[0] = '\0';
            (*out_count)++;
            i += 2;
            expect_unary = 0;
            continue;
        }

        // Parentheses
        if (input[i] == '(') {
            stack[++stack_top] = input[i++];
            expect_unary = 1;
            continue;
        }
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

        // Insert implicit multiplication
        char prev = (i > 0) ? input[i-1] : ' ';
        if ((isalnum(prev) || prev == '.' || prev == ')') &&
            (isalpha(input[i]) || input[i] == '(')) {
            // treat as '*'
            char op = '*';
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

        // Operators
        if (strchr("+-*/^", input[i])) {
            char raw = input[i++];
            char op = ((raw == '+' || raw == '-') && expect_unary)
                        ? ((raw == '-') ? 'u' : 'p') // unary
                        : raw;

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

        // Numbers and variables
        if (isdigit((unsigned char)input[i]) || isalpha((unsigned char)input[i]) || input[i] == '.') {
            char num[64]; int j = 0;

            // numeric part (coeff)
            while ((isdigit((unsigned char)input[i]) || input[i] == '.') && j < (int)sizeof(num)-1)
                num[j++] = input[i++];
            num[j] = '\0';
            double value = (j > 0) ? atof(num) : 1.0;

            // variable part
            if (isalpha((unsigned char)input[i])) {
                int v = 0;
                output[*out_count].type = 'a';
                while (isalpha((unsigned char)input[i]) && v < (int)sizeof(output[*out_count].var)-1)
                    output[*out_count].var[v++] = input[i++];
                output[*out_count].var[v] = '\0';

                // exponent
                if (input[i] == '^') {
                    i++;
                    char exp_str[16]; int k = 0;
                    while ((isdigit((unsigned char)input[i]) || input[i] == '.') && k < (int)sizeof(exp_str)-1)
                        exp_str[k++] = input[i++];
                    exp_str[k] = '\0';
                    output[*out_count].exponent = (k > 0) ? atof(exp_str) : 1;
                } else {
                    output[*out_count].exponent = 1;
                }

                output[*out_count].coeff = value;
                output[*out_count].constant = 0;
                (*out_count)++;
                expect_unary = 0;
                continue;
            } 
            // No variable (plain number)
            else {
                output[*out_count].type = 'n';
                output[*out_count].var[0] = '\0';
                output[*out_count].coeff = 0;
                output[*out_count].constant = value;
                output[*out_count].exponent = 0;
                (*out_count)++;
                expect_unary = 0;
                continue;
            }
        }

        // What is this?
        fprintf(stderr, "Error: invalid character '%c'\n", input[i]);
        exit(1);
    }

    // Clean up crew
    while (stack_top >= 0) {
        output[*out_count].type = 'o';
        output[*out_count].op = stack[stack_top--];
        (*out_count)++;
    }
}


void subtract_poly(Poly lhs[MAX_TOKENS], int lhs_count, Poly rhs[MAX_TOKENS], int rhs_count) {
    int i = 0;
    double bigest = -INFINITY;
    while (rhs_count > 0 && lhs_count > 0) {
        if(rhs[i].type == 'a') {
            for (int j = 0; j < rhs_count; j++) {
                if (rhs[j].exponent > bigest) bigest = rhs[j].exponent;
            }
                for(int k = 0; k < (lhs_count&&rhs_count); k++){
                    if(strcmp(lhs[k].var, rhs[i].var) == 0 && lhs[k].exponent == rhs[i].exponent){
                        lhs[k].coeff -= rhs[i].coeff;
                        // Remove rhs term
                        for(int m = i; m < rhs_count - 1; m++){
                            rhs[m] = rhs[m + 1];
                        }
                }
            } 
            i++;
            rhs_count--;
        }
        if(rhs[i].type == 'n'){
            i = 0;
            for(int n = 0; n < rhs_count; n++){                
                if(bigest == 0 || bigest == 1){
                    rhs[i].constant = -lhs[i].constant;
                    i++;
                }
                else if(bigest > 1){
                    lhs[i].constant = -rhs[i].constant;
                    i++;
                }
                lhs_count -= i;
            }
        }
    }
}
    