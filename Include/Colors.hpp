#pragma once
#include "ogldev_math_3d.h"

// Basic colors
extern const float color_black[];
extern const float color_white[];
extern const float color_red[];
extern const float color_lime[];
extern const float color_blue[];
extern const float color_yellow[];
extern const float color_cyan[];
extern const float color_magenta[];

// Aditional colors
extern const float color_saddle_brown[];

// functions
Vector3f RGBintToRGBnormalizedFloat(int R, int G, int B);
Vector3f RGBnormalizedFloatToRGBint(float R, float G, float B);
Vector3f RGBtoHSV(float R, float G, float B);
Vector3f HSVtoRGB(float H, float S, float V);