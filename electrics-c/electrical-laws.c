#include"../header_files/electrical-laws.h"
#include"../header_files/constants.h"
#include"../header_files/help.h"
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<wchar.h>
#include<locale.h>

void applyOhmLaw(double voltage, double resistance, double current, double power) {
    //Voltage
    if (voltage == 0 && current != 0 && resistance != 0) voltage = current * resistance;
    if (voltage == 0 && power != 0 && current != 0)      voltage = power / current;
    if (voltage == 0 && power != 0 && resistance != 0)   voltage = sqrt(power * resistance);
    
    //Current
    if (current == 0 && voltage != 0 && resistance != 0) current = voltage / resistance;
    if (current == 0 && power != 0 && voltage != 0)      current = power / voltage;
    if (current == 0 && power != 0 && resistance != 0)   current = sqrt(power / resistance);

    //Resistance
    if (resistance == 0 && voltage != 0 && current != 0) resistance = voltage / current;
    if (resistance == 0 && power != 0 && current != 0)   resistance = power / (current * current);
    if (resistance == 0 && voltage != 0 && power != 0)   resistance = (voltage * voltage) / power;

    //Power
    if (power == 0 && voltage != 0 && current != 0)      power = voltage * current;
    if (power == 0 && voltage != 0 && resistance != 0)   power = (voltage * voltage) / resistance;
    if (power == 0 && current != 0 && resistance != 0)   power = resistance * (current * current);
    
    pretty_print(voltage, 'V');
    wprintf(L"\n");
    wchar_t ohm = L'Ω';
    pretty_print(resistance, ohm);
    wprintf(L"\n");
    pretty_print(current, 'A');
    wprintf(L"\n");
    pretty_print(power, 'W');
    wprintf(L"\n");
}