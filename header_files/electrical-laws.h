#pragma once
#include"../header_files/help.h"

void applyOhmLaw(double voltage, double resistance, double current, double power);
int solve_rl(const Inputs *in, Outputs *out);
int solve_rc(const Inputs *in, Outputs *out);