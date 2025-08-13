//gcc main.c calcs/help.c calcs/basic-calc.c calcs/Science-calc.c electrics-c/electrical-laws.c electrics-c/initLaws.c -o mycalc
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
    setlocale(LC_ALL, "");
    SetConsoleOutputCP(CP_UTF8);
    _setmode(_fileno(stdout), _O_U8TEXT);
    int use_case;
    wprintf(L"Select a use case:\n"
        "1. Basic calculator\n"
        "2. Science calculator\n"
        "3. Ohm's Law\n");

    scanf("%d", &use_case);
    getchar();

    switch(use_case) {
        case 1: calc(); break;
        case 2: sciCalc(); break;
        case 3: initOhmLaws(); break;
    }
    return 0;
}