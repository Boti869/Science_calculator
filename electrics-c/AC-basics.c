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

bool volt_AC(Outputs *out, bool progress) {
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

    return progress;
}

bool power_AC(Outputs *out, bool progress) {
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
        return progress;
}

bool xl_AC(Outputs *out, bool progress) {
    // Xl from L and f
        if (out->Xl == 0 && out->L != 0 && OMEGA(out->f) != 0) {
            out->Xl = OMEGA(out->f) * out->L; progress = true;
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
    return progress;
}

bool imp_AC(Outputs *out, int type, bool progress) {
    if(type == 1){
        // Z from V and I
        if (out->Z == 0 && out->V != 0 && out->I != 0) {
            out->Z = out->V / out->I; progress = true;
        }
        // Z from R and Xl
        if (out->Z == 0 && out->R != 0 && out->Xl != 0) {
            out->Z = sqrt(pow(out->R, 2) + pow(out->Xl, 2)); progress = true;
        }}
    if(type == 2){
        // Z from V and I
        if (out->Z == 0 && out->V != 0 && out->I != 0) {
            out->Z = out->V / out->I; progress = true;
        }
        // Z from R and Xc
        if (out->Z == 0 && out->R != 0 && out->Xc != 0) {
            out->Z = sqrt(pow(out->R, 2) + pow(out->Xc, 2)); progress = true;
        }} 
    return progress;
}

bool phi_AC(Outputs *out, int type, bool progress) {
    if(type == 1){
        // phi from R & Xl
        if (out->phi == 0 && out->R != 0 && out->Xl != 0) {
            if (out->Xl != 0) {
                out->phi = RAD_TO_DEG(atan2(out->Xl, out->R)); progress = true;
            } else {
                out->phi = 0; // Xl=0 means no phase shift
            }
        }
        //phi from Z & R
        if (out->phi == 0 && out->Z != 0 && out->R != 0) {
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
    }
    if(type == 2){
        // phi from R & Xc
        if (out->phi == 0 && out->R != 0 && out->Xc != 0) {
            if (out->Xc != 0) {
                out->phi = RAD_TO_DEG(atan2(-out->Xc, out->R)); progress = true;
            } else {
                out->phi = 0; 
            }
        }
        //phi from Z & R
        if (out->phi == 0 && out->Z != 0 && out->R != 0) {
            if (out->Z != 0) {
                out->phi = RAD_TO_DEG(acos(out->R / out->Z)); progress = true;
            } else {
                out->phi = 0;
            }
        }
        // phi from Z & Xc
        if (out->phi == 0 && out->Xc != 0 && out->Z != 0) {
            if (out->Z != 0) {
                out->phi = RAD_TO_DEG(asin(-out->Xc / out->Z)); progress = true;
            } else {
                out->phi = 0; 
            }
        }
    }
    return progress;
}

bool I_AC(Outputs *out, bool progress) {
     // I from V and Z
        if (out->I == 0 && out->V != 0 && out->Z != 0) {
            out->I = out->V / out->Z; progress = true;
        }
        // I from S and V
        if (out->I == 0 && out->S != 0 && out->V != 0) {
            out->I = out->S / out->V; progress = true;
        }
        // I from P and V (if phi known)
        if (out->I == 0 && out->P != 0 && out->V != 0) {
            out->I = out->P / out->V / cos(DEG_TO_RAD(out->phi)); progress = true;
        }
        // I from Q and V (if phi known)
        if (out->I == 0 && out->Q != 0 && out->V != 0) {
            out->I = out->Q / out->V / sin(DEG_TO_RAD(out->phi)); progress = true;
        }
    return progress;
}

bool R_AC(Outputs *out, int type, bool progress) {
    if(type == 1){
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
    }
    if(type == 2){
        // R from Z and Xc
        if (out->R == 0 && out->Z != 0 && out->Xc != 0) {
            out->R = sqrt(pow(out->Z, 2) - pow(out->Xc, 2)); progress = true;
        }
        // R from Z and phi
        if (out->R == 0 && out->Z != 0 && out->phi != 0) {
            out->R = out->Z * cos(DEG_TO_RAD(out->phi)); progress = true;
        }
        // R from Xc and phi
        if (out->R == 0 && out->Xc != 0 && out->phi != 0) {
            out->R = -out->Xc / tan(DEG_TO_RAD(out->phi)); progress = true;
        }
    }
    return progress;
}

bool f_ac(Outputs *out, bool progress) {
    // f from R and Xl (if phi known)
        if (out->f == 0 && out->R != 0 && out->Xl != 0 && out->phi != 0) {
            out->f = out->R * tan(DEG_TO_RAD(out->phi) / out->Xl); progress = true;
        }
    return progress;
}

bool L_C_AC(Outputs *out, int type, bool progress) {
    if(type == 1){
    // L from Xl and f (requires f)
        if (out->L == 0 && out->Xl != 0 && out->f != 0) {
            out->L = out->Xl / OMEGA(out->f); progress = true;
        }}
    if(type == 2){
    // C from Xc and f (requires f)
        if (out->C == 0 && out->Xc != 0 && out->f != 0) {
            out->C = 1 / (OMEGA(out->f) * out->Xc); progress = true;
        }}
    return progress;
}

bool get_AC_values(Outputs *out, int type, bool progress) {
    //XL
        xl_AC(out, progress);

    //I
        I_AC(out, progress);

    //Z
        imp_AC(out, type, progress);

    //L
        L_C_AC(out, type, progress);
    
    //R
        R_AC(out, type, progress);
    
    //PHI    
        phi_AC(out, type, progress);

    //V
        volt_AC(out, progress);
    
    //S, P, Q
        power_AC(out, progress);
        
    //f
        f_ac(out, progress);

    return progress;
}