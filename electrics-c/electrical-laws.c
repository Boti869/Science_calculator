#include"../header_files/electrical-laws.h"
#include"../header_files/constants.h"
#include"../header_files/help.h"
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<wchar.h>
#include<locale.h>

double applyOhmLaw(double voltage, double resistance, double current) {
        if (voltage == 0) {
        voltage = current * resistance;
        pretty_print(voltage, 'V', 'n');
    } else if (current == 0) {
        current = voltage / resistance;
        pretty_print(current, 'A', 'n');
    } else if (resistance == 0) {
        resistance = voltage / current;
        wchar_t ohm = L'Ω';
        pretty_print(resistance, ohm, 'y');
    } else {
        wprintf(L"All values provided, nothing to calculate.\n");
    }
}