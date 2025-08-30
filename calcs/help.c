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

    while (num >= 1000.0 || num < 1.0 && num != 0.0) {
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
    wprintf(L"V = ");    
    pretty_print(out->V, 'V');
    wprintf(L"\n");
    wprintf(L"f = ");
    pretty_print(out->f, 'H');
    wprintf(L"z");
    wprintf(L"\n");
    wprintf(L"R = ");
    wchar_t ohm = L'Ω';
    pretty_print(out->R, ohm);
    wprintf(L"\n");
    
    switch (type){ 
        case 1:
        wprintf(L"L = ");
            pretty_print(out->L, 'H'); break;
        case 2: wprintf(L"C = ");
            pretty_print(out->C, 'F'); break;
    }
    wprintf(L"\n");
    switch (type){ 
        case 1: wprintf(L"Xl = ");
            pretty_print(out->Xl, ohm); break;
        case 2: wprintf(L"Xc = ");
            pretty_print(out->Xc, ohm); break;
    }
    switch(parallel) {
        case 1:
            switch (type){
                case 1: wprintf(L"\nBl = ");
                    pretty_print(out->Bl, 'S'); break;
                case 2: wprintf(L"\nBc = ");
                    pretty_print(out->Bc, 'S'); break;
            } break;
    }
    wprintf(L"\n");
    wprintf(L"Z = ");
    pretty_print(out->Z, ohm);
    wprintf(L"\n");
    wprintf(L"I = ");
    pretty_print(out->I, 'A');
    wprintf(L"\n");
    wprintf(L"S = ");
    pretty_print(out->S, 'V');
    wprintf(L"A");
    wprintf(L"\n");
    wprintf(L"Q = ");
    pretty_print(out->Q, 'V');
    wprintf(L"A");
    wprintf(L"r");
    wprintf(L"\n");
    wprintf(L"P = ");
    pretty_print(out->P, 'W');
    wprintf(L"\n");
    wchar_t phi = L'φ';
    wprintf(L"%lc = ", phi);
    wchar_t rad = L'°';
    wprintf(L"%.5f%lc", out->phi, rad);
    wprintf(L"\n");
}

void add_number(const char *input, int *i, Token *output, int *out_count) {
    char buffer[64];  
    int j = 0;

    while (isdigit(input[*i]) || input[*i] == '.' || input[*i] == 'e' || input[*i] == 'E') {
    buffer[(*i)++] = input[(*i)++];
}
    buffer[(*i)] = '\0';
    output[(*out_count)].type = 'n';
    output[(*out_count)].value = atof(buffer);   
    (*out_count)++;

}

Type value_of(char buffer[]) {
    Type rr = {0};
    const char *tokens[4] = { "voltage", "resistance", "current", "power" };
    Token output[MAX_TOKENS];
    int out_count;

    for (int i = 0; i < 4; i++) {
        char *token;
        if (i == 0) {
            token = strtok(buffer, " ");
        } else {
            token = strtok(NULL, " ");
        }

        if (!token) break;

        double value;
        if (strcmp(token, "pi") == 0) {
            value = PI; 
        } else {
            char cleaned[64];
            int k = 0;
            int j = 0;
             while (token[j] != '\0') {
                if (token[j] == '*' && token[j+1] == '1' && token[j+2] == '0') {
                    cleaned[k++] = 'e';
                    j += 3;

                    while (token[j] == ' ') j++;
                    if (token[j] == '^') j++; 
                    while (token[j] == ' ') j++;

                    if (token[j] == '+' || token[j] == '-') {
                        cleaned[k++] = token[j++];
                    }

                    while (isdigit(token[j])) {
                        cleaned[k++] = token[j++];
                    }
                } else {
                    cleaned[k++] = token[j++];
                }
            }
            cleaned[k] = '\0';
            shunting_yard(cleaned, output, &out_count); 
            value = evaluate_postfix(output, out_count);  
        }

        switch (i) {
            case 0: rr.voltage    = value; break;
            case 1: rr.resistance = value; break;
            case 2: rr.current    = value; break;
            case 3: rr.power      = value; break;
        }
    }

    return rr;
}

