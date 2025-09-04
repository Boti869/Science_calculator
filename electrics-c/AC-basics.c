// type 1 = L (Inductive), 2 = C (Capacitive), 3 = LC (Both)
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
        else if(is_known(out->Y) && is_known(out->I))
            out->V = out->I / out->Y;
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
bool imp_AC(Outputs *out, int type, int parallel, bool *progress) {
    if (!is_known(out->Z)) {
        if (is_known(out->V) && is_known(out->I))
            out->Z = out->V / out->I;
        else if (is_known(out->R)) {
            if (type == 1 && is_known(out->Xl))
                out->Z = hypot(out->R, out->Xl);
            else if (type == 2 && is_known(out->Xc))
                out->Z = hypot(out->R, out->Xc);
            else if (type == 3 && is_known(out->Xl) && is_known(out->Xc)) {
                if(out->Xl > out->Xc) {double X = out->Xl - out->Xc;
                out->Z = hypot(out->R, X);}
                else {double X = out->Xc - out->Xl;
                out->Z = hypot(out->R, X);}
            } else return *progress;
        } else return *progress;
        *progress = true;
    }
    if(parallel == 1){
        double B;
            if(out->Bl > out->Bc) B = out->Bl - out->Bc;
            else B = out->Bc - out->Bl;
        if(!is_known(out->Y)){
            if(is_known(out->Z)) out->Y = 1.0 / out->Z;
            else if(is_known(out->G) && is_known(out->phi)) out->Y = out->G / cos(DEG_TO_RAD(out->phi));
            else if(is_known(out->Bl) && is_known(out->Bc) && is_known(out->phi)) out->Y = B / sin(DEG_TO_RAD(out->phi));
            else if(is_known(out->G) && is_known(out->Bl) && is_known(out->Bc)) out->Y = hypot(out->G, B);
            else return *progress;
            *progress = true;
        }
    }
    return *progress;
}

// ---------- Reactance ----------
bool xl_c_AC(Outputs *out, int type, int parallel, bool *progress) {
    // Inductive reactance (Xl)
    if ((type == 1 || type == 3) && !is_known(out->Xl)) {
        if (is_known(out->L) && is_known(out->f))
            out->Xl = OMEGA(out->f) * out->L;
        else if (is_known(out->R) && is_known(out->Z))
            out->Xl = sqrt(out->Z * out->Z - out->R * out->R);
        else if (is_known(out->Z) && is_known(out->phi))
            out->Xl = out->Z * sin(DEG_TO_RAD(out->phi));
        else if (is_known(out->R) && is_known(out->phi))
            out->Xl = out->R * tan(DEG_TO_RAD(out->phi));
        else if (parallel == 1 && is_known(out->Bl))
            out->Xl = 1.0 / out->Bl;
        else
            return *progress;

        *progress = true;
    }

    // Capacitive reactance (Xc)
    if ((type == 2 || type == 3) && !is_known(out->Xc)) {
        if (is_known(out->C) && is_known(out->f))
            out->Xc = 1.0 / (OMEGA(out->f) * out->C);
        else if (is_known(out->R) && is_known(out->Z))
            out->Xc = sqrt(out->Z * out->Z - out->R * out->R);
        else if (is_known(out->Z) && is_known(out->phi))
            out->Xc = -out->Z * sin(DEG_TO_RAD(out->phi));
        else if (is_known(out->R) && is_known(out->phi))
            out->Xc = -out->R * tan(DEG_TO_RAD(out->phi));
        else if (parallel == 1 && is_known(out->Bc))
            out->Xc = 1.0 / out->Bc;
        else
            return *progress;

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
        else if (type == 3 && is_known(out->Xl) && is_known(out->Xc) && is_known(out->phi)) {
           if(out->Xl > out->Xc){ double X = out->Xl - out->Xc;
            out->R = X / tan(DEG_TO_RAD(out->phi));
        }
            else{ double X = out->Xc - out->Xl;
            out->R = X / tan(DEG_TO_RAD(out->phi));}
        }
        else if(is_known(out->G)) out->R = 1.0 / out->G;
        else return *progress;
        *progress = true;
    }
    return *progress;
}

