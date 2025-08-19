#include"../header_files/AC.h"
#include"../header_files/calc.h"
#include"../header_files/initLaws.h"
#include"../header_files/constants.h"
#include"../header_files/help.h"
#include"../header_files/electrical-laws.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <locale.h>

int RLseries() {
    char buffer[256];
    
    wprintf(L"Enter all 0s to exit\n"
            "enter 0 in place of unknown\n");
    while(1){
    wprintf(L"Enter values for |   V   |   f   |   R   |   L   |   Xl   |   Z   |   I   |   S   |   Q   |   P   |   phi   (with spaces): \n"); 
    fgets(buffer, sizeof(buffer), stdin);
    Inputs result = RLsCalc(buffer);

    double  V = result.V, 
            f = result.f,       
            R = result.R, 
            L = result.L, 
            Xl = result.Xl, 
            Z = result.Z, 
            I = result.I, 
            S = result.S,
            Q = result.Q,
            P = result.P, 
            phi = DEG_TO_RAD(result.phi);        
    
    if (V == 0 && f == 0 && R == 0 && L == 0 && Xl == 0 && Z == 0 && I == 0 && P == 0 && phi == 0) {
        wprintf(L"Exiting...\n");
        return 0; 
    }

    int nozero = 0;
    if (V != 0) nozero++; 
    if (f != 0) nozero++;
    if (R != 0) nozero++; 
    if (L != 0) nozero++; 
    if (Xl != 0) nozero++; 
    if (Z != 0) nozero++; 
    if (I != 0) nozero++; 
    if (S != 0) nozero++; 
    if (phi != 0) nozero++; 
    
    if (nozero < 3) {
        wprintf(L"Not enough data provided.\n");
        fflush(stdin);
        continue; 
        }
        
    if(R < 0 || L < 0 || Xl < 0 || Z < 0 || S < 0 || phi < 0 || f < 0) {
        wprintf(L"Only V and I can be negative\n");
        fflush(stdin);
        continue; 
        }
    
    solve_rl(&result, (Outputs *)&result);
    }

};