#include"../header_files/electrical-laws.h"
#include"../header_files/initLaws.h"
#include"../header_files/constants.h"
#include"../header_files/help.h"
#include"../header_files/AC-basic.h"
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

bool validate_AC_inputs(double top, double bottomL, double bottomR, bool *progress) {
    double valid = 1e-6;

    bool t = is_known(top);
    bool bL = is_known(bottomL);
    bool bR = is_known(bottomR);

    if(t && bL && bR) {
        if(fabs(top - (bottomL * bottomR)) > valid) {
            wprintf(L"Input values don't match\n");
            return false;
        }
    }
    return progress;
}

int fix_AC_input(double top, double bottomL, double bottomR, bool *progress) {
    if(!progress){

        wprintf(L"Fixing input values...\n"
                L"Provided values: Top=%.4f, Bottom Left=%.4f, Bottom Right=%.4f\n", top, bottomL, bottomR);
        wprintf(L"First option: Top = %.4f\n", bottomL * bottomR);
        wprintf(L"Or BotttomL = %.4f\n", top / bottomR);
        wprintf(L"Or BottomR = %.4f\n", top / bottomL);
          
    }
    return 0;
}

bool validate_with_phi(double a , double b, double c, double phi, bool *progress) {
    double valid = 1e-6;

    bool A = is_known(a);
    bool B = is_known(b);
    bool C = is_known(c);
    bool PHI = is_known(phi);
    double phi_rad = DEG_TO_RAD(phi);

    if(A && B && C) {
        if(fabs(c - (hypot(a, b))) > valid) {
            wprintf(L"Input values don't check out on %.4f^2 = %.4f^2 + %.4f^2\n", c, a, b);
            return false;
        }
    }
    if(A && B && C && PHI) {
        if(fabs(sin(phi_rad) - (a/c)) > valid) {
            wprintf(L"Input values don't check out on sin(%.4f°) = %.4f/%.4f\n", phi, a, c);
            return false;
        }
        if(fabs(cos(phi_rad) - (b/c)) > valid) {
            wprintf(L"Input values don't check out on cos(%.4f°) = %.4f/%.4f\n", phi, b, c);
            return false;
        }
    }
    return progress;
}
bool check_phi(double a , double b, double c, double phi, bool *progress) {
    double valid = 1e-6;

    bool A = is_known(a);
    bool B = is_known(b);
    bool C = is_known(c);
    bool PHI = is_known(phi);
    double phi_rad = DEG_TO_RAD(phi);

    if(A && B && C && PHI) {
        if(fabs(sin(phi_rad) - (a/c)) > valid) {
            wprintf(L"Input values don't check out on sin(%.4f°) = %.4f/%.4f\n", phi, a, c);
            return false;
        }
        if(fabs(cos(phi_rad) - (b/c)) > valid) {
            wprintf(L"Input values don't check out on cos(%.4f°) = %.4f/%.4f\n", phi, b, c);
            return false;
        }
        if(fabs(tan(phi_rad) - (a/b)) > valid) {
            wprintf(L"Input values don't check out on tan(%.4f°) = %.4f/%.4f\n", phi, a, b);
            return false;
        }
    }
    return progress;
}
bool validate_Xl_C(double X, double f, double L, bool *progress, char type) {
    double valid = 1e-6;

    bool XX = is_known(X);
    bool F = is_known(f);
    bool LL = is_known(L);


    if(XX && F && LL) {
        if(type == 'L') {
            if(fabs(X - (OMEGA(f) * L)) > valid) {
                wprintf(L"Input values don't match %.4f = 2πf%.4f\n", X, L);
                return false;
            }
        }
        else if(type == 'C') {
            if(fabs(X - (1.0 / (OMEGA(f) * L))) > valid) {
                wprintf(L"Input values don't match %.4f = 1/(2πf%.4f)\n", X, L);
                return false;
            }
        }
    }
    return progress;
}            

