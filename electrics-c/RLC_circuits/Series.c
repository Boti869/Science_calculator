#include"../../header_files/electrical-laws.h"
#include"../../header_files/constants.h"
#include"../../header_files/help.h"
#include"../../header_files/AC-basic.h"
#include"../../header_files/Series-AC.h"
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<wchar.h>
#include<locale.h>
#include<stdbool.h>

// ---------- Individual Voltages ----------
bool indiv_volt_AC(Outputs *out, int type, bool *progress){
        if(!is_known(out->Vr)){
            if(is_known(out->V) && is_known(out->phi))
                out->Vr = out->V * cos(DEG_TO_RAD(out->phi));
            else if(is_known(out->I) && is_known(out->R))
                out->Vr = out->I * out->R;
            else return *progress;
        *progress = true;
        }
        if(!is_known(out->Vl)){
            if(is_known(out->V) && is_known(out->phi) && type ==1)
                    out->Vl = out->V * sin(DEG_TO_RAD(out->phi));
            else if(is_known(out->I) && is_known(out->Xl))
                    out->Vl = out->I * out->Xl;
            else return *progress;
        *progress = true;
        }
        if(!is_known(out->Vc)){
            if(is_known(out->V) && is_known(out->phi) && type == 2)
                    out->Vc = out->V * sin(DEG_TO_RAD(out->phi));
            else if(is_known(out->I) && is_known(out->Xc))
                    out->Vc = out->I * out->Xc;
            else return *progress;
        *progress = true;
        }
    return *progress;
}
// ---------- Current ----------
bool series_current_AC(Outputs *out, bool *progress){
    if(!is_known(out->I)){
        if(is_known(out->V) && is_known(out->Z)){
            out->I = out->V / out->Z;
            *progress = true;
        }
        else if(is_known(out->P) && is_known(out->V) && is_known(out->phi)){
            out->I = out->P / (out->V * cos(DEG_TO_RAD(out->phi)));
            *progress = true;
        }
        else if(is_known(out->S) && is_known(out->V)){
            out->I = out->S / out->V;
            *progress = true;
        }
        else if(is_known(out->Q) && is_known(out->V) && is_known(out->phi)){
            out->I = out->Q / (out->V * sin(DEG_TO_RAD(out->phi)));
            *progress = true;
        }
        else return *progress;
    }
    return *progress;
}
// ---------- Voltage ----------
bool series_volt_AC(Outputs *out, int type, bool *progress){
    if(!is_known(out->V)){
        if(type == 1){
            if(is_known(out->Vr) && is_known(out->Vl)){
                out->V = hypot(out->Vr, out->Vl);
                *progress = true;
            }
            else return *progress;
        }
        else if(type == 2){
            if(is_known(out->Vr) && is_known(out->Vc)){
                out->V = hypot(out->Vr, out->Vc);
                *progress = true;
            }
            else return *progress;
        }
        else if(type == 3){
            if(is_known(out->Vr) && is_known(out->Vl) && is_known(out->Vc)){
                double Vlc = out->Vl - out->Vc;
                out->V = hypot(out->Vr, Vlc);
                *progress = true;
            }
            else return *progress;
        }
        else return *progress;
    }
    return *progress;
}

// ---------- Phase Angle ----------
bool series_phi_AC(Outputs *out, int type, bool *progress){
    if (is_known(out->phi)) return *progress;

    double phi_rad = 0;
    bool ok = false;

        // SERIES RL
        if (type == 1) {
            if (is_known(out->R) && is_known(out->Xl)) {
                phi_rad = atan2(out->Xl, out->R);
                ok = true;
            }
            else if (is_known(out->Z) && is_known(out->R)) {
                phi_rad = acos(out->R / out->Z);
                ok = true;
            }
            else if (is_known(out->Z) && is_known(out->Xl)) {
                phi_rad = asin(out->Xl / out->Z);
                ok = true;
            }
            else return *progress;
        }

        // SERIES RC
        else if (type == 2) {
            if (is_known(out->R) && is_known(out->Xc)) {
                phi_rad = atan2(-out->Xc, out->R);
                ok = true;
            }
            else if (is_known(out->Z) && is_known(out->R)) {
                phi_rad = acos(out->R / out->Z);
                ok = true;
            }
            else if (is_known(out->Z) && is_known(out->Xc)) {
                phi_rad = asin(-out->Xc / out->Z);
                ok = true;
            }
            else return *progress;
        }

        // SERIES RLC
        else if (type == 3) {
            double X = 0.0;
            if (out->Xl > out->Xc) X = out->Xl - out->Xc;
            else X = out->Xc - out->Xl;

            if (is_known(out->R) && is_known(out->Xl) && is_known(out->Xc)) {
                phi_rad = atan2(X, out->R);
                ok = true;
            }
            else if (is_known(out->Z) && is_known(out->R)) {
                phi_rad = acos(out->R / out->Z);
                ok = true;
            }
            else if (is_known(out->Z) && is_known(out->Xl) && is_known(out->Xc)) {
                phi_rad = asin(X / out->Z);
                ok = true;
            }
            else return *progress;
        }

    if (!ok){
        return *progress;
    }
    out->phi = RAD_TO_DEG(phi_rad);
    
    out->phi_deg = (int)out->phi;
    out->phi_min = (int)((out->phi - out->phi_deg) * 60);
    out->phi_sec = (int)((((out->phi - out->phi_deg) * 60) - out->phi_min) * 60);

    *progress = true;
    return *progress;
}

