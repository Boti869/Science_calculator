#include"../header_files/electrical-laws.h"
#include"../header_files/constants.h"
#include"../header_files/help.h"
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<wchar.h>
#include<locale.h>
#include<stdbool.h>

void applyOhmLaw(double voltage, double resistance, double current, double power) {
    //Voltage
    if (voltage == 0 && current != 0 && resistance != 0) voltage = current * resistance;
    if (voltage == 0 && power != 0 && current != 0)      voltage = power / current;
    if (voltage == 0 && power != 0 && resistance != 0)   voltage = sqrt(power * resistance);
    
    //Current
    if (current == 0 && voltage != 0 && resistance != 0) current = voltage / resistance;
    if (current == 0 && power != 0 && voltage != 0)      current = power / voltage;
    if (current == 0 && power != 0 && resistance != 0)   current = sqrt(power / resistance);

    //Resistance
    if (resistance == 0 && voltage != 0 && current != 0) resistance = voltage / current;
    if (resistance == 0 && power != 0 && current != 0)   resistance = power / (current * current);
    if (resistance == 0 && voltage != 0 && power != 0)   resistance = (voltage * voltage) / power;

    //Power
    if (power == 0 && voltage != 0 && current != 0)      power = voltage * current;
    if (power == 0 && voltage != 0 && resistance != 0)   power = (voltage * voltage) / resistance;
    if (power == 0 && current != 0 && resistance != 0)   power = resistance * (current * current);
    
    pretty_print(voltage, 'V');
    wprintf(L"\n");
    wchar_t ohm = L'Ω';
    pretty_print(resistance, ohm);
    wprintf(L"\n");
    pretty_print(current, 'A');
    wprintf(L"\n");
    pretty_print(power, 'W');
    wprintf(L"\n");
}

