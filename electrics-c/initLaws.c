#include"../header_files/electrical-laws.h"
#include"../header_files/initLaws.h"
#include"../header_files/constants.h"
#include"../header_files/help.h"
#include"../header_files/AC-basic.h"
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

int initOhmLaws() {
    char buffer[100];
    
    wprintf(L"0 0 0 0 to exit\n");
    while (1) {
        wprintf(L"                            Voltage | Resistance | Current | Power\n"
                L"COMMA to separate values, eg.: V=230, I=2\n");
        fgets(buffer, 100, stdin);
        buffer[strcspn(buffer, "\n")] = '\0';
    
        Type input = value_of(buffer);
        
        double voltage = input.voltage, resistance = input.resistance, current = input.current, power = input.power;
        if (voltage == 0 && resistance == 0 && current == 0 && power == 0) {
            wprintf(L"Exiting....\n");
            return 0; 
        }

        int nozero = 0;
        if (voltage != 0.00) nozero++;
        if (resistance != 0.00) nozero++;
        if (current != 0.00) nozero++;
        if (power != 0.00) nozero++;
        
        if(nozero < 2 ){
            wprintf(L"Not enough data provided.\n");
            fflush(stdin);
            continue; 
        }
        if (resistance < 0 || power < 0) {
            wprintf(L"Resistance or power cannot be negative in Ohm's Law\n");
            continue; 
        }
        if (voltage != 0 && resistance != 0 && current != 0 && power != 0) {
            wprintf(L"All values provided, nothing to calculate.\n");
            wprintf(L"Checking for consistency...\n");
        }
            bool progress = true;
        if(validate_AC_inputs(voltage, resistance, current, &progress) == false){
            continue;
        }
        if(validate_AC_inputs(power, voltage, current, &progress) == false){
            continue;
        }

        applyOhmLaw(voltage, resistance, current, power);
    }
}

// ---------- Setup ----------
int series_setup(int type, int parallel) {
    char buffer[256];
    wprintf(L"Enter 'quit' or 'exit' to exit\n"
        L"COMMA to separate values, eg.: V = 230, I=2, Q=115\n");

    while (1) {
        // Pick prompt dynamically
        if (type == 1 && parallel == 0)
            wprintf(L"\nEnter values for  V | Vr | Vl | f | R | L | Xl | Z | I | S | Q | P | phi \n");
        else if (type == 1 && parallel == 1)
            wprintf(L"\nEnter values for  V | f | R | L | Xl | Bl | Y | I | Ir | Il | S | Q | P | phi \n");
        else if (type == 2 && parallel == 0)
            wprintf(L"\nEnter values for  V | Vr | Vc | f | R | C | Xc | Z | I | S | Q | P | phi \n");
        else if (type == 2 && parallel == 1)
            wprintf(L"\nEnter values for  V | f | R | C | Xc | Bc | Y | I | Ir | Ic | S | Q | P | phi \n");
        else if (type == 3 && parallel == 0)
            wprintf(L"\nEnter values for  V | Vr | Vl | Vc | f | R | C | Xc | L | Xl | Z | I | S | Q | P | phi \n");
        else if (type == 3 && parallel == 1)
            wprintf(L"\nEnter values for  V | f | R | G | C | Xc | Bc | L | Xl | Bl | Y | I | Ir | Il | Ic | S | Q | P | phi \n");

        fgets(buffer, sizeof(buffer), stdin);
        if (buffer[strlen(buffer) - 1] == '\n') buffer[strlen(buffer) - 1] = '\0';

        Inputs result = {0};
        if (parallel == 0)
            switch(type){
                case 1: result = RLsCalc(buffer); break;
                case 2: result = RCsCalc(buffer); break;
                case 3: result = RLCsCalc(buffer); break;
            }
        else if (parallel == 1)
            switch(type){
                case 1: result = RLpCalc(buffer); break;
                case 2: result = RCpCalc(buffer); break;
                case 3: result = RLCpCalc(buffer); break;
            }
        
        if (strcmp(buffer, "exit") == 0 || strcmp(buffer, "quit") == 0) {
            wprintf(L"Exiting...\n");
            return 0;
        }

        int nozero = nozero_count(&result);
        
        if (nozero < 2) {
            wprintf(L"Not enough data provided.\n");
            continue;
        }

        if ((result.R < 0) || (result.L < 0) || (result.Xl < 0) || (result.Z < 0) ||
            (result.S < 0) || (result.f < 0) || (result.C < 0) || (result.Xc < 0) || 
            (result.P < 0) || (result.Q < 0) || (result.G < 0) || (result.Bc < 0) || 
            (result.Bl < 0 || (result.Y < 0))) {
            wprintf(L"Only V and I can be negative.\n");
            continue;
        }
        if(result.phi > 90.00){
            wprintf(L"Phase angle cannot be greater than 90 degrees in RLC circuits.\n");
            continue;
        }
        
        if(check_triangle(result, type, parallel) == false){
            continue;
        }
        
        // Solve
        if (parallel == 0) {
            if (type == 1) solve_circuit(&result, (Outputs *)&result, SERIES_RL);
            else if (type == 2) solve_circuit(&result, (Outputs *)&result, SERIES_RC);
            else solve_circuit(&result, (Outputs *)&result, SERIES_RLC);
        
        } else {
            if (type == 1) solve_circuit(&result, (Outputs *)&result, PARALLEL_RL);
            else if (type == 2) solve_circuit(&result, (Outputs *)&result, PARALLEL_RC);
            else solve_circuit(&result, (Outputs *)&result, PARALLEL_RLC);
        }
    }
}

