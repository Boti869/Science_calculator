#include"../header_files/electrical-laws.h"
#include"../header_files/initLaws.h"
#include"../header_files/constants.h"
#include"../header_files/help.h"
#include"../header_files/AC-basic.h"
#include"../header_files/input_check.h"
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

int initOhmLaws() {
    char buffer[100];
    
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
            fix_AC_input(voltage, resistance, current, false);
            continue;
        }
        if(validate_AC_inputs(power, voltage, current, &progress) == false){
            fix_AC_input(power, voltage, current, false);
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
    int ch = valid_choice(choice);
    switch (ch) {
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
    int ch = valid_choice(choice);
    switch (ch) {
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