int solve_rl(const Inputs *in, Outputs *out) {
    out->V   = in->V;//
    out->I   = in->I;//
    out->R   = in->R;//
    out->L   = in->L;//
    out->f   = in->f;
    out->phi = in->phi;//
    out->P   = in->P;//
    out->Xl  = in->Xl;//
    out->Z   = in->Z;//
    out->Q   = in->Q;//
    out->S   = in->S;//

    bool progress = true;
    int iter = 0;
    while (progress && iter++ < 50) {
        progress = false;

    //omega if f known
        double w = (out->f != 0 ? OMEGA(out->f) : NAN);
    
    //XL
        // Xl from L and f
        if (out->Xl == 0 && out->L != 0 && w != 0) {
            out->Xl = w * out->L; progress = true;
        }
        // xl from R and Z
        if (out->Xl == 0 && out->R != 0 && out->Z != 0) {
            out->Xl = sqrt(pow(out->Z, 2) - pow(out->R, 2)); progress = true;
        }
        // xl from Z and phi
        if (out->Xl == 0 && out->Z != 0 && out->phi != 0) {
            out->Xl = out->Z * cos(out->phi); progress = true;
        }
        // xl from R and phi
        if (out->Xl == 0 && out->R != 0 && out->phi != 0) {
            out->Xl = out->R * tan(DEG_TO_RAD(out->phi)); progress = true;
        }

    //I
        // I from V and Z
        if (out->I == 0 && out->V != 0 && out->Z != 0) {
            out->I = out->V / out->Z; progress = true;
        }
        // I from S and V
        if (out->I == 0 && out->S != 0 && out->V != 0) {
            out->I = out->S / out->V; progress = true;
        }

    //Z
        // Z from V and I
        if (out->Z == 0 && out->V != 0 && out->I != 0) {
            out->Z = out->V / out->I; progress = true;
        }
        // Z from R and Xl
        if (out->Z == 0 && out->R != 0 && out->Xl != 0) {
            out->Z = sqrt(pow(out->R, 2) + pow(out->Xl, 2)); progress = true;
        }
    //L
        // L from Xl and f (requires f)
        if (out->L == 0 && out->Xl != 0 && out->f != 0) {
            out->L = out->Xl / OMEGA(out->f); progress = true;
        }
    
    //R
        // R from Z and xl
        if (out->R == 0 && out->Z != 0 && out->Xl != 0) {
            out->R = sqrt(pow(out->Z, 2) - pow(out->Xl, 2)); progress = true;
        }
        // R from Z and phi
        if (out->R == 0 && out->Z != 0 && out->phi != 0) {
            out->R = out->Z * cos(DEG_TO_RAD(out->phi)); progress = true;
        }
        // R from Xl and phi
        if (out->R == 0 && out->Xl != 0 && out->phi != 0) {
            out->R = out->Xl / tan(DEG_TO_RAD(out->phi)); progress = true;
        }
    
    //PHI    
        // phi from R & Xl
        if (out->phi == 0 && out->R != 0 && out->Xl != 0) {
            if (out->Xl != 0) {
                out->phi = RAD_TO_DEG(atan2(out->Xl, out->R)); progress = true;
            } else {
                out->phi = 0; // Xl=0 means no phase shift
            }
        }
        //phi from Z & R
        if (out->phi == 0 && !isnan(out->Z) && !isnan(out->R)) {
            if (out->Z != 0) {
                out->phi = RAD_TO_DEG(acos(out->R / out->Z)); progress = true;
            } else {
                out->phi = 0; // Z=0 means no phase shift
            }
        }
        // phi from Z & Xl
        if (out->phi == 0 && out->Xl != 0 && out->Z != 0) {
            if (out->Z != 0) {
                out->phi = RAD_TO_DEG(asin(out->Xl / out->Z)); progress = true;
            } else {
                out->phi = 0; // Z=0 means no phase shift
            }
        }

    //V
        // V from I and Z
        if (out->V == 0 && out->I != 0 && out->Z != 0) {
            out->V = out->I * out->Z; progress = true;
        }
        // V from S and I
        if (out->V == 0 && out->S != 0 && out->I != 0) {
            out->V = out->S / out->I; progress = true;
        }
        // V from P and I (if phi known)
        if (out->V == 0 && out->P != 0 && out->I != 0) {
            out->V = out->P / out->I / cos(DEG_TO_RAD(out->phi)); progress = true;
        }
        // V from Q and I (if phi known)
        if (out->V == 0 && out->Q != 0 && out->I != 0) {
            out->V = out->Q / out->I / sin(DEG_TO_RAD(out->phi)); progress = true;
        }

    //S
        // S from V and I
        if (out->S == 0 && out->V != 0 && out->I != 0) {
            out->S = out->V * out->I; progress = true;
        }
        // S from P and Q
        if (out->S == 0 && out->P != 0 && out->Q != 0) {
            out->S = sqrt(pow(out->P, 2) + pow(out->Q, 2)); progress = true;
        }
    //Q
        // Q from S and P
        if (out->Q == 0 && out->S != 0 && out->P != 0) {
            out->Q = sqrt(pow(out->S, 2) - pow(out->P, 2)); progress = true;
        }
        // Q from V and I (if phi known)
        if (out->Q == 0 && out->V != 0 && out->phi != 0 && out->I != 0) {
            out->Q = out->V * out->I * sin(DEG_TO_RAD(out->phi)); progress = true;
        }
    //P
        // P from S and Q
        if (out->P == 0 && out->S != 0 && out->Q != 0) {
            out->P = sqrt(pow(out->S, 2) - pow(out->Q, 2)); progress = true;
        }
        // P from V and I (if phi known)
        if (out->P == 0 && out->V != 0 && out->phi != 0 && out->I != 0) {
            out->P = out->V * out->I * cos(DEG_TO_RAD(out->phi)); progress = true;
        }

    //f
        // f from R and Xl (if phi known)
        if (out->f == 0 && out->R != 0 && out->Xl != 0 && out->phi != 0) {
            out->f = out->R * tan(DEG_TO_RAD(out->phi) / out->Xl); progress = true;
        }
}

        
    pretty_print(out->V, 'V');
    wprintf(L"\n");
    pretty_print(out->f, 'H');
    wprintf(L"z");
    wprintf(L"\n");
    wchar_t ohm = L'Ω';
    pretty_print(out->R, ohm);
    wprintf(L"\n");
    pretty_print(out->L, 'H');
    wprintf(L"\n");
    pretty_print(out->Xl, ohm);
    wprintf(L"\n");
    pretty_print(out->Z, ohm);
    wprintf(L"\n");
    pretty_print(out->I, 'A');
    wprintf(L"\n");
    pretty_print(out->S, 'V');
    wprintf(L"A");
    wprintf(L"\n");
    pretty_print(out->Q, 'V');
    wprintf(L"A");
    wprintf(L"r");
    wprintf(L"\n");
    pretty_print(out->P, 'W');
    wprintf(L"\n");
    wchar_t rad = L'°';
    pretty_print(out->phi, rad);
    wprintf(L"\n");
    return 0;
}