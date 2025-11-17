#pragma once
#include"../header_files/help.h"
#include<stdbool.h>

bool solve_series_AC(Outputs *out, int type, bool *progress);
bool indiv_volt_AC(Outputs *out, int type, bool *progress); //
bool series_volt_AC(Outputs *out, int type, bool *progress);//
bool series_current_AC(Outputs *out, bool *progress);//
bool Reactance_AC(Outputs *out, int type, bool *progress);//
bool series_R_AC(Outputs *out, int type, bool *progress); //
bool series_phi_AC(Outputs *out, int type, bool *progress); //


