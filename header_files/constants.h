#pragma once 
//Ω
#define PI 3.14159265358979323846
#define MAX_TOKENS 100
#define DEG_TO_RAD(angle) ((angle)*(PI/180.0))
#define RAD_TO_DEG(angle) ((angle)*(180.0/PI))
#define OMEGA(freq) (2 * PI * (freq))
#define COPY(field) ( (known_mask & KN_##field) ? in->field : NAN )