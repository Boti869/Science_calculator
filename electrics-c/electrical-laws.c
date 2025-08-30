#include"../header_files/electrical-laws.h"
#include"../header_files/constants.h"
#include"../header_files/help.h"
#include"../header_files/AC-basic.h"
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

int solve_circuit(const Inputs *in, Outputs *out, CircuitType type) {
    // all have these
    out->V   = in->V;
    out->I   = in->I;
    out->R   = in->R;
    out->f   = in->f;
    out->phi = in->phi;
    out->P   = in->P;
    out->Z   = in->Z;
    out->Q   = in->Q;
    out->S   = in->S;

    switch (type) {
        case SERIES_RL:
        case PARALLEL_RL:
            out->L  = in->L;
            out->Xl = in->Xl;
            if (type == PARALLEL_RL) out->Bl = in->Bl;
            break;

        case SERIES_RC:
        case PARALLEL_RC:
            out->C  = in->C;
            out->Xc = in->Xc;
            if (type == PARALLEL_RC) out->Bc = in->Bc;
            break;
    }

    // Iterative solving
    bool progress = true;
    int iter = 0;
    while (progress && iter < 50) {
        progress = false;

        switch (type) {
            case SERIES_RL:     get_AC_values(out, 1, 0, &progress); break;
            case SERIES_RC:     get_AC_values(out, 2, 0, &progress); break;
            case PARALLEL_RL:   get_AC_values(out, 1, 1, &progress); break;
            case PARALLEL_RC:   get_AC_values(out, 2, 2, &progress); break;
        }

        iter++;
    }

    // Print results
    switch (type) {
        case SERIES_RL:   print_results(out, 1, 0); break;
        case SERIES_RC:   print_results(out, 2, 0); break;
        case PARALLEL_RL: print_results(out, 1, 1); break;
        case PARALLEL_RC: print_results(out, 2, 1); break;
    }

    return 0;
}
