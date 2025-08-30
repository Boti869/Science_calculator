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

static inline bool is_known(double x) { return x != 0.0; }

// ---------- Voltage ----------
bool volt_AC(Outputs *out, bool *progress) {
    if (!is_known(out->V)) {
        if (is_known(out->I) && is_known(out->Z))
            out->V = out->I * out->Z;
        else if (is_known(out->S) && is_known(out->I))
            out->V = out->S / out->I;
        else if (is_known(out->P) && is_known(out->I) && is_known(out->phi))
            out->V = out->P / (out->I * cos(DEG_TO_RAD(out->phi)));
        else if (is_known(out->Q) && is_known(out->I) && is_known(out->phi))
            out->V = out->Q / (out->I * sin(DEG_TO_RAD(out->phi)));
        else return *progress;
        *progress = true;
    }
    return *progress;
}

// ---------- Current ----------
bool I_AC(Outputs *out, bool *progress) {
    if (!is_known(out->I)) {
        if (is_known(out->V) && is_known(out->Z))
            out->I = out->V / out->Z;
        else if (is_known(out->S) && is_known(out->V))
            out->I = out->S / out->V;
        else if (is_known(out->P) && is_known(out->V) && is_known(out->phi))
            out->I = out->P / (out->V * cos(DEG_TO_RAD(out->phi)));
        else if (is_known(out->Q) && is_known(out->V) && is_known(out->phi))
            out->I = out->Q / (out->V * sin(DEG_TO_RAD(out->phi)));
        else return *progress;
        *progress = true;
    }
    return *progress;
}

// ---------- Impedance ----------
bool imp_AC(Outputs *out, int type, bool *progress) {
    if (!is_known(out->Z)) {
        if (is_known(out->V) && is_known(out->I))
            out->Z = out->V / out->I;
        else if (is_known(out->R)) {
            if (type == 1 && is_known(out->Xl))
                out->Z = hypot(out->R, out->Xl);
            else if (type == 2 && is_known(out->Xc))
                out->Z = hypot(out->R, out->Xc);
            else return *progress;
        } else return *progress;
        *progress = true;
    }
    return *progress;
}

// ---------- Reactance ----------
bool xl_c_AC(Outputs *out, int type, bool *progress) {
    if (type == 1 && !is_known(out->Xl)) {
        if (is_known(out->L) && is_known(out->f))
            out->Xl = OMEGA(out->f) * out->L;
        else if (is_known(out->R) && is_known(out->Z))
            out->Xl = sqrt(out->Z*out->Z - out->R*out->R);
        else if (is_known(out->Z) && is_known(out->phi))
            out->Xl = out->Z * sin(DEG_TO_RAD(out->phi));
        else if (is_known(out->R) && is_known(out->phi))
            out->Xl = out->R * tan(DEG_TO_RAD(out->phi));
        else return *progress;
        *progress = true;
    }
    if (type == 2 && !is_known(out->Xc)) {
        if (is_known(out->C) && is_known(out->f))
            out->Xc = 1.0 / (OMEGA(out->f) * out->C);
        else if (is_known(out->R) && is_known(out->Z))
            out->Xc = sqrt(out->Z*out->Z - out->R*out->R);
        else if (is_known(out->Z) && is_known(out->phi))
            out->Xc = -out->Z * sin(DEG_TO_RAD(out->phi));
        else if (is_known(out->R) && is_known(out->phi))
            out->Xc = -out->R * tan(DEG_TO_RAD(out->phi));
        else return *progress;
        *progress = true;
    }
    return *progress;
}

// ---------- Resistance ----------
bool R_AC(Outputs *out, int type, bool *progress) {
    if (!is_known(out->R)) {
        if (type == 1 && is_known(out->Z) && is_known(out->Xl))
            out->R = sqrt(out->Z*out->Z - out->Xl*out->Xl);
        else if (type == 2 && is_known(out->Z) && is_known(out->Xc))
            out->R = sqrt(out->Z*out->Z - out->Xc*out->Xc);
        else if (is_known(out->Z) && is_known(out->phi))
            out->R = out->Z * cos(DEG_TO_RAD(out->phi));
        else if (type == 1 && is_known(out->Xl) && is_known(out->phi))
            out->R = out->Xl / tan(DEG_TO_RAD(out->phi));
        else if (type == 2 && is_known(out->Xc) && is_known(out->phi))
            out->R = -out->Xc / tan(DEG_TO_RAD(out->phi));
        else return *progress;
        *progress = true;
    }
    return *progress;
}

