#pragma once
#include"../header_files/help.h"

void applyOhmLaw(double voltage, double resistance, double current, double power);
int solve_circuit(const Inputs *in, Outputs *out, CircuitType type);