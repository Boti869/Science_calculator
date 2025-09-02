#pragma once
#include"../header_files/help.h"
#include<stdbool.h>

bool volt_AC(Outputs *out , bool *progress);
bool power_AC(Outputs *out, bool *progress);
bool xl_c_AC(Outputs *out, int type, bool *progress);
bool imp_AC(Outputs *out, int type, int parallel, bool *progress);
bool phi_AC(Outputs *out, int type, int parallel, bool *progress);
bool I_AC(Outputs *out, bool *progress);
bool R_AC(Outputs *out,int type, bool *progress);
bool f_ac(Outputs *out, bool *progress);
bool L_C_AC(Outputs *out, int type, bool *progress);
bool Bl_C_AC(Outputs *out, int type, bool *progress);
bool get_AC_values(Outputs *out, int type, int parallel, bool *progress);
int series_setup(int type, int parallel);