int ACseries() {
    wprintf(L"What components are in series?\n");
    wprintf(   L"1. R and L\n"
                "2. R and C\n"
                "3. R L and C\n");

    int choice;
    scanf("%d", &choice);
    getchar(); 
    switch (choice) {
        case 1: 
            series_setup(1, 0);
            break;
        case 2:
            series_setup(2, 0);
            break;
        case 3:
            series_setup(3, 0);
            break;
    }

    return 0;
}

int ACparallel() {
    wprintf(L"What components are in parallel?\n");
    wprintf(   L"1. R and L\n"
                "2. R and C\n"
                "3. R L and C\n");

    int choice;
    scanf("%d", &choice);
    getchar(); 
    switch (choice) {
        case 1: 
            series_setup(1, 1);
            break;
        case 2:
            series_setup(2, 1);
            break;
        case 3:
            series_setup(3, 1);
            break;
    }

    return 0;
}
int nozero_count(Inputs *in){
    int count = 0;
    #define CHECK(x) if(is_known(in->x)) count++

    CHECK(V);
    CHECK(Vr);
    CHECK(Vl);
    CHECK(Vc);
    CHECK(f);
    CHECK(R);
    CHECK(G);
    CHECK(L);
    CHECK(Xl);
    CHECK(Bl);
    CHECK(Z);
    CHECK(Y);
    CHECK(I);
    CHECK(Ir);
    CHECK(Il);
    CHECK(Ic);
    CHECK(S);
    CHECK(Q);
    CHECK(P);
    CHECK(phi);
    CHECK(C);
    CHECK(Xc);
    CHECK(Bc);

    #undef CHECK

    return count;
}

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

bool validate_with_phi(double a , double b, double c, double phi, bool *progress) {
    double valid = 1e-6;

    bool A = is_known(a);
    bool B = is_known(b);
    bool C = is_known(c);
    bool PHI = is_known(phi);
    double phi_rad = DEG_TO_RAD(phi);

    if(A && B && C) {
        if(fabs(c - (hypot(a, b))) > valid) {
            wprintf(L"Input values don't check out on a2 + b2 = c2\n");
            return false;
        }
    }
    if(A && B && C && PHI) {
        if(fabs(sin(phi_rad) - (a/c)) > valid) {
            wprintf(L"Input values don't check out on sin(phi) = a/c\n");
            return false;
        }
        if(fabs(cos(phi_rad) - (b/c)) > valid) {
            wprintf(L"Input values don't check out on cos(phi) = b/c\n");
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
                wprintf(L"Input values don't match Xl = 2πfL\n");
                return false;
            }
        }
        else if(type == 'C') {
            if(fabs(X - (1.0 / (OMEGA(f) * L))) > valid) {
                wprintf(L"Input values don't match Xc = 1/(2πfC)\n");
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
                validate_AC_inputs(result.Vr, result.R, result.I, &progress);
                if(type == 1){
                    validate_AC_inputs(result.Vl, result.Xl, result.I, &progress);
                    validate_with_phi(result.Vl, result.Vr, result.V, result.phi, &progress);
                    validate_with_phi(result.Xl, result.R, result.Z, result.phi, &progress);
                }
                else if(type == 2){
                    validate_AC_inputs(result.Vc, result.Xc, result.I, &progress);
                    validate_with_phi(result.Vc, result.Vr, result.V, result.phi, &progress);
                    validate_with_phi(result.Xc, result.R, result.Z, result.phi, &progress);
                }
                else if(type == 3){
                    validate_AC_inputs(result.Vl, result.Xl, result.I, &progress);
                    validate_AC_inputs(result.Vc, result.Xc, result.I, &progress);
                    validate_with_phi(V, result.Vr, result.V, result.phi, &progress);
                    validate_with_phi(X, result.R, result.Z, result.phi, &progress);
                } 
            }
            else{
                validate_AC_inputs(result.I, result.Y, result.V, &progress);
                validate_AC_inputs(result.Ir, result.G, result.V, &progress);
                if(result.G != 0.0){
                    validate_AC_inputs(1.0 / result.R, result.G, 1.0, &progress);
                }
                if(type == 1){
                    validate_AC_inputs(result.Il, result.Bl, result.V, &progress);
                    validate_with_phi(result.Il, result.Ir, result.I, result.phi, &progress);
                    validate_with_phi(result.Bl, result.G, result.Y, result.phi, &progress);
                    if(result.Bl != 0.0){
                        validate_AC_inputs(1.0 / result.Xl, result.Bl, 1.0, &progress);
                    }
                }
                else if(type == 2){
                    validate_AC_inputs(result.Ic, result.Bc, result.V, &progress);
                    validate_with_phi(result.Ic, result.Ir, result.I, result.phi, &progress);
                    validate_with_phi(result.Bc, result.G, result.Y, result.phi, &progress);
                    if(result.Bc != 0.0){
                        validate_AC_inputs(1.0 / result.Xc, result.Bc, 1.0, &progress);
                    }
                }
                else if(type == 3){
                    validate_AC_inputs(result.Il, result.Bl, result.V, &progress);
                    validate_AC_inputs(result.Ic, result.Bc, result.V, &progress);
                    validate_with_phi(I, result.Ir, result.I, result.phi, &progress);
                    validate_with_phi(B, result.G, result.Y, result.phi, &progress);
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