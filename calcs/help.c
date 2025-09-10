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

double pretty_print(double num, wchar_t unit) {
    int mult = 0;
    int sign = (num < 0.0) ? -1 : 1;
    num = (num < 0) ? -num : num;

    while (num >= 1000.0 || (num < 1.0 && num != 0.0)) {
        if (num >= 1000.0) { num /= 1000.0; mult++; }
        else if (num < 1.0) { num *= 1000.0; mult--; }
    }

    num *= sign;

    const wchar_t *prefixes[] = { L"f", L"p", L"n", L"µ", L"m", L"", L"k", L"M", L"G", L"T", L"P" };
    int index = mult + 5;
    if (index >= 0 && index <= 10) {
       wprintf(L"%.5f%ls%lc", num, prefixes[index], unit);
    } else {
        wprintf(L"%.5fe%d%lc", num, mult * 3, unit);
        
    }   
    return num;
}
void print_results(Outputs *out, int type, int parallel) {
       wchar_t ohm = L'Ω', phi = L'φ', rad = L'°';

    wprintf(L"V  = ");    pretty_print(out->V, 'V'); wprintf(L"\n");
    wprintf(L"f  = ");    pretty_print(out->f, 'H'); wprintf(L"z\n");
    wprintf(L"R  = ");    pretty_print(out->R, ohm); wprintf(L"\n");

    if (type == 2 || type == 3) {
        wprintf(L"C  = ");  pretty_print(out->C, 'F'); wprintf(L"\n");
        wprintf(L"Xc = ");  pretty_print(out->Xc, ohm); wprintf(L"\n");
    }
    if (type == 1 || type == 3) {
        wprintf(L"L  = "); pretty_print(out->L, 'H'); wprintf(L"\n");
        wprintf(L"Xl = "); pretty_print(out->Xl, ohm); wprintf(L"\n");
    }

    if (parallel == 1) {
        wprintf(L"G  = "); pretty_print(out->G, 'S'); wprintf(L"\n");

        if (type == 2 || type == 3) {
            wprintf(L"Bc = "); pretty_print(out->Bc, 'S'); wprintf(L"\n");
        }
        if (type == 1 || type == 3) {
            wprintf(L"Bl = "); pretty_print(out->Bl, 'S'); wprintf(L"\n");
        }
    }

    wprintf(L"Z = ");   pretty_print(out->Z, ohm); wprintf(L"\n");
    wprintf(L"I = ");   pretty_print(out->I, 'A'); wprintf(L"\n");
    wprintf(L"S = ");   pretty_print(out->S, 'V'); wprintf(L"A");  wprintf(L"\n");
    wprintf(L"Q = ");   pretty_print(out->Q, 'V'); wprintf(L"Ar"); wprintf(L"\n");
    wprintf(L"P = ");   pretty_print(out->P, 'W'); wprintf(L"\n");
    wprintf(L"%lc = ", phi); wprintf(L"%.5f%lc", out->phi, rad);   wprintf(L"\n");
}

int up_low(const char *s1, const char *s2) {
    while (*s1 && *s2) {
        char c1 = tolower((unsigned char)*s1);
        char c2 = tolower((unsigned char)*s2);
        if (c1 != c2) return c1 - c2;
        s1++;
        s2++;
    }
    return (unsigned char)*s1 - (unsigned char)*s2;
}

Type value_of(char buffer[]) {
    Type rr = {0};
    const char *tokens[4] = { "V", "R", "I", "P" };
    Token output[MAX_TOKENS];
    int out_count;

    for (int i = 0; i < 4; i++) {
        char *token = (i == 0) ? strtok(buffer, ",") : strtok(NULL, ",");
        if (!token) break;
        double value;
        while (isspace((unsigned char)*token)) token++;

        char *equal = strchr(token, '=');
        if (!equal) continue;
        
        *equal = '\0';
        char *var = token;
        char *expr = equal + 1;

        while (isspace(*var)) var++;
        char *end = var + strlen(var) - 1;
        while (isspace(*expr)) expr++;
        while (end > var && isspace((unsigned char)*end)) {
            *end = '\0';
            end--;
        }
        while(isspace((unsigned char)*expr)) expr++;
        if(up_low(expr, "pi") == 0) {
            value = PI;
        } else if (up_low(expr, "C") == 0) {
            value = LIGHT;
        } else {
        shunting_yard(expr, output, &out_count);
        value = evaluate_postfix(output, out_count);
        
        }
        if (up_low(var, "V") == 0) rr.voltage = value;
        else if (up_low(var, "R") == 0) rr.resistance = value;
        else if (up_low(var, "I") == 0) rr.current = value;
        else if (up_low(var, "P") == 0) rr.power = value;
    }

    return rr;
}

