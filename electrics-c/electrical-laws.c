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
    //unkowns set to NAN
    int known_mask = 0;
    out->V   = COPY(V);
    out->I   = COPY(I);
    out->R   = COPY(R);
    out->L   = COPY(L);
    out->f   = COPY(f);
    out->phi = COPY(phi);
    out->P   = COPY(P);
    out->Q   = COPY(Q);
    out->S   = COPY(S);
    out->Xl  = COPY(Xl);
    out->Z   = COPY(Z);

    bool progress = true;
    int iter = 0;
    while (progress && iter++ < 40) {
        progress = false;

        //omega if f known
        double w = (!isnan(out->f) ? OMEGA(out->f) : NAN);

        // Xl from L and f
        if (isnan(out->Xl) && !isnan(out->L) && !isnan(w)) {
            out->Xl = w * out->L; progress = true;
        }
        // L from Xl and f (requires f)
        if (isnan(out->L) && !isnan(out->Xl) && !isnan(out->f)) {
            out->L = out->Xl / OMEGA(out->f); progress = true;
        }

        // Z from V and I
        if (isnan(out->Z) && !isnan(out->V) && !isnan(out->I) && out->I != 0) {
            out->Z = out->V / out->I; progress = true;
        }
        // try to compute Z from R and Xl
        if (isnan(out->Z) && !isnan(out->R) && !isnan(out->Xl)) {
            out->Z = sqrt(out->R * out->R + out->Xl * out->Xl); progress = true;
        }
        // compute R or Xl from Z and the other
        if (!isnan(out->Z) && !isnan(out->R) && isnan(out->Xl)) {
            double sq = out->Z*out->Z - out->R*out->R;
            if (sq < -1e-9) return -1; // inconsistent
            out->Xl = (sq <= 0.0) ? 0.0 : sqrt(sq); progress = true;
        }
        if (!isnan(out->Z) && !isnan(out->Xl) && isnan(out->R)) {
            double sq = out->Z*out->Z - out->Xl*out->Xl;
            if (sq < -1e-9) return -1;
            out->R = (sq <= 0.0) ? 0.0 : sqrt(sq); progress = true;
        }

        // phi from R & Xl
        if (isnan(out->phi) && !isnan(out->R) && !isnan(out->Xl)) {
            out->phi = RAD_TO_DEG(atan2(out->Xl, out->R)); progress = true;
        }
        // R/Xl from phi & Z
        if (!isnan(out->Z) && !isnan(out->phi)) {
            double phir = DEG_TO_RAD(out->phi);
            if (isnan(out->R)) { out->R = out->Z * cos(phir); progress = true; }
            if (isnan(out->Xl)) { out->Xl = out->Z * sin(phir); progress = true; }
        }

        // P, Q, S relations if V & I known (or if S known)
        if (!isnan(out->V) && !isnan(out->I)) {
            if (isnan(out->S)) { out->S = out->V * out->I; progress = true; }
            if (!isnan(out->phi)) {
                double phir = DEG_TO_RAD(out->phi);
                if (isnan(out->P)) { out->P = out->V * out->I * cos(phir); progress = true; }
                if (isnan(out->Q)) { out->Q = out->V * out->I * sin(phir); progress = true; }
            } else {
                // if R known and Z known -> phi computable earlier
                if (!isnan(out->R) && !isnan(out->Z) && isnan(out->phi)) {
                    out->phi = RAD_TO_DEG(acos(out->R / out->Z)); progress = true;
                }
            }
        }
        // P,Q -> S,phi,I (if V known)
        if (!isnan(out->P) && !isnan(out->Q) && isnan(out->S)) {
            out->S = hypot(out->P, out->Q); progress = true;
        }
        if (!isnan(out->S) && !isnan(out->V) && isnan(out->I) && out->V != 0) {
            out->I = out->S / out->V; progress = true;
        }
        if (!isnan(out->P) && !isnan(out->Q) && isnan(out->phi)) {
            out->phi = RAD_TO_DEG(atan2(out->Q, out->P)); progress = true;
        }
        // from S & phi -> P,Q
        if (!isnan(out->S) && !isnan(out->phi)) {
            double phir = DEG_TO_RAD(out->phi);
            if (isnan(out->P)) { out->P = out->S * cos(phir); progress = true; }
            if (isnan(out->Q)) { out->Q = out->S * sin(phir); progress = true; }
        }

        // If we have Xl but not L and we have f -> L
        if (isnan(out->L) && !isnan(out->Xl) && !isnan(out->f)) {
            out->L = out->Xl / OMEGA(out->f); progress = true;
        }

        // sanity: compute Z if not yet but V & I present
        if (isnan(out->Z) && !isnan(out->V) && !isnan(out->I) && out->I != 0) {
            out->Z = out->V / out->I; progress = true;
        }
    } 

    // If L is requested but f not known, leave L as NAN (can't compute)
    if (!isnan(out->L) && isnan(out->f)) {
        // keep L if given, else it would be NAN
    }
    pretty_print(out->V, 'V');
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