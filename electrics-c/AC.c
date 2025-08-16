#include"../header_files/calc.h"
#include"../header_files/initLaws.h"
#include"../header_files/constants.h"
#include"../header_files/help.h"
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
    wprintf(L"Enter values for |   V   |   R   |   L   |   Xl   |   Z   |   I   |   P   |   phi   (with spaces): "); 
    fgets(buffer, sizeof(buffer), stdin);
    RLseriesType result = RLsCalc(buffer);

    double V = result.V, 
           R = result.R, 
           L = result.L, 
           Xl = result.Xl, 
           Z = result.Z, 
           I = result.I, 
           P = result.P, 
           phi = DEG_TO_RAD(result.phi);        
    
    int nozero = 0;
    if (V != 0) nozero++; 
    if (R != 0) nozero++; 
    if (L != 0) nozero++; 
    if (Xl != 0) nozero++; 
    if (Z != 0) nozero++; 
    if (I != 0) nozero++; 
    if (P != 0) nozero++; 
    if (phi != 0) nozero++; 
    if (nozero < 2) {
        wprintf(L"Not enough data provided.\n");
        fflush(stdin);
        return 1; 
        }
        
        
    }   
};