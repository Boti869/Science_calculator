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
        char *input1, *input2;
        if (!split(input, &input1, &input2)) {
            wprintf(L"Syntax error, no equal sign\n");
            continue;
}
        if(!input1 || !input2){wprintf(L"Syntax error\n"); continue;}
        Term output[MAX_TOKENS];
        int out_count;
        algebra_parser(input1, output, &out_count);
        Poly result_L = evaluate_algebra(output, out_count);

        algebra_parser(input2, output, &out_count);
        Poly result_R = evaluate_algebra(output, out_count);

        solve_equation(result_L, result_R);
        // Compare result_L and result_R
        if (result_L.coeff != result_R.coeff && result_L.exponent != result_R.exponent && result_L.is_const != result_R.is_const) {
            wprintf(L"Left and right sides are not equal\n");
        }
        
    }

}
int split(char input[], char **left, char **right) {
    char *equal = strchr(input, '=');
    if (!equal) return 0; // no '='

    *equal = '\0';
    *left = input;
    *right = equal + 1;
    return 1;
}

void algebra_parser(const char input[], Term output[], int *out_count) {
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
            output[*out_count].constant = PI;
            (*out_count)++;
            i += 2;      // skip the 'pi' characters
            expect_unary = 0;
            continue;
        }
        char num[64];
        int j = 0;    
        // Numbers and letters (variables)
        while (isdigit((unsigned char)input[i]) && j < (int)sizeof(num)-1)
            num[j++] = input[i++];
        if (input[i] == '.') {
            num[j++] = input[i++];
            while (isdigit((unsigned char)input[i]) && j < (int)sizeof(num)-1)
                num[j++] = input[i++];
        }
        num[j] = '\0';

        double value = (j > 0) ? atof(num) : 1.0; // default coeff = 1 if just "x"

        // check for pi
        if (up_low(input+i, "pi") == 0) {
            value *= PI;
            output[*out_count].type = 'n';
            output[*out_count].constant = value;
            (*out_count)++;
            i += 2;
            expect_unary = 0;
            continue;
        }
        int v = 0;
        // variable case
        if (isalpha((unsigned char)input[i])) {
            output[*out_count].type = 'a';
            output[*out_count].var[v] = input[i];
            i++;
            v++;

            // check for exponent
            if (input[i] == '^') {
                i++; // skip '^'
                char exp[10];
                int k = 0;
                while (isdigit((unsigned char)input[i]) && k < (int)sizeof(exp)-1)
                    exp[k++] = input[i++];
                exp[k] = '\0';
                output[*out_count].exponent = (k > 0) ? atof(exp) : 1;
            } else {
                output[*out_count].exponent = 1;
            }

            output[*out_count].coeff = value;
            (*out_count)++;
            expect_unary = 0;
            continue;
}
        // no variable
        else {
            output[*out_count].type = 'n';
            output[*out_count].var[v] = 0;
            output[*out_count].coeff = 0;
            output[*out_count].constant = value;
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

        // Implicit multiplication (e.g., 2x or 3(x+1))
        char prev = (i > 0) ? input[i-1] : ' ';
            if ((isalnum(prev) || prev == ')') &&
            (isalpha(input[i]) || input[i] == '(')) {
            // insert '*'
            output[*out_count].type = 'o';
            output[*out_count].op = '*';
            (*out_count)++;
            continue;
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

int collect_terms(Poly polys[], int *polyCount, Term t) {
    for (int i = 0; i < *polyCount; i++) {
        if (strcmp(polys[i].var, t.var) == 0 && polys[i].exponent == t.exponent) {
            polys[i].coeff += t.coeff;
            return 0;
        }
    }
    // New term
    strcpy(polys[*polyCount].var, t.var);
    polys[*polyCount].exponent = t.exponent;
    polys[*polyCount].coeff = t.coeff;
    (*polyCount)++;
    return 0;
}
Poly make_const(double c) {
    Poly p = { .coeff = c, .var = "", .exponent = 0, .is_const = 1 };
    return p;
}

Poly make_var(double coeff, const char *var, int exp) {
    Poly p;
    p.coeff = coeff;
    strcpy(p.var, var);
    p.exponent = exp;
    p.is_const = 0;
    return p;
}

// Addition
Poly add_poly(Poly a, Poly b) {
    Poly res = {0};
    if (a.is_const && b.is_const) {
        res = make_const(a.coeff + b.coeff);
    } else if (!a.is_const && !b.is_const &&
               strcmp(a.var, b.var) == 0 && a.exponent == b.exponent) {
        // Same variable and exponent → combine coefficients
        res = make_var(a.coeff + b.coeff, a.var, a.exponent);
    } else {
        fprintf(stderr, "Error: cannot add different terms\n");
        exit(1);
    }
    return res;
}

// Multiplication
Poly mul_poly(Poly a, Poly b) {
    Poly res = {0};
    if (a.is_const && b.is_const) {
        res = make_const(a.coeff * b.coeff);
    } else if (a.is_const && !b.is_const) {
        res = make_var(b.coeff * a.coeff, b.var, b.exponent);
    } else if (!a.is_const && b.is_const) {
        res = make_var(a.coeff * b.coeff, a.var, a.exponent);
    } else if (!a.is_const && !b.is_const && strcmp(a.var, b.var) == 0) {
        // x^m * x^n = x^(m+n)
        res = make_var(a.coeff * b.coeff, a.var, a.exponent + b.exponent);
    } else {
        fprintf(stderr, "Error: multiplication with different vars not supported yet\n");
        exit(1);
    }
    return res;
}

// Exponentiation (only supports var^const or const^const here)
Poly pow_poly(Poly base, Poly exp) {
    if (!exp.is_const) {
        fprintf(stderr, "Error: non-constant exponent not supported\n");
        exit(1);
    }
    if (base.is_const) {
        return make_const(pow(base.coeff, exp.coeff));
    } else {
        // (a*x^n)^m = a^m * x^(n*m)
        double coeff_pow = pow(base.coeff, exp.coeff);
        int new_exp = base.exponent * (int)exp.coeff;
        return make_var(coeff_pow, base.var, new_exp);
    }
}
// New evaluate_algebra: reduce Terms -> single Poly
Poly evaluate_algebra(Term terms[], int n) {
    Poly result = make_const(0);
    for (int i = 0; i < n; i++) {
        if (terms[i].type == 'n') {
            result = add_poly(result, make_const(terms[i].constant));
        } else if (terms[i].type == 'a') {
            result = add_poly(result, make_var(terms[i].coeff, terms[i].var, terms[i].exponent));
        } else {
            fprintf(stderr, "Error: operators not fully supported in demo\n");
            exit(1);
        }
    }
    return result;
}
void solve_equation(Poly left, Poly right) {
    // Move everything to LHS
    if (right.is_const) {
        left = add_poly(left, make_const(-right.coeff));
    } else if (!right.is_const) {
        // Only supports one variable demo
        left = add_poly(left, make_var(-right.coeff, right.var, right.exponent));
    }

    // Handle linear
    if (!left.is_const && left.exponent == 1) {
        double a = left.coeff;
        double b = 0;
        printf("Solution: %s = %.4f\n", left.var, -b / a);
    }
    // Handle quadratic with const
    else if (!left.is_const && left.exponent == 2) {
        double a = left.coeff;
        double b = 0;
        double c = 0;
        double disc = b*b - 4*a*c;
        if (disc < 0) {
            printf("No real solutions.\n");
        } else {
            double r1 = (-b + sqrt(disc)) / (2*a);
            double r2 = (-b - sqrt(disc)) / (2*a);
            printf("Solutions: %s = %.4f, %.4f\n", left.var, r1, r2);
        }
    } else {
        printf("Equation too complex for demo.\n");
    }
}