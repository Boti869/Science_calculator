#include"../header_files/electrical-laws.h"
#include"../header_files/initLaws.h"
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

double initOhmLaws() {
    char buffer[100];
    double voltage, resistance, current;
    while (1) {
    wprintf(L"Enter numbers: Voltage, Resistance, Current (with spaces)\n"
           "Enter 0 in place of unknown:");
    fgets(buffer, 100, stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    voltage = atof(strtok(buffer, " "));
    resistance = atof(strtok(NULL, " "));
    current = atof(strtok(NULL, " "));
    if(resistance < 0) {
        wprintf(L"Resistance cannot be negative in Ohm's Law\n");
        continue; 
    }
    else if((voltage || current || resistance) == '\0'){
        continue; 
    }
    return applyOhmLaw(voltage, resistance, current);
    }
}