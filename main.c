#include"header_files/electrical-laws.h"
#include"header_files/initLaws.h"
#include"header_files/constants.h"
#include"header_files/calc.h"
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

int main() {
    int use_case;
    printf("Select a use case:\n"
        "1. Basic calculator\n"
        "2. Ohm's Law\n");

    scanf("%d", &use_case);
    getchar();

    switch(use_case) {
        case 1: calc(); break;
        case 2: initOhmLaws(); break;
    }
    return 0;
}