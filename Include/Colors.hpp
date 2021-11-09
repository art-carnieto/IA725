#pragma once

// Colors list and values from https://www.rapidtables.com/web/color/RGB_Color.html

// Basic colors
extern const float color_black[] = { 0.0f, 0.0f, 0.0f };
extern const float color_white[] = { 1.0f, 1.0f, 1.0f };
extern const float color_red[] = { 1.0f, 0.0f, 0.0f };
extern const float color_lime[] = { 0.0f, 1.0f, 0.0f };
extern const float color_blue[] = { 0.0f, 0.0f, 1.0f };
extern const float color_yellow[] = { 1.0f, 1.0f, 0.0f };
extern const float color_cyan[] = { 0.0f, 1.0f, 1.0f };
extern const float color_magenta[] = { 1.0f, 0.0f, 1.0f };

// Aditional colors
extern const float color_saddle_brown[] = { 0.5450980392156863f, 0.2705882352941176f, 0.0745098039215686f };

Vector3f RGBintToRGBnormalizedFloat(int R, int G, int B) {
	return Vector3f((R / 255.0f), (G / 255.0f), (B / 255.0f));
}

Vector3f RGBnormalizedFloatToRGBint(float R, float G, float B) {
	return Vector3f((R * 255.0f), (G * 255.0f), (B * 255.0f));
}