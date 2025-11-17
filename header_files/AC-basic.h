#pragma once
#include"../header_files/help.h"
#include<stdbool.h>

bool power_AC(Outputs *out, bool *progress);
bool L_C_AC(Outputs *out, int type, bool *progress);
bool get_AC_values(Outputs *out, int type, bool *progress); 