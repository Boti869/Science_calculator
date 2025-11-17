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
    wprintf(L"I = ");   pretty_print(out->I, 'A'); wprintf(L"\n");

    if(parallel == 0){wprintf(L"Z  = "); pretty_print(out->Z, ohm); wprintf(L"\n");}
    else{wprintf(L"Y  = "); pretty_print(out->Y, 'S'); wprintf(L"\n");}
    
    wprintf(L"f  = ");    pretty_print(out->f, 'H'); wprintf(L"z\n");
    
    wprintf(L"R  = ");    pretty_print(out->R, ohm); wprintf(L"\n");
    if(parallel == 0){wprintf(L"Vr = "); pretty_print(out->Vr, 'V'); wprintf(L"\n");}
    else{
        wprintf(L"G  = "); pretty_print(out->G, 'S'); wprintf(L"\n");
        wprintf(L"Ir = "); pretty_print(out->Ir, 'A'); wprintf(L"\n");
    }
   
    if (type == 1 || type == 3) {
        wprintf(L"L  = "); pretty_print(out->L, 'H'); wprintf(L"\n");
        wprintf(L"Xl = "); pretty_print(out->Xl, ohm); wprintf(L"\n");
        if(parallel == 0){
            wprintf(L"Vl = "); pretty_print(out->Vl, 'V'); wprintf(L"\n");
        }
        else{
            wprintf(L"Bl = "); pretty_print(out->Bl, 'S'); wprintf(L"\n");
            wprintf(L"Il = "); pretty_print(out->Il, 'A'); wprintf(L"\n");
        }
    }
    if (type == 2 || type == 3) {
        wprintf(L"C  = "); pretty_print(out->C, 'F'); wprintf(L"\n");
        wprintf(L"Xc = "); pretty_print(out->Xc, ohm); wprintf(L"\n");
        if(parallel == 0){
            wprintf(L"Vc = "); pretty_print(out->Vc, 'V'); wprintf(L"\n");
        }
        else{
            wprintf(L"Bc = "); pretty_print(out->Bc, 'S'); wprintf(L"\n");
            wprintf(L"Ic = "); pretty_print(out->Ic, 'A'); wprintf(L"\n");
        }
    }

    wprintf(L"S = ");   pretty_print(out->S, 'V'); wprintf(L"A");  wprintf(L"\n");
    wprintf(L"P = ");   pretty_print(out->P, 'W'); wprintf(L"\n");
    wprintf(L"Q = ");   pretty_print(out->Q, 'V'); wprintf(L"Ar"); wprintf(L"\n");
    wprintf(L"%lc = ", phi); wprintf(L"%.5f%lc", out->phi, rad);
        wprintf(L"= %d%lc %d' %d\"%lc \n", out->phi_deg, rad, out->phi_min, out->phi_sec); 
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

Inputs parseInputs(char buffer[], int count, int type, int parallel) {
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
        } else {
        shunting_yard(expr, output, &out_count);
        value = evaluate_postfix(output, out_count);

        if(up_low(var, "V") == 0) rr.V = value;
        else if(up_low(var, "vr") == 0 && parallel == 0) rr.Vr = value;
        else if(up_low(var, "vl") == 0 && parallel == 0 && (type == 1 || type == 3)) rr.Vl = value;
        else if(up_low(var, "vc") == 0 && parallel == 0 && (type == 2 || type == 3)) rr.Vc = value;
        else if(up_low(var, "f") == 0) rr.f = value;
        else if(up_low(var, "R") == 0) rr.R = value;
        else if(up_low(var, "L") == 0 && (type == 1 || type == 3)) rr.L = value;
        else if(up_low(var, "Xl") == 0 && (type == 1 || type == 3)) rr.Xl = value;
        else if(up_low(var, "C") == 0 && (type == 2 || type == 3)) rr.C = value;
        else if(up_low(var, "Xc") == 0 && (type == 2 || type == 3)) rr.Xc = value;
        else if(up_low(var, "Z") == 0 && parallel == 0) rr.Z = value;
        else if(up_low(var, "I") == 0) rr.I = value;
        else if(up_low(var, "ir") == 0 && parallel == 1) rr.Ir = value;
        else if(up_low(var, "il") == 0 && parallel == 1 && (type == 1 || type == 3)) rr.Il = value;
        else if(up_low(var, "ic") == 0 && parallel == 1 && (type == 2 || type == 3)) rr.Ic = value;
        else if(up_low(var, "S") == 0) rr.S = value;
        else if(up_low(var, "Q") == 0) rr.Q = value;
        else if(up_low(var, "P") == 0) rr.P = value;
        else if(up_low(var, "phi") == 0) rr.phi = value;
        else if(up_low(var, "Bl") == 0 && parallel == 1 && (type == 1 || type == 3)) rr.Bl = value;
        else if(up_low(var, "Bc") == 0 && parallel == 1 && (type == 2 || type == 3)) rr.Bc = value;
        else if(up_low(var, "G") == 0 && parallel == 1) rr.G = value;
        else if(up_low(var, "Y") == 0 && parallel == 1) rr.Y = value;
        
        }
    }
    return rr;
}

Inputs RLsCalc(char buffer[]) {
    return parseInputs(buffer, 13, 1, 0);
}

Inputs RCsCalc(char buffer[]) {
    return parseInputs(buffer, 13, 2, 0);
}

Inputs RLpCalc(char buffer[]) {
    return parseInputs(buffer, 15, 1, 1);
}

Inputs RCpCalc(char buffer[]) {
    return parseInputs(buffer, 15, 2, 1);
}

Inputs RLCsCalc(char buffer[]) {
    return parseInputs(buffer, 16, 3, 0);
}

Inputs RLCpCalc(char buffer[]) {
    return parseInputs(buffer, 19, 3, 1);
}