// ---------- Resistance ----------
bool series_R_AC(Outputs *out, int type, bool *progress){
    if(!is_known(out->R)){
        if(is_known(out->Vr) && is_known(out->I)){
            out->R = out->Vr / out->I;
            *progress = true;
        }
        if(type == 1){
            if(is_known(out->Z) && is_known(out->Xl)){
                out->R = sqrt(out->Z*out->Z - out->Xl*out->Xl);
            }
            else if(is_known(out->Xl) && is_known(out->phi)){
                out->R = out->Xl / tan(DEG_TO_RAD(out->phi));
            }
            else return *progress;
            *progress = true;
        
        }
        else if(type == 2){
            if(is_known(out->Z) && is_known(out->Xc)){
                out->R = sqrt(pow(out->Z,2) - pow(out->Xc,2));
            }
            else if(is_known(out->Xc) && is_known(out->phi)){
                out->R = -out->Xc / tan(DEG_TO_RAD(out->phi));
            }
            else return *progress;
            *progress = true;
        }
        else if(type == 3){
            double X = 0.0;
            if(out->Xl > out->Xc) X = out->Xl - out->Xc;
            else X = out->Xc - out->Xl;

            if(is_known(out->Z) && is_known(out->Xl) && is_known(out->Xc)){
                out->R = sqrt(out->Z*out->Z - X*X);
            }
            else if(is_known(out->Xl) && is_known(out->Xc) && is_known(out->phi)){
                out->R = X / tan(DEG_TO_RAD(out->phi));
            }
            else return *progress;
            *progress = true;
        }
        else return *progress;
    }
    return *progress;
}

// ---------- Reactance ----------
bool Reactance_AC(Outputs *out, int type, bool *progress){
    if(type == 1 || type == 3){
        if(!is_known(out->Xl)){
            if(is_known(out->Vl) && is_known(out->I)){
                out->Xl = out->Vl / out->I;
                *progress = true;
            }
            else if(is_known(out->L) && is_known(out->f)){
                out->Xl = OMEGA(out->f) * out->L;
                *progress = true;
            }
            else if(type != 3){
                if(is_known(out->Z) && is_known(out->R)){
                    out->Xl = sqrt(out->Z*out->Z - out->R*out->R);
                    *progress = true;
                }
                else if(is_known(out->Z) && is_known(out->phi)){
                    out->Xl = out->Z * sin(DEG_TO_RAD(out->phi));
                    *progress = true;
                }
                else if(is_known(out->R) && is_known(out->phi)){
                    out->Xl = out->R * tan(DEG_TO_RAD(out->phi));
                    *progress = true;
                }
                else return *progress;
            }
            else return *progress;
        }
    }
    if(type == 2 || type == 3){
        if(!is_known(out->Xc)){
            if(is_known(out->Vc) && is_known(out->I)){
                out->Xc = out->Vc / out->I;
                *progress = true;
            }
            else if(is_known(out->C) && is_known(out->f)){
                out->Xc = 1.0 / (OMEGA(out->f) * out->C);
                *progress = true;
            }
            else if(type != 3){
                if(is_known(out->Z) && is_known(out->R)){
                    out->Xc = sqrt(pow(out->Z,2) - pow(out->R,2));
                    *progress = true;
                }
                else if(is_known(out->Z) && is_known(out->phi)){
                    out->Xc = -out->Z * sin(DEG_TO_RAD(out->phi));
                    *progress = true;
                }
                else if(is_known(out->R) && is_known(out->phi)){
                    out->Xc = -out->R * tan(DEG_TO_RAD(out->phi));
                    *progress = true;
                }
                else return *progress;
            }
            else return *progress;
        }
        if(!is_known(out->Z)){
            if(is_known(out->R) && is_known(out->phi)){
                out->Z = out->R / cos(DEG_TO_RAD(out->phi));
                *progress = true;
            }
            if (type == 1){
                if(is_known(out->R) && is_known(out->Xl)){
                    out->Z = sqrt(pow(out->R,2) + pow(out->Xl,2));
                    *progress = true;
                }
                else if(is_known(out->Xl) && is_known(out->phi)){
                    out->Z = out->Xl / sin(DEG_TO_RAD(out->phi));
                    *progress = true;
                }
                else return *progress;
            }
            else if(type == 2){
                if(is_known(out->R) && is_known(out->Xc)){
                    out->Z = sqrt(pow(out->R,2) + pow(out->Xc,2));
                    *progress = true;
                }
                else if(is_known(out->Xc) && is_known(out->phi)){
                    out->Z = -out->Xc / sin(DEG_TO_RAD(out->phi));
                    *progress = true;
                }
                else return *progress;
            }
            else if(type == 3){
                double X = 0.0;
                if(out->Xl > out->Xc) X = out->Xl - out->Xc;
                else X = out->Xc - out->Xl;
                if(is_known(out->R) && is_known(out->Xl) && is_known(out->Xc)){
                    out->Z = sqrt(pow(out->R,2) + pow(X,2));
                    *progress = true;
                }
                else if(is_known(out->Xl) && is_known(out->Xc) && is_known(out->phi)){
                    out->Z = X / sin(DEG_TO_RAD(out->phi));
                    *progress = true;
                }
                else return *progress;
            }
            else return *progress;
        }
    }
    return *progress;
}

// ---------- Driver ----------
bool solve_series_AC(Outputs *out, int type, bool *progress){
    series_volt_AC(out, type, progress);
    series_current_AC(out, progress);
    series_phi_AC(out, type, progress);
    indiv_volt_AC(out, type, progress);
    Reactance_AC(out, type, progress);
    series_R_AC(out, type, progress);
    get_AC_values(out, type, progress);
    return *progress;
}