Inputs parseInputs(char buffer[], const char *tokens[], int count, int type, int parallel) {
    Inputs rr = {0};
    Token output[MAX_TOKENS];
    int out_count;

    for (int i = 0; i < count; i++) {
        char *token = (i == 0) ? strtok(buffer, " ") : strtok(NULL, " ");
        if (!token) break;

        double value;
        if (strcmp(token, "pi") == 0) {
            value = PI;
        } else {
            shunting_yard(token, output, &out_count);
            value = evaluate_postfix(output, out_count);
        }

        if (type == 1 && parallel == 0) {
            // ----- Series RL -----
            // V | f | R | L | Xl | Z | I | S | Q | P | phi
            switch (i) {
                case 0:  rr.V   = value; break;
                case 1:  rr.f   = value; break;
                case 2:  rr.R   = value; break;
                case 3:  rr.L   = value; break;
                case 4:  rr.Xl  = value; break;
                case 5:  rr.Z   = value; break;
                case 6:  rr.I   = value; break;
                case 7:  rr.S   = value; break;
                case 8:  rr.Q   = value; break;
                case 9:  rr.P   = value; break;
                case 10: rr.phi = value; break;
            }
        }
        else if (type == 1 && parallel == 1) {
            // ----- Parallel RL -----
            // V | f | R | L | Xl | Bl | Z | I | S | Q | P | phi
            switch (i) {
                case 0:  rr.V   = value; break;
                case 1:  rr.f   = value; break;
                case 2:  rr.R   = value; break;
                case 3:  rr.L   = value; break;
                case 4:  rr.Xl  = value; break;
                case 5:  rr.Bl  = value; break;
                case 6:  rr.Z   = value; break;
                case 7:  rr.I   = value; break;
                case 8:  rr.S   = value; break;
                case 9:  rr.Q   = value; break;
                case 10: rr.P   = value; break;
                case 11: rr.phi = value; break;
            }
        }
        else if (type == 2 && parallel == 0) {
            // ----- Series RC -----
            // V | f | R | C | Xc | Z | I | S | Q | P | phi
            switch (i) {
                case 0:  rr.V   = value; break;
                case 1:  rr.f   = value; break;
                case 2:  rr.R   = value; break;
                case 3:  rr.C   = value; break;
                case 4:  rr.Xc  = value; break;  // NOTE: you had no Xc field earlier?
                case 5:  rr.Z   = value; break;
                case 6:  rr.I   = value; break;
                case 7:  rr.S   = value; break;
                case 8:  rr.Q   = value; break;
                case 9:  rr.P   = value; break;
                case 10: rr.phi = value; break;
            }
        }
        else {
            // ----- Parallel RC -----
            // V | f | R | C | Xc | Bc | Z | I | S | Q | P | phi
            switch (i) {
                case 0:  rr.V   = value; break;
                case 1:  rr.f   = value; break;
                case 2:  rr.R   = value; break;
                case 3:  rr.C   = value; break;
                case 4:  rr.Xc  = value; break;  
                case 5:  rr.Bc  = value; break;
                case 6:  rr.Z   = value; break;
                case 7:  rr.I   = value; break;
                case 8:  rr.S   = value; break;
                case 9:  rr.Q   = value; break;
                case 10: rr.P   = value; break;
                case 11: rr.phi = value; break;
            }
        }
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
    const char *tokens[] = { "V", "f", "R", "G", "L", "Xl", "Bl", "Z", "I", "S", "Q", "P", "phi" };
    return parseInputs(buffer, tokens, 13, 1, 1);
}

Inputs RCpCalc(char buffer[]) {
    const char *tokens[] = { "V", "f", "R", "G", "C", "Xc", "Bc", "Z", "I", "S", "Q", "P", "phi" };
    return parseInputs(buffer, tokens, 13, 2, 1);
}