bool check_triangle(Inputs result, int type, int parallel) {
    bool progress = true;

    while (progress) {
        double X = 0.0;
            if(result.Xl > result.Xc) X = result.Xl - result.Xc;
            else X = result.Xc - result.Xl;

            double V = 0.0;
            if(result.Vl > result.Vc) V = result.Vl - result.Vc;
            else V = result.Vc - result.Vl;

            double I = 0.0;
            if(result.Il > result.Ic) I = result.Il - result.Ic;
            else I = result.Ic - result.Il;

            double B = result.Bl - result.Bc;

            if(type == 1 || type == 3){
                validate_Xl_C(result.Xl, result.f, result.L, &progress, 'L');
            }
            if(type == 2 || type == 3){
                validate_Xl_C(result.Xc, result.f, result.C, &progress, 'C');
            }
           
            validate_AC_inputs(result.S, result.V, result.I, &progress);
            validate_with_phi(result.P, result.Q, result.S, result.phi, &progress);
            if(parallel == 0){
                validate_AC_inputs(result.V, result.Z, result.I, &progress);
                check_phi(result.V, result.Vr, result.V, result.phi, &progress);
                validate_AC_inputs(result.Vr, result.R, result.I, &progress);
                check_phi(result.Vr, result.R, result.Z, result.phi, &progress);
                if(type == 1){
                    validate_AC_inputs(result.Vl, result.Xl, result.I, &progress);
                    validate_with_phi(result.Vl, result.Vr, result.V, result.phi, &progress);
                    check_phi(result.Vl, result.Vr, result.V, result.phi, &progress);
                    validate_with_phi(result.Xl, result.R, result.Z, result.phi, &progress);
                    check_phi(result.Xl, result.R, result.Z, result.phi, &progress);
                }
                else if(type == 2){
                    validate_AC_inputs(result.Vc, result.Xc, result.I, &progress);
                    validate_with_phi(result.Vc, result.Vr, result.V, result.phi, &progress);
                    check_phi (result.Vc, result.Vr, result.V, result.phi, &progress);
                    validate_with_phi(result.Xc, result.R, result.Z, result.phi, &progress);
                    check_phi(result.Xc, result.R, result.Z, result.phi, &progress);
                }
                else if(type == 3){
                    validate_AC_inputs(result.Vl, result.Xl, result.I, &progress);
                    validate_AC_inputs(result.Vc, result.Xc, result.I, &progress);
                    validate_with_phi(V, result.Vr, result.V, result.phi, &progress);
                    check_phi(V, result.Vr, result.V, result.phi, &progress);
                    validate_with_phi(X, result.R, result.Z, result.phi, &progress);
                    check_phi(X, result.R, result.Z, result.phi, &progress);
                } 
            }
            else{
                validate_AC_inputs(result.I, result.Y, result.V, &progress);
                check_phi(result.I, result.Y, result.V, result.phi, &progress);
                validate_AC_inputs(result.Ir, result.G, result.V, &progress);
                if(result.G != 0.0){
                    validate_AC_inputs(1.0 / result.R, result.G, 1.0, &progress);
                }
                if(type == 1){
                    validate_AC_inputs(result.Il, result.Bl, result.V, &progress);
                    validate_with_phi(result.Il, result.Ir, result.I, result.phi, &progress);
                    check_phi(result.Il, result.Ir, result.I, result.phi, &progress);
                    validate_with_phi(result.Bl, result.G, result.Y, result.phi, &progress);
                    check_phi(result.Bl, result.G, result.Y, result.phi, &progress);
                    if(result.Bl != 0.0){
                        validate_AC_inputs(1.0 / result.Xl, result.Bl, 1.0, &progress);
                    }
                }
                else if(type == 2){
                    validate_AC_inputs(result.Ic, result.Bc, result.V, &progress);
                    validate_with_phi(result.Ic, result.Ir, result.I, result.phi, &progress);
                    check_phi(result.Ic, result.Ir, result.I, result.phi, &progress);
                    validate_with_phi(result.Bc, result.G, result.Y, result.phi, &progress);
                    check_phi(result.Bc, result.G, result.Y, result.phi, &progress);
                    if(result.Bc != 0.0){
                        validate_AC_inputs(1.0 / result.Xc, result.Bc, 1.0, &progress);
                    }
                }
                else if(type == 3){
                    validate_AC_inputs(result.Il, result.Bl, result.V, &progress);
                    validate_AC_inputs(result.Ic, result.Bc, result.V, &progress);
                    validate_with_phi(I, result.Ir, result.I, result.phi, &progress);
                    check_phi(I, result.Ir, result.I, result.phi, &progress);
                    validate_with_phi(B, result.G, result.Y, result.phi, &progress);
                    check_phi(B, result.G, result.Y, result.phi, &progress);
                    if(result.Bl != 0.0){
                        validate_AC_inputs(1.0 / result.Xl, result.Bl, 1.0, &progress);
                    }
                    if(result.Bc != 0.0){
                        validate_AC_inputs(1.0 / result.Xc, result.Bc, 1.0, &progress);
                    }

                }
            }
        break;
    }
    return progress;
}

int valid_choice(int choice){
    while(choice < 1 || choice > 3){
        wprintf(L"Invalid choice, please choose 1, 2 or 3\n");
        scanf("%d", &choice);
        getchar(); 
    }
    return choice;
}