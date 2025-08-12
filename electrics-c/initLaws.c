#include"../header_files/electrical-laws.h"
#include"../header_files/initLaws.h"
#include<stdio.h>
#include<stdlib.h>

double initOhmLaws() {
    double voltage, resistance, current;
    while (1) {
    printf("Enter numbers: Voltage, Resistance, Current (with spaces)\n"
           "Enter 0 in place of unknown:");
    scanf("%lf %lf %lf", &voltage, &resistance, &current);
    getchar();
    if(resistance < 0) {
        printf("Resistance cannot be negative in Ohm's Law\n");
        continue; 
    }
    return applyOhmLaw(voltage, resistance, current);
    }
}