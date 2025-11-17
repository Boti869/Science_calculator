#include"../../header_files/electrical-laws.h"
#include"../../header_files/constants.h"
#include"../../header_files/help.h"
#include"../../header_files/AC-basic.h"
#include"../../header_files/Parallel-AC.h"
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<wchar.h>
#include<locale.h>
#include<stdbool.h>

// ---------- Individual Amperes ----------
bool indiv_amp_AC(Outputs *out, int type, bool *progress){  
    if(!is_known(out->Ir)){ 
        if(is_known(out->I) && is_known(out->phi))
            out->Ir = out->I * cos(DEG_TO_RAD(out->phi));
        else if(is_known(out->V) && is_known(out->R))
            out->Ir = out->V * out->G;
        else return *progress;
        
        *progress = true;
    }
    if(!is_known(out->Il) && (type == 1 || type ==3)){
        if(is_known(out->I) && is_known(out->phi) && type ==1)
            out->Il = out->I * sin(DEG_TO_RAD(out->phi));
        else if(is_known(out->V) && is_known(out->Bl))
            out->Il = out->V * out->Bl;
        else return *progress;
            *progress = true;
    }
    if(!is_known(out->Ic) && (type == 2 || type ==3)){
        if(is_known(out->I) && is_known(out->phi) && type ==2)
            out->Ic = out->I * sin(DEG_TO_RAD(out->phi));
        else if(is_known(out->V) && is_known(out->Bc))
            out->Ic = out->V * out->Bc;
        else return *progress;
        *progress = true;
    }
    return *progress;
}

// ---------- Phase Angle ----------
bool parallel_phi_AC(Outputs *out, int type, bool *progress){
    if (is_known(out->phi)) return *progress;

    double phi_rad = 0;
    bool ok = false;

        // PARALLEL RL
        if (type == 1) {
            if (is_known(out->G) && is_known(out->Bl)) {
                phi_rad = atan2(out->Bl, out->G);
                ok = true;
            }
            else if (is_known(out->Y) && is_known(out->G)) {
                phi_rad = acos(out->G / out->Y);
                ok = true;
            }
            else if (is_known(out->Y) && is_known(out->Bl)) {
                phi_rad = asin(out->Bl / out->Y);
                ok = true;
            }
            else return *progress;
        }

        // PARALLEL RC
        else if (type == 2) {
            if (is_known(out->G) && is_known(out->Bc)) {
                phi_rad = -atan2(out->Bc, out->G);
                ok = true;
            }
            else if (is_known(out->Y) && is_known(out->G)) {
                phi_rad = acos(out->G / out->Y);
                ok = true;
            }
            else if (is_known(out->Y) && is_known(out->Bc)) {
                phi_rad = -asin(out->Bc / out->Y);
                ok = true;
            }
            else return *progress;
        }

        // PARALLEL RLC
        else if (type == 3) {
            double B;
            if(out->Bl > out->Bc) B = out->Bl - out->Bc;
            else B = out->Bc - out->Bl;

            if (is_known(out->G) && is_known(B)) {
                phi_rad = atan2(B, out->G);
                ok = true;
            }
            else if (is_known(out->Y) && is_known(out->G)) {
                phi_rad = acos(out->G / out->Y);
                ok = true;
            }
            else if (is_known(out->Y) && is_known(B)) {
                phi_rad = asin(B / out->Y);
                ok = true;
            }
            else return *progress;
        }

    if (ok) {
        out->phi = RAD_TO_DEG(phi_rad);
        *progress = true;
    }
    else {return *progress;}
    
    out->phi_deg = (int)out->phi;
    out->phi_min = (int)((out->phi - out->phi_deg) * 60);
    out->phi_sec = (int)((((out->phi - out->phi_deg) * 60) - out->phi_min) * 60);

    return *progress;
}

