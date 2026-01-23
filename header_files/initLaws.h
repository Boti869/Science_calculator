#pragma once
#include <stdbool.h>
#include"help.h"

int initOhmLaws();
int ACseries();
int ACparallel();
int series_setup(int type, int parallel);
int nozero_count(Inputs *result);


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