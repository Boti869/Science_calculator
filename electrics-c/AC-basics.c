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
bool phi_AC(Outputs *out, int type, bool *progress) {
    if (is_known(out->phi)) return *progress;

    if (type == 1) { // inductive
        if (is_known(out->R) && is_known(out->Xl))
            out->phi = RAD_TO_DEG(atan2(out->Xl, out->R));
        else if (is_known(out->Z) && is_known(out->R))
            out->phi = RAD_TO_DEG(acos(out->R / out->Z));
        else if (is_known(out->Z) && is_known(out->Xl))
            out->phi = RAD_TO_DEG(asin(out->Xl / out->Z));
        else return *progress;
    } else { // capacitive
        if (is_known(out->R) && is_known(out->Xc))
            out->phi = RAD_TO_DEG(atan2(-out->Xc, out->R));
        else if (is_known(out->Z) && is_known(out->R))
            out->phi = RAD_TO_DEG(acos(out->R / out->Z));
        else if (is_known(out->Z) && is_known(out->Xc))
            out->phi = RAD_TO_DEG(asin(-out->Xc / out->Z));
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

// ---------- Driver ----------
bool get_AC_values(Outputs *out, int type, bool *progress) {
    xl_c_AC(out, type, progress);
    I_AC(out, progress);
    imp_AC(out, type, progress);
    L_C_AC(out, type, progress);
    R_AC(out, type, progress);
    phi_AC(out, type, progress);
    volt_AC(out, progress);
    power_AC(out, progress);
    f_ac(out, progress);
    return *progress;
}


int series_setup(int type) {
    char buffer[256];
    wprintf(L"Enter a letter to exit (break the code)\n"
            "enter 0 in place of unknown\n");
    while(1){
    if(type == 1)wprintf(L"Enter values for |   V   |   f   |   R   |   L   |   Xl   |   Z   |   I   |   S   |   Q   |   P   |   phi   (with spaces): \n");
    else if(type == 2)wprintf(L"Enter values for |   V   |   f   |   R   |   C   |   Xc   |   Z   |   I   |   S   |   Q   |   P   |   phi   (with spaces): \n"); 
    
    fgets(buffer, sizeof(buffer), stdin);
    Inputs result = (type == 1) ? RLsCalc(buffer) : RCsCalc(buffer);
    
    double L = 0, Xl = 0, C = 0, Xc = 0;
    if(type == 1){
        L = result.L, 
        Xl = result.Xl;}
    else{L = 0; Xl = 0;} 
    if(type == 2){
        C = result.C, 
        Xc = result.Xc;}
    else{C = 0; Xc = 0;}

    double  V = result.V, 
            f = result.f,       
            R = result.R, 
            Z = result.Z, 
            I = result.I, 
            S = result.S,
            Q = result.Q,
            P = result.P, 
            phi = DEG_TO_RAD(result.phi);        
    
    if(V == 0 && f == 0 && R == 0 && ((type == 1 &&L == 0 && Xl == 0) || (type == 2 && C == 0 && Xc == 0)) && Z == 0 && I == 0 && P == 0 && phi == 0) {
        wprintf(L"Exiting...\n");
        return 0; 
    }

    int nozero = 0;
    if (V != 0) nozero++; 
    if (f != 0) nozero++;
    if (R != 0) nozero++; 
    if (L != 0 && type == 1) nozero++; 
    if (Xl != 0 && type == 1) nozero++;
    if (C != 0 && type == 2) nozero++;
    if (Xc != 0 && type == 2) nozero++; 
    if (Z != 0) nozero++; 
    if (I != 0) nozero++; 
    if (S != 0) nozero++; 
    if (phi != 0) nozero++; 
    
    if (nozero < 3) {
        wprintf(L"Not enough data provided.\n");
        fflush(stdin);
        continue; 
        }
        
    if(R < 0 || L < 0 || Xl < 0 || Z < 0 || S < 0 || phi < 0 || f < 0 || C < 0 || Xc < 0 || P < 0 || Q < 0) {
        wprintf(L"Only V and I can be negative\n");
        fflush(stdin);
        continue; 
        }
    
    switch(type){case 1:solve_rl(&result, (Outputs *)&result); break;
                case 2:solve_rc(&result, (Outputs *)&result); break;}
    }
    return 0;
}