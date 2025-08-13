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

    Token output[MAX_TOKENS];
    int out_count;
    shunting_yard(expr, output, &out_count);

    double result = evaluate_postfix(output, out_count);
    pretty_print(result, '\0', 'n');

    return 0;
}