// ---------- Phase angle ----------
bool phi_AC(Outputs *out, int type, int parallel, bool *progress) {
    if (is_known(out->phi)) return *progress;

    // Series inductive
    if (type == 1) {
        if (is_known(out->R) && is_known(out->Xl))
            out->phi = RAD_TO_DEG(atan2(out->Xl, out->R));
        else if (is_known(out->Z) && is_known(out->R))
            out->phi = RAD_TO_DEG(acos(out->R / out->Z));
        else if (is_known(out->Z) && is_known(out->Xl))
            out->phi = RAD_TO_DEG(asin(out->Xl / out->Z));
        else return *progress;
    }

    // Series capacitive
    else if (type == 2) {
        if (is_known(out->R) && is_known(out->Xc))
            out->phi = RAD_TO_DEG(atan2(-out->Xc, out->R));
        else if (is_known(out->Z) && is_known(out->R))
            out->phi = RAD_TO_DEG(acos(out->R / out->Z));
        else if (is_known(out->Z) && is_known(out->Xc))
            out->phi = RAD_TO_DEG(asin(-out->Xc / out->Z));
        else return *progress;
    }

    // Series RLC
    else if (type == 3) {
        double X = out->Xl - out->Xc;
        if (is_known(out->R) && is_known(out->Xl) && is_known(out->Xc))
            out->phi = RAD_TO_DEG(atan2(X, out->R));
        else if (is_known(out->Z) && is_known(out->R) && is_known(out->Xl) && is_known(out->Xc))
            out->phi = RAD_TO_DEG(atan2(X, out->R));
        else if (is_known(out->Z) && is_known(out->R))
            out->phi = RAD_TO_DEG(acos(out->R / out->Z));
        else if (is_known(out->Z) && is_known(out->Xl) && is_known(out->Xc))
            out->phi = RAD_TO_DEG(asin(X / out->Z));
        else return *progress;
    }

    // Parallel RL
    else if (parallel == 1) {
        if (is_known(out->G) && is_known(out->Bl))
            out->phi = RAD_TO_DEG(atan2(out->Bl, out->G));
        else if (is_known(out->Y) && is_known(out->G))
            out->phi = RAD_TO_DEG(acos(out->G * out->Y));
        else if (is_known(out->Y) && is_known(out->Bl))
            out->phi = RAD_TO_DEG(asin(out->Bl * out->Y));
        else return *progress;
    }

    // Parallel RC
    else if (parallel == 2) {
        if (is_known(out->R) && is_known(out->C))
            out->phi = RAD_TO_DEG(atan2(out->C, out->R));
        else if (is_known(out->Z) && is_known(out->R))
            out->phi = RAD_TO_DEG(acos(out->R / out->Z));
        else if (is_known(out->Z) && is_known(out->C))
            out->phi = RAD_TO_DEG(asin(out->C / out->Z));
        else return *progress;
    }

    // Parallel RLC
    else if (parallel == 3) {
        double B = out->Bc - out->Bl;
        if (is_known(out->G) && is_known(out->Bl) && is_known(out->Bc))
            out->phi = RAD_TO_DEG(atan2(B, out->G));
        else if (is_known(out->Z) && is_known(out->G))
            out->phi = RAD_TO_DEG(acos(out->G * out->Z));
        else if (is_known(out->Y) && is_known(out->Bl) && is_known(out->Bc))
            out->phi = RAD_TO_DEG(asin(B * out->Y));
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
    if ((type == 1 || type == 3) && !is_known(out->L) && is_known(out->Xl) && is_known(out->f)) {
        out->L = out->Xl / OMEGA(out->f);
        *progress = true;
    } else if ((type == 2 || type == 3) && !is_known(out->C) && is_known(out->Xc) && is_known(out->f)) {
        out->C = 1.0 / (OMEGA(out->f) * out->Xc);
        *progress = true;}
    return *progress;
}

// ---------- Frequency (rarely solvable like this) ----------
bool f_ac(Outputs *out, bool *progress) {
    // Normally f must be known, but placeholder for completeness
    return *progress;
}

// ---------- Susceptance (for parallel circuits) ----------
bool Bl_C_AC(Outputs *out, int type, bool *progress) {
    if (is_known(out->R) && !is_known(out->G)) {
        out->G = 1.0 / out->R;
        *progress = true;
    }
    if ((type == 1 || type == 3) && !is_known(out->Bl) && is_known(out->Xl)) {
        out->Bl = 1.0 / out->Xl;
        *progress = true;
    }
    if ((type == 2 || type == 3) && !is_known(out->Bc) && is_known(out->Xc)) {
        out->Bc = 1.0 / out->Xc;
        *progress = true;
    }
    return *progress;
}

// ---------- Driver ----------
bool get_AC_values(Outputs *out, int type, int parallel, bool *progress) {
    xl_c_AC(out, type, parallel, progress);
    I_AC(out, progress);
    imp_AC(out, type, parallel, progress);
    L_C_AC(out, type, progress);
    R_AC(out, type, progress);
    phi_AC(out, type, parallel, progress);
    volt_AC(out, progress);
    power_AC(out, progress);
    Bl_C_AC(out, type, progress);
    return *progress;
}
