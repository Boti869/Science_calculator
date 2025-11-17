#pragma once
#include"../header_files/help.h"
#include<stdbool.h>

bool solve_parallel_AC(Outputs *out, int type, bool *progress);
bool indiv_amp_AC(Outputs *out, int type, bool *progress); //
bool parallel_volt_AC(Outputs *out, bool *progress); //
bool parallel_current_AC(Outputs *out, bool *progress);//
bool parallel_phi_AC(Outputs *out, int type, bool *progress); //
bool Admittances_AC(Outputs *out, int type, bool *progress);//
bool parallel_xl_c_AC(Outputs *out, int type, bool *progress); //