// ---------- Phase angle ----------
bool phi_AC(Outputs *out, int type, int parallel, bool *progress) {
    if (is_known(out->phi)) return *progress;

    if (type == 1) { // inductive
        if (is_known(out->R) && is_known(out->Xl))
            out->phi = RAD_TO_DEG(atan2(out->Xl, out->R));
        else if (is_known(out->Z) && is_known(out->R))
            out->phi = RAD_TO_DEG(acos(out->R / out->Z));
        else if (is_known(out->Z) && is_known(out->Xl))
            out->phi = RAD_TO_DEG(asin(out->Xl / out->Z));
        else return *progress;
    } else if(type == 2){ // capacitive
        if (is_known(out->R) && is_known(out->Xc))
            out->phi = RAD_TO_DEG(atan2(-out->Xc, out->R));
        else if (is_known(out->Z) && is_known(out->R))
            out->phi = RAD_TO_DEG(acos(out->R / out->Z));
        else if (is_known(out->Z) && is_known(out->Xc))
            out->phi = RAD_TO_DEG(asin(-out->Xc / out->Z));
        else return *progress;
    }else if (parallel == 1){ //parallel
        if (is_known(out->G) && is_known(out->Bl))
            out->phi = RAD_TO_DEG(atan2(out->Bl, out->G));
        else if (is_known(out->Z) && is_known(out->G))
            out->phi = RAD_TO_DEG(acos(out->G * out->Z));
        else if (is_known(out->Z) && is_known(out->Bl))
            out->phi = RAD_TO_DEG(asin(out->Bl * out->Z));
        else return *progress;
    }else if(parallel == 2){ //prallel RC
        if (is_known(out->R) && is_known(out->C))
            out->phi = RAD_TO_DEG(atan2(out->C, out->R));
        else if (is_known(out->Z) && is_known(out->R))
            out->phi = RAD_TO_DEG(acos(out->R / out->Z));
        else if (is_known(out->Z) && is_known(out->C))
            out->phi = RAD_TO_DEG(asin(out->C / out->Z));
        else return *progress;
    }
    *progress = true;
    return *progress;
}

// ---------- Power ----------
bool power_AC(Outputs *out, bool *progress) {
    if (!is_known(out->S)) {
        if (is_known(out->V) && is_known(out->I))
            out->S = out->V * out->I;
        else if (is_known(out->P) && is_known(out->Q))
            out->S = hypot(out->P, out->Q);
        if (is_known(out->S)) *progress = true;
    }
    if (!is_known(out->P)) {
        if (is_known(out->S) && is_known(out->Q))
            out->P = sqrt(out->S*out->S - out->Q*out->Q);
        else if (is_known(out->V) && is_known(out->I) && is_known(out->phi))
            out->P = out->V * out->I * cos(DEG_TO_RAD(out->phi));
        if (is_known(out->P)) *progress = true;
    }
    if (!is_known(out->Q)) {
        if (is_known(out->S) && is_known(out->P))
            out->Q = sqrt(out->S*out->S - out->P*out->P);
        else if (is_known(out->V) && is_known(out->I) && is_known(out->phi))
            out->Q = out->V * out->I * sin(DEG_TO_RAD(out->phi));
        if (is_known(out->Q)) *progress = true;
    }
    return *progress;
}

// ---------- L or C ----------
bool L_C_AC(Outputs *out, int type, bool *progress) {
    if (type == 1 && !is_known(out->L) && is_known(out->Xl) && is_known(out->f)) {
        out->L = out->Xl / OMEGA(out->f);
        *progress = true;
    } else if (type == 2 && !is_known(out->C) && is_known(out->Xc) && is_known(out->f)) {
        out->C = 1.0 / (OMEGA(out->f) * out->Xc);
        *progress = true;
    }
    return *progress;
}

