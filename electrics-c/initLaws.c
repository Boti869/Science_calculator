#include"../header_files/electrical-laws.h"
#include"../header_files/initLaws.h"
#include"../header_files/constants.h"
#include"../header_files/help.h"
#include"../header_files/AC.h"
#include"../header_files/AC-basic.h"
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

int initOhmLaws() {
    char buffer[100];
    
    wprintf(L"0 0 0 0 to exit\n");
    while (1) {
    wprintf(L"                            Voltage | Resistance | Current | Power |  (with spaces)\n"
            "Enter 0 in place of unknown: ");
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
        wprintf(L"Resistance cannot be negative in Ohm's Law\n");
        continue; 
    }
    if (voltage != 0 && resistance != 0 && current != 0 && power != 0) {
        wprintf(L"All values provided, nothing to calculate.\n");
        continue;
    }
    applyOhmLaw(voltage, resistance, current, power);
    }
}

int ACseries() {
    wprintf(L"What components are in series?\n");
    wprintf(   L"1. R and L\n"
                "2. R and C\n"
                "3. coming soon\n");

    int choice;
    scanf("%d", &choice);
    getchar(); 
    switch (choice) {
        case 1: 
            series_setup(1);
            break;
        case 2:
            series_setup(2);
            break;
        case 3:
            wprintf(L"coming soon\n");
            break;
    }

    return 0;
}