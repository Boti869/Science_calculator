#pragma once
#include <stdbool.h>
#include"help.h"

bool validate_AC_inputs(double top, double bottomL, double bottomR, bool *progress);
int fix_AC_input(double top, double bottomL, double bottomR, bool *progress);
bool validate_with_phi(double a , double b, double c, double phi, bool *progress);
bool validate_Xl_C(double Xl, double f, double L, bool *progress, char type);
bool check_triangle(Inputs in, int type, int parallel);
bool check_phi(double a , double b, double c, double phi, bool *progress);
int valid_choice(int choice);