// ---------- Frequency (rarely solvable like this) ----------
bool f_ac(Outputs *out, bool *progress) {
    // Normally f must be known, but placeholder for completeness
    return *progress;
}

// ---------- Susceptance (for parallel circuits) ----------
bool Bl_C_AC(Outputs *out, int type, bool *progress) {
    if(is_known(out->R)) out->G = 1.0 / out->R; return *progress;
    if (type == 1 && !is_known(out->Bl) && is_known(out->Xl) && is_known(out->f)) {
        out->Bl = 1.0 / out->Xl;
        *progress = true;
    } else if (type == 2 && !is_known(out->Bc) && is_known(out->Xc) && is_known(out->f)) {
        out->Bc = 1.0 / out->Xc;
        *progress = true;
    }
    return *progress;
}
// ---------- Driver ----------
bool get_AC_values(Outputs *out, int type, int parallel, bool *progress) {
    xl_c_AC(out, type, progress);
    I_AC(out, progress);
    imp_AC(out, type, progress);
    L_C_AC(out, type, progress);
    R_AC(out, type, progress);
    phi_AC(out, type, parallel, progress);
    volt_AC(out, progress);
    power_AC(out, progress);
    f_ac(out, progress);
    return *progress;
}

int series_setup(int type, int parallel) {
    char buffer[256];
    wprintf(L"Enter a letter to exit (break the code)\n"
            "Enter 0 in place of unknown\n");

    while (1) {
        // Pick prompt dynamically
        if (type == 1 && parallel == 0)
            wprintf(L"Enter values for  V | f | R | L | Xl | Z | I | S | Q | P | phi \n");
        else if (type == 1 && parallel == 1)
            wprintf(L"Enter values for  V | f | R | L | Xl | Bl | Z | I | S | Q | P | phi \n");
        else if (type == 2 && parallel == 0)
            wprintf(L"Enter values for  V | f | R | C | Xc | Z | I | S | Q | P | phi \n");
        else
            wprintf(L"Enter values for  V | f | R | C | Xc | Bc | Z | I | S | Q | P | phi \n");

        if (!fgets(buffer, sizeof(buffer), stdin))
            return 0;

        Inputs result = {0};
        if (parallel == 0)
            result = (type == 1) ? RLsCalc(buffer) : RCsCalc(buffer);
        else
            result = (type == 1) ? RLpCalc(buffer) : RCpCalc(buffer);

        if (result.V == 0 && result.f == 0 && result.R == 0 && 
            result.L == 0 && result.Xl == 0 && result.C == 0 && result.Xc == 0 && 
            result.Z == 0 && result.I == 0 && result.P == 0 && result.phi == 0) {
            wprintf(L"Exiting...\n");
            return 0;
        }

        int nozero = 0;
        if (result.V) nozero++;
        if (result.f) nozero++;
        if (result.R) nozero++;
        if (type == 1 && (result.L || result.Xl)) nozero++;
        if (type == 2 && (result.C || result.Xc)) nozero++;
        if (result.Z) nozero++;
        if (result.I) nozero++;
        if (result.S) nozero++;
        if (result.phi) nozero++;

        if (nozero < 3) {
            wprintf(L"Not enough data provided.\n");
            continue;
        }

        if ((result.R < 0) || (result.L < 0) || (result.Xl < 0) || (result.Z < 0) ||
            (result.S < 0) || (result.phi < 0) || (result.f < 0) ||
            (result.C < 0) || (result.Xc < 0) || (result.P < 0) || (result.Q < 0)) {
            wprintf(L"Only V and I can be negative.\n");
            continue;
        }

        // Solve
        if (parallel == 0) {
            if (type == 1) solve_circuit(&result, (Outputs *)&result, SERIES_RL);
            else            solve_circuit(&result, (Outputs *)&result, SERIES_RC);
        } else {
            if (type == 1) solve_circuit(&result, (Outputs *)&result, PARALLEL_RL);
            else            solve_circuit(&result, (Outputs *)&result, PARALLEL_RC);
        }
    }
}
