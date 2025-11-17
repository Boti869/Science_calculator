#include"../header_files/electrical-laws.h"
#include"../header_files/constants.h"
#include"../header_files/help.h"
#include"../header_files/AC-basic.h"
#include "../header_files/Parallel-AC.h"
#include "../header_files/Series-AC.h"
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
    if (resistance == 0 && power != 0 && current != 0)   resistance = power / pow(current, 2);
    if (resistance == 0 && voltage != 0 && power != 0)   resistance = pow(voltage, 2) / power;

    //Power
    if (power == 0 && voltage != 0 && current != 0)      power = voltage * current;
    if (power == 0 && voltage != 0 && resistance != 0)   power = pow(voltage, 2) / resistance;
    if (power == 0 && current != 0 && resistance != 0)   power = resistance * pow(current, 2);
    
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

int solve_circuit(const Inputs *in, Outputs *out, CircuitType type) {

    // Always appear
    SET(out->V,   in->V);
    SET(out->I,   in->I);
    SET(out->R,   in->R);
    SET(out->f,   in->f);
    SET(out->phi, in->phi);
    SET(out->P,   in->P);
    SET(out->Q,   in->Q);
    SET(out->S,   in->S);

    switch (type) {
        case SERIES_RL:
            SET(out->L,  in->L);
            SET(out->Xl, in->Xl);
            SET(out->Vl, in->Vl);
            SET(out->Vr, in->Vr);
            SET(out->Z, in->Z);
            break;

        case PARALLEL_RL:
            SET(out->L,  in->L);
            SET(out->Xl, in->Xl);
            SET(out->Bl, in->Bl);
            SET(out->G,  in->G);
            SET(out->Y,  in->Y);
            SET(out->Il, in->Il);
            SET(out->Ir, in->Ir);
            break;

        case SERIES_RC:
            SET(out->C,  in->C);
            SET(out->Xc, in->Xc);
            SET(out->Vc, in->Vc);
            SET(out->Vr, in->Vr);
            SET(out->Z,  in->Z);
            break;

        case PARALLEL_RC:
            SET(out->C,  in->C);
            SET(out->Xc, in->Xc);
            SET(out->Bc, in->Bc);
            SET(out->G,  in->G);
            SET(out->Y,  in->Y);
            SET(out->Ic, in->Ic);
            SET(out->Ir, in->Ir);
            break;

        case SERIES_RLC:
            SET(out->L,  in->L);
            SET(out->C,  in->C);
            SET(out->Xl, in->Xl);
            SET(out->Xc, in->Xc);
            SET(out->Vl, in->Vl);
            SET(out->Vc, in->Vc);
            SET(out->Vr, in->Vr);
            SET(out->Z,  in->Z);
            break;

        case PARALLEL_RLC:
            SET(out->L,  in->L);
            SET(out->C,  in->C);
            SET(out->Xl, in->Xl);
            SET(out->Xc, in->Xc);
            SET(out->Bl, in->Bl);
            SET(out->Bc, in->Bc);
            SET(out->G,  in->G);
            SET(out->Y,  in->Y);
            SET(out->Il, in->Il);
            SET(out->Ic, in->Ic);
            SET(out->Ir, in->Ir);
            break;
    }

    // Iterative solving
    bool progress = true;
    int iter = 0;
    while (progress && iter < 50) {
        progress = false;

        switch (type) {
            case SERIES_RL:     solve_series_AC(out, 1, &progress); break;
            case SERIES_RC:     solve_series_AC(out, 2, &progress); break;
            case SERIES_RLC:    solve_series_AC(out, 3, &progress); break;
            case PARALLEL_RL:   solve_parallel_AC(out, 1, &progress); break;
            case PARALLEL_RC:   solve_parallel_AC(out, 2, &progress); break;
            case PARALLEL_RLC:  solve_parallel_AC(out, 3, &progress); break;

        }
        iter++;
    }

    // Print results
    switch (type) {
        case SERIES_RL:   print_results(out, 1, 0); break;
        case SERIES_RC:   print_results(out, 2, 0); break;
        case SERIES_RLC:  print_results(out, 3, 0); break;
        case PARALLEL_RL: print_results(out, 1, 1); break;
        case PARALLEL_RC: print_results(out, 2, 1); break;
        case PARALLEL_RLC:print_results(out, 3, 1); break;
    }

    return 0;
}
