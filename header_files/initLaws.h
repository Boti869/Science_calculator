#pragma once
#include <stdbool.h>

int initOhmLaws();
int ACseries();
int ACparallel();
int series_setup(int type, int parallel);
int nozero_count(Inputs *result);
bool validate_AC_inputs(double top, double bottomL, double bottomR);
bool validate_with_phi(double a , double b, double c, double phi);
bool check_triangle(Inputs in, int type, int parallel);

/*  ____a______ 
    |        /
    |       /
    |      /
    b     /c
    |    /
    |phi/
    |  /
    | /
    |/
*/