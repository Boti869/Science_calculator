//gcc main.c calcs/help.c calcs/Science-calc.c electrics-c/electrical-laws.c electrics-c/initLaws.c electrics-c/AC-basics.c -o mycalc
#include"header_files/electrical-laws.h"
#include"header_files/initLaws.h"
#include"header_files/constants.h"
#include"header_files/calc.h"
#include<stdio.h>
#include<stdlib.h>
#include<locale.h>
#include<windows.h>
#include <fcntl.h>
#include <io.h>

int main() {
    setlocale(LC_CTYPE, "");
    setlocale(LC_NUMERIC, "C");
    _setmode(_fileno(stdout), _O_U16TEXT); 
    int use_case;
    wprintf(L"Select a use case:\n"
        "1. Science calculator\n"
        "2. Basic electrical calculations (V, R, I and P)\n"
        "3. AC series calculators\n"
        "4. AC parallel calculators\n");
    
    scanf("%d", &use_case);
    getchar();

    switch(use_case) {
        case 1: sciCalc(); break;
        case 2: initOhmLaws(); break;
        case 3: ACseries(); break;
        case 4: ACparallel(); break;
        default: wprintf(L"That wasn't on the list...\n"
                        L"Try again ");
    }
    return 0;
}