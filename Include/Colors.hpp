#pragma once
#include "ogldev_math_3d.h"

// Colors list and values adapted from https://www.rapidtables.com/web/color/RGB_Color.html
extern const float color_maroon[];
extern const float color_dark_red[];
extern const float color_brown[];
extern const float color_firebrick[];
extern const float color_crimson[];
extern const float color_red[];
extern const float color_tomato[];
extern const float color_coral[];
extern const float color_indian_red[];
extern const float color_light_coral[];
extern const float color_dark_salmon[];
extern const float color_salmon[];
extern const float color_light_salmon[];
extern const float color_orange_red[];
extern const float color_dark_orange[];
extern const float color_orange[];
extern const float color_gold[];
extern const float color_dark_golden_rod[];
extern const float color_golden_rod[];
extern const float color_pale_golden_rod[];
extern const float color_dark_khaki[];
extern const float color_khaki[];
extern const float color_olive[];
extern const float color_yellow[];
extern const float color_yellow_green[];
extern const float color_dark_olive_green[];
extern const float color_olive_drab[];
extern const float color_lawn_green[];
extern const float color_chart_reuse[];
extern const float color_green_yellow[];
extern const float color_dark_green[];
extern const float color_green[];
extern const float color_forest_green[];
extern const float color_lime[];
extern const float color_lime_green[];
extern const float color_light_green[];
extern const float color_pale_green[];
extern const float color_dark_sea_green[];
extern const float color_medium_spring_green[];
extern const float color_spring_green[];
extern const float color_sea_green[];
extern const float color_medium_aqua_marine[];
extern const float color_medium_sea_green[];
extern const float color_light_sea_green[];
extern const float color_dark_slate_gray[];
extern const float color_teal[];
extern const float color_dark_cyan[];
extern const float color_aqua[];
extern const float color_cyan[];
extern const float color_light_cyan[];
extern const float color_dark_turquoise[];
extern const float color_turquoise[];
extern const float color_medium_turquoise[];
extern const float color_pale_turquoise[];
extern const float color_aqua_marine[];
extern const float color_powder_blue[];
extern const float color_cadet_blue[];
extern const float color_steel_blue[];
extern const float color_corn_flower_blue[];
extern const float color_deep_sky_blue[];
extern const float color_dodger_blue[];
extern const float color_light_blue[];
extern const float color_sky_blue[];
extern const float color_light_sky_blue[];
extern const float color_midnight_blue[];
extern const float color_navy[];
extern const float color_dark_blue[];
extern const float color_medium_blue[];
extern const float color_blue[];
extern const float color_royal_blue[];
extern const float color_blue_violet[];
extern const float color_indigo[];
extern const float color_dark_slate_blue[];
extern const float color_slate_blue[];
extern const float color_medium_slate_blue[];
extern const float color_medium_purple[];
extern const float color_dark_magenta[];
extern const float color_dark_violet[];
extern const float color_dark_orchid[];
extern const float color_medium_orchid[];
extern const float color_purple[];
extern const float color_thistle[];
extern const float color_plum[];
extern const float color_violet[];
extern const float color_magenta[];
extern const float color_orchid[];
extern const float color_medium_violet_red[];
extern const float color_pale_violet_red[];
extern const float color_deep_pink[];
extern const float color_hot_pink[];
extern const float color_light_pink[];
extern const float color_pink[];
extern const float color_antique_white[];
extern const float color_beige[];
extern const float color_bisque[];
extern const float color_blanched_almond[];
extern const float color_wheat[];
extern const float color_corn_silk[];
extern const float color_lemon_chiffon[];
extern const float color_light_golden_rod_yellow[];
extern const float color_light_yellow[];
extern const float color_saddle_brown[];
extern const float color_sienna[];
extern const float color_chocolate[];
extern const float color_peru[];
extern const float color_sandy_brown[];
extern const float color_burly_wood[];
extern const float color_tan[];
extern const float color_rosy_brown[];
extern const float color_moccasin[];
extern const float color_navajo_white[];
extern const float color_peach_puff[];
extern const float color_misty_rose[];
extern const float color_lavender_blush[];
extern const float color_linen[];
extern const float color_old_lace[];
extern const float color_papaya_whip[];
extern const float color_sea_shell[];
extern const float color_mint_cream[];
extern const float color_slate_gray[];
extern const float color_light_slate_gray[];
extern const float color_light_steel_blue[];
extern const float color_lavender[];
extern const float color_floral_white[];
extern const float color_alice_blue[];
extern const float color_ghost_white[];
extern const float color_honeydew[];
extern const float color_ivory[];
extern const float color_azure[];
extern const float color_snow[];
extern const float color_black[];
extern const float color_dim_gray[];
extern const float color_gray[];
extern const float color_dark_gray[];
extern const float color_silver[];
extern const float color_light_gray[];
extern const float color_gainsboro[];
extern const float color_white_smoke[];
extern const float color_white[];

// functions
Vector3f RGBintToRGBnormalizedFloat(int R, int G, int B);
Vector3f RGBnormalizedFloatToRGBint(float R, float G, float B);
Vector3f RGBtoHSV(float R, float G, float B);
Vector3f HSVtoRGB(float H, float S, float V);