Inputs parseInputs(char buffer[], const char *tokens[], int count, int type, int parallel) {
    Inputs rr = {0};
    Token output[MAX_TOKENS];
    int out_count;

    for (int i = 0; i < count; i++) {
        char *token = (i == 0) ? strtok(buffer, ",") : strtok(NULL, ",");
        if (!token) break;
        double value;
        while (isspace((unsigned char)*token)) token++;

        char *equal = strchr(token, '=');
        if (!equal) continue;
        
        *equal = '\0';
        char *var = token;
        char *expr = equal + 1;

        while (isspace(*var)) var++;
        char *end = var + strlen(var) - 1;
        while (isspace(*expr)) expr++;
        while (end > var && isspace((unsigned char)*end)) {
            *end = '\0';
            end--;
        }
        while(isspace((unsigned char)*expr)) expr++;
        if(up_low(expr, "pi") == 0) {
            value = PI;
        } else if (up_low(expr, "C") == 0) {
            value = LIGHT;
        } else {
        shunting_yard(expr, output, &out_count);
        value = evaluate_postfix(output, out_count);

        if(up_low(var, "V") == 0) rr.V = value;
        else if(up_low(var, "f") == 0) rr.f = value;
        else if(up_low(var, "R") == 0) rr.R = value;
        else if(up_low(var, "L") == 0 && (type == 1 || type == 3)) rr.L = value;
        else if(up_low(var, "Xl") == 0 && (type == 1 || type == 3)) rr.Xl = value;
        else if(up_low(var, "C") == 0 && (type == 2 || type == 3)) rr.C = value;
        else if(up_low(var, "Xc") == 0 && (type == 2 || type == 3)) rr.Xc = value;
        else if(up_low(var, "Z") == 0) rr.Z = value;
        else if(up_low(var, "I") == 0) rr.I = value;
        else if(up_low(var, "S") == 0) rr.S = value;
        else if(up_low(var, "Q") == 0) rr.Q = value;
        else if(up_low(var, "P") == 0) rr.P = value;
        else if(up_low(var, "phi") == 0) rr.phi = value;
        else if(up_low(var, "Bl") == 0 && parallel == 1 && (type == 1 || type == 3)) rr.Bl = value;
        else if(up_low(var, "Bc") == 0 && parallel == 1 && (type == 2 || type == 3)) rr.Bc = value;
        else if(up_low(var, "G") == 0 && parallel == 1) rr.G = value;
        else if(up_low(var, "Y") == 0 && parallel == 1) rr.Y = value;
        
    }
        /*(type == 1 && parallel == 0) {
            // ----- Series RL -----
            // V | f | R | L | Xl | Z | I | S | Q | P | phi
        
        (type == 1 && parallel == 1) {
            // ----- Parallel RL -----
            // V | f | R | L | Xl | Bl | Z | Y | I | S | Q | P | phi
       
        type == 2 && parallel == 0) {
            // ----- Series RC -----
            // V | f | R | C | Xc | Z | I | S | Q | P | phi
      
       type == 2 && parallel == 1) {
            // ----- Parallel RC -----
            // V | f | R | C | Xc | Bc | Z | Y | I | S | Q | P | phi
         
        (type == 3 && parallel == 0) {
            // ----- Series RLC -----
            // V | f | R | C | Xc | L | Xl | Z | I | S | Q | P | phi
            
        
        (type == 3 && parallel == 1) {
            // ----- Parallel RLC -----
            // V | f | R | G | C | Xc | Bc | L | Xl | Bl | Z | Y | I | S | Q | P | phi
        */
    }

    return rr;
}

Inputs RLsCalc(char buffer[]) {
    const char *tokens[] = { "V", "f", "R", "L", "Xl", "Z", "I", "S", "Q", "P", "phi" };
    return parseInputs(buffer, tokens, 11, 1, 0);
}

Inputs RCsCalc(char buffer[]) {
    const char *tokens[] = { "V", "f", "R", "C", "Xc", "Z", "I", "S", "Q", "P", "phi" };
    return parseInputs(buffer, tokens, 11, 2, 0);
}

Inputs RLpCalc(char buffer[]) {
    const char *tokens[] = { "V", "f", "R", "G", "L", "Xl", "Bl", "Z", "Y", "I", "S", "Q", "P", "phi" };
    return parseInputs(buffer, tokens, 14, 1, 1);
}

Inputs RCpCalc(char buffer[]) {
    const char *tokens[] = { "V", "f", "R", "G", "C", "Xc", "Bc", "Z", "Y", "I", "S", "Q", "P", "phi" };
    return parseInputs(buffer, tokens, 14, 2, 1);
}

Inputs RLCsCalc(char buffer[]) {
    const char *tokens[] = { "V", "f", "R", "C", "Xc", "L", "Xl", "Z", "I", "S", "Q", "P", "phi" };
    return parseInputs(buffer, tokens, 13, 3, 0);
}

Inputs RLCpCalc(char buffer[]) {
    const char *tokens[] = { "V", "f", "R", "G", "C", "Xc", "Bc", "L", "Xl", "Bl", "Z", "Y", "I", "S", "Q", "P", "phi" };
    return parseInputs(buffer, tokens, 17, 3, 1);
}

