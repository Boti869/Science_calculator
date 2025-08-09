#include"../header_files/electrical-laws.h"
#include"../header_files/constants.h"
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

double applyOhmLaw(double voltage, double resistance, double current) {
        if (voltage == 0) {
        voltage = current * resistance;
        printf("Voltage = %f V\n", voltage);
    } else if (current == 0) {
        current = voltage / resistance;
        printf("Current = %f A\n", current);
    } else if (resistance == 0) {
        resistance = voltage / current;
        printf("Resistance = %f Ω\n", resistance);
    } else {
        printf("All values provided, nothing to calculate.\n");
    }
}