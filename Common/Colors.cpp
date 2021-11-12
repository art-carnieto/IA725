#include "Colors.hpp"
#include <algorithm>

// Colors list and values adapted from https://www.rapidtables.com/web/color/RGB_Color.html

// Basic colors
const float color_black[] = { 0.0f, 0.0f, 0.0f };
const float color_white[] = { 1.0f, 1.0f, 1.0f };
const float color_red[] = { 1.0f, 0.0f, 0.0f };
const float color_lime[] = { 0.0f, 1.0f, 0.0f };
const float color_blue[] = { 0.0f, 0.0f, 1.0f };
const float color_yellow[] = { 1.0f, 1.0f, 0.0f };
const float color_cyan[] = { 0.0f, 1.0f, 1.0f };
const float color_magenta[] = { 1.0f, 0.0f, 1.0f };

// Aditional colors
const float color_saddle_brown[] = { 0.5450980392156863f, 0.2705882352941176f, 0.0745098039215686f };

Vector3f RGBintToRGBnormalizedFloat(int R, int G, int B) {
    return Vector3f((R / 255.0f), (G / 255.0f), (B / 255.0f));
}

Vector3f RGBnormalizedFloatToRGBint(float R, float G, float B) {
    return Vector3f((R * 255.0f), (G * 255.0f), (B * 255.0f));
}

// Adapted from https://en.wikipedia.org/wiki/HSL_and_HSV#To_RGB
// RGB values are required to be from 0.0 to 1.0
// Hue is returned as an angle between 0 and 360 degrees
// Saturation and Value are returned with values between 0.0 and 1.0
Vector3f RGBtoHSV(float R, float G, float B) {
    float Xmax = std::max({ R, G, B });  // Xmax = value
    float Xmin = std::min({ R, G, B });

    float chroma = Xmax - Xmin;  // range

    float hue;  // angle in degrees
    if (chroma == 0.0f) hue = 0.0f;
    else if (Xmax == R) hue = 60 * (0 + ((G - B) / chroma));
    else if (Xmax == G) hue = 60 * (2 + ((B - R) / chroma));
    else                hue = 60 * (4 + ((R - G) / chroma));  // Xmax == B
    
    if (hue < 0.0f) hue = 360.0f + hue;  // in case hue is negative: converts to a positive angle degree

    float saturation;
    if (Xmax == 0.0f) saturation = 0.0f;
    else              saturation = (chroma / Xmax);

    return Vector3f(hue, saturation, Xmax);
}

// Adapted from https://en.wikipedia.org/wiki/HSL_and_HSV#HSV_to_RGB
// Hue value is an angle degree from 0 to 360
// Saturation and Value are from 0.0 to 1.0
// Returned values are RGB from 0.0 to 1.0
Vector3f HSVtoRGB(float H, float S, float V) {
    float chroma = V * S;

    float H_line = H / 60.0f;

    float X = chroma * (1 - std::abs(std::fmod(H_line, 2.0f) - 1));

    float R_1 = 0.0f;
    float G_1 = 0.0f;
    float B_1 = 0.0f;

    if (S == 0.0f) { R_1 = 0.0f;  G_1 = 0.0f;  B_1 = 0.0f; }  // H is undefined
    else if (H_line >= 0.0f && H_line < 1.0f) { R_1 = chroma;  G_1 = X;  B_1 = 0.0f; }
    else if (H_line >= 1.0f && H_line < 2.0f) { R_1 = X;  G_1 = chroma;  B_1 = 0.0f; }
    else if (H_line >= 2.0f && H_line < 3.0f) { R_1 = 0.0f;  G_1 = chroma;  B_1 = X; }
    else if (H_line >= 3.0f && H_line < 4.0f) { R_1 = 0.0f;  G_1 = X;  B_1 = chroma; }
    else if (H_line >= 4.0f && H_line < 5.0f) { R_1 = X;  G_1 = 0.0f;  B_1 = chroma; }
    else if (H_line >= 5.0f && H_line < 6.0f) { R_1 = chroma;  G_1 = 0.0f;  B_1 = X; }

    float m = V - chroma;

    return Vector3f(R_1 + m, G_1 + m, B_1 + m);
}