// ---------- Admittances ----------
bool Admittances_AC(Outputs *out, int type, bool *progress){
    
    // G <--> R
    if(!is_known(out->G) && is_known(out->R)){
        out->G = 1.0 / out->R;
        *progress = true;
    }
    if(is_known(out->G) && !is_known(out->R)){
        out->R = 1.0 / out->G;
        *progress = true;
    }

    // Xl <--> Bl
    if((type == 1 || type ==3)){
        if(!is_known(out->Bl) && is_known(out->Xl)){
            out->Bl = 1.0 / out->Xl;
            *progress = true;
        }
        else if(!is_known(out->Xl) && is_known(out->Bl)){
            out->Xl = 1.0 / out->Bl;
            *progress = true;
        }
        else return *progress;
    }

    // Xc <--> Bc
    if((type == 2 || type ==3)){
        if(!is_known(out->Bc) && is_known(out->Xc)){
            out->Bc = 1.0 / out->Xc;
            *progress = true;
        }
        else if(!is_known(out->Xc) && is_known(out->Bc)){
            out->Xc = 1.0 / out->Bc;
            *progress = true;
        }
        else return *progress;
    }

    // Y
    double B = out->Bl - out->Bc;

    // In RL
    if(type == 1){
        if(!is_known(out->Y) && is_known(out->G) && is_known(out->Bl)){
            out->Y = hypot(out->G, out->Bl);
            *progress = true;
        }
        else if(!is_known(out->Y) && is_known(out->phi) && is_known(out->G)){
            out->Y = out->G / cos(DEG_TO_RAD(out->phi));
            *progress = true;
        }
        else if(!is_known(out->Y) && is_known(out->phi) && is_known(out->Bl)){
            out->Y = out->Bl / sin(DEG_TO_RAD(out->phi));
            *progress = true;
        }
        else if(is_known(out->Y) && !is_known(out->G) && is_known(out->Bl)){
            out->G = sqrt(out->Y*out->Y - out->Bl*out->Bl);
            *progress = true;
        }
        else if(is_known(out->Y) && is_known(out->G) && !is_known(out->Bl)){
            out->Bl = sqrt(out->Y*out->Y - out->G*out->G);
            *progress = true;
        }
        else return *progress;
    }

    //In RC
    else if(type == 2){
        if(!is_known(out->Y) && is_known(out->G) && is_known(out->Bc)){
            out->Y = hypot(out->G, out->Bc);
            *progress = true;
        }
        else if(!is_known(out->Y) && is_known(out->phi) && is_known(out->G)){
            out->Y = out->G / cos(DEG_TO_RAD(out->phi));
            *progress = true;
        }
        else if(!is_known(out->Y) && is_known(out->phi) && is_known(out->Bc)){
            out->Y = out->Bc / sin(DEG_TO_RAD(out->phi));
            *progress = true;
        }
        else if(is_known(out->Y) && !is_known(out->G) && is_known(out->Bc)){
            out->G = sqrt(pow(out->Y,2) - pow(out->Bc,2));
            *progress = true;
        }
        else if(is_known(out->Y) && is_known(out->G) && !is_known(out->Bc)){
            out->Bc = sqrt(pow(out->Y,2) - pow(out->G,2));
            *progress = true;
        }
        else return *progress;
    }

    // In RLC
    else if(type == 3){
        if(!is_known(out->Y) && is_known(out->G) && is_known(B)){
            out->Y = hypot(out->G, B);
            *progress = true;
        }
        else if(!is_known(out->Y) && is_known(out->phi) && is_known(out->G)){
            out->Y = out->G / cos(DEG_TO_RAD(out->phi));
            *progress = true;
        }
        else if(!is_known(out->Y) && is_known(out->phi) && is_known(B)){
            out->Y = B / sin(DEG_TO_RAD(out->phi));
            *progress = true;
        }
        else if(is_known(out->Y) && !is_known(out->G) && is_known(B)){
            out->G = sqrt(pow(out->Y,2) - B*B);
            *progress = true;
        }
        else if(is_known(out->Y) && is_known(out->G) && !is_known(B)){
            B = sqrt(pow(out->Y,2) - pow(out->G,2));
            *progress = true;
        }
        else return *progress;
    }

    return *progress;
}

// ---------- Voltage ----------
bool parallel_volt_AC(Outputs *out, bool *progress){
    if(!is_known(out->V)){
        if(is_known(out->I) && is_known(out->Y)){
            out->V = out->I / out->Y;
            *progress = true;
        }
        else if(is_known(out->P) && is_known(out->I) && is_known(out->phi)){
            out->V = out->P / (out->I * cos(DEG_TO_RAD(out->phi)));
            *progress = true;
        }
        else if(is_known(out->S) && is_known(out->I)){
            out->V = out->S / out->I;
            *progress = true;
        }
        else if(is_known(out->Q) && is_known(out->I) && is_known(out->phi)){
            out->V = out->Q / (out->I * sin(DEG_TO_RAD(out->phi)));
            *progress = true;
        }
        else return *progress;
    }
    return *progress;
}

// ---------- Current ----------
bool parallel_current_AC(Outputs *out, bool *progress){
    if(!is_known(out->I)){
        if(is_known(out->V) && is_known(out->Y)){
            out->I = out->Y * out->V;
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

// ---------- Xl and Xc ----------
bool parallel_xl_c_AC(Outputs *out, int type, bool *progress){
    if(type == 1 || type ==3){
        if(!is_known(out->Xl)){
            if(is_known(out->V) && is_known(out->Il)){
                out->Xl = out->V / out->Il;
                *progress = true;
            }
            else if(is_known(out->L) && is_known(out->f)){
                out->Xl = OMEGA(out->f) * out->L;
                *progress = true;
            }
            else return *progress;
        }
    }
    if(type == 2 || type ==3){
        if(!is_known(out->Xc)){
            if(is_known(out->V) && is_known(out->Ic)){
                out->Xc = out->V / out->Ic;
                *progress = true;
            }
            else if(is_known(out->C) && is_known(out->f)){
                out->Xc = 1.0 / (OMEGA(out->f) * out->C);
                *progress = true;
            }
            else return *progress;
        }
    }
    return *progress;
}
// ---------- Driver ----------
bool solve_parallel_AC(Outputs *out, int type, bool *progress){
    Admittances_AC(out, type, progress);
    indiv_amp_AC(out, type, progress);
    parallel_volt_AC(out, progress);
    parallel_current_AC(out, progress);
    parallel_phi_AC(out, type, progress);
    parallel_xl_c_AC(out, type, progress);
    get_AC_values(out, type, progress);
    return *progress;
}