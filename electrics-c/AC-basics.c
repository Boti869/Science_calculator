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
#include"../header_files/Series-AC.h"

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
    if ((type == 1 || type == 3)){
        if (!is_known(out->L) && is_known(out->Xl) && is_known(out->f)) {
            out->L = out->Xl / OMEGA(out->f);
            *progress = true;
        }
        else return *progress;

    }
    if ((type == 2 || type == 3)){
        if (!is_known(out->C) && is_known(out->Xc) && is_known(out->f)) {
            out->C = 1.0 / (OMEGA(out->f) * out->Xc);
            *progress = true;
        }
        else return *progress;
    }
    return *progress;
}

// ---------- Driver ----------
bool get_AC_values(Outputs *out, int type, bool *progress) {
    L_C_AC(out, type, progress);
    power_AC(out, progress);
    return *progress;
}

/*  
            | Circuit      | type | parallel |
            | ------------ | ---- | -------- |
            | Series RL    | 1    | 0        |
            | Series RC    | 2    | 0        |
            | Series RLC   | 3    | 0        |
            | Parallel RL  | 1    | 1        |
            | Parallel RC  | 2    | 1        |
            | Parallel RLC | 3    | 1        |

*/