//gcc main.c calcs/help.c calcs/Science-calc.c calcs/algebra.c electrics-c/electrical-laws.c electrics-c/initLaws.c electrics-c/AC-basics.c electrics-c/RLC_circuits/Series.c electrics-c/RLC_circuits/Parallel.c -o mycalc
#include"header_files/electrical-laws.h"
#include"header_files/initLaws.h"
#include"header_files/constants.h"
#include"header_files/calc.h"
#include"header_files/algebra.h"
#include"platform.h"
#include<stdio.h>
#include<stdlib.h>
#include<locale.h>
#include<windows.h>
#include <fcntl.h>
#include <io.h>

int main() {
    init_console();
    int use_case;
    wprintf(L"Select a use case:\n"
        "1. Science calculator\n"
        "2. Basic electrical calculations (V, R, I and P)\n"
        "3. AC series calculators\n"
        "4. AC parallel calculators\n"
        "5. Algebra solver:  BETA, DOESN'T WORK\n");
    
    scanf("%d", &use_case);
    getchar();
    while(use_case < 1 || use_case > 5) {
        wprintf(L"That wasn't on the list. Select a valid one: ");
        scanf("%d", &use_case);
        getchar();
    }
    switch(use_case) {
        case 1: sciCalc(); break;
        case 2: initOhmLaws(); break;
        case 3: ACseries(); break;
        case 4: ACparallel(); break;
        case 5: algebraCalc(); break;
    }
    return 0;
}