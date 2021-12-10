#include "Colors.hpp"
#include <algorithm>

// Colors list and values adapted from https://www.rapidtables.com/web/color/RGB_Color.html
const float color_maroon[] =  { 0.501960784313725f, 0.0f, 0.0f };
const float color_dark_red[] =  { 0.545098039215686f, 0.0f, 0.0f };
const float color_brown[] =  { 0.647058823529412f, 0.164705882352941f, 0.164705882352941f };
const float color_firebrick[] =  { 0.698039215686274f, 0.133333333333333f, 0.133333333333333f };
const float color_crimson[] =  { 0.862745098039216f, 0.0784313725490196f, 0.235294117647059f };
const float color_red[] =  { 1.0f, 0.0f, 0.0f };
const float color_tomato[] =  { 1.0f, 0.388235294117647f, 0.27843137254902f };
const float color_coral[] =  { 1.0f, 0.498039215686275f, 0.313725490196078f };
const float color_indian_red[] =  { 0.803921568627451f, 0.36078431372549f, 0.36078431372549f };
const float color_light_coral[] =  { 0.941176470588235f, 0.501960784313725f, 0.501960784313725f };
const float color_dark_salmon[] =  { 0.913725490196078f, 0.588235294117647f, 0.47843137254902f };
const float color_salmon[] =  { 0.980392156862745f, 0.501960784313725f, 0.447058823529412f };
const float color_light_salmon[] =  { 1.0f, 0.627450980392157f, 0.47843137254902f };
const float color_orange_red[] =  { 1.0f, 0.270588235294118f, 0.0f };
const float color_dark_orange[] =  { 1.0f, 0.549019607843137f, 0.0f };
const float color_orange[] =  { 1.0f, 0.647058823529412f, 0.0f };
const float color_gold[] =  { 1.0f, 0.843137254901961f, 0.0f };
const float color_dark_golden_rod[] =  { 0.72156862745098f, 0.525490196078431f, 0.0431372549019608f };
const float color_golden_rod[] =  { 0.854901960784314f, 0.647058823529412f, 0.125490196078431f };
const float color_pale_golden_rod[] =  { 0.933333333333333f, 0.909803921568627f, 0.666666666666667f };
const float color_dark_khaki[] =  { 0.741176470588235f, 0.717647058823529f, 0.419607843137255f };
const float color_khaki[] =  { 0.941176470588235f, 0.901960784313726f, 0.549019607843137f };
const float color_olive[] =  { 0.501960784313725f, 0.501960784313725f, 0.0f };
const float color_yellow[] =  { 1.0f, 1.0f, 0.0f };
const float color_yellow_green[] =  { 0.603921568627451f, 0.803921568627451f, 0.196078431372549f };
const float color_dark_olive_green[] =  { 0.333333333333333f, 0.419607843137255f, 0.184313725490196f };
const float color_olive_drab[] =  { 0.419607843137255f, 0.556862745098039f, 0.137254901960784f };
const float color_lawn_green[] =  { 0.486274509803922f, 0.988235294117647f, 0.0f };
const float color_chart_reuse[] =  { 0.498039215686275f, 1.0f, 0.0f };
const float color_green_yellow[] =  { 0.67843137254902f, 1.0f, 0.184313725490196f };
const float color_dark_green[] =  { 0.0f, 0.392156862745098f, 0.0f };
const float color_green[] =  { 0.0f, 0.501960784313725f, 0.0f };
const float color_forest_green[] =  { 0.133333333333333f, 0.545098039215686f, 0.133333333333333f };
const float color_lime[] =  { 0.0f, 1.0f, 0.0f };
const float color_lime_green[] =  { 0.196078431372549f, 0.803921568627451f, 0.196078431372549f };
const float color_light_green[] =  { 0.564705882352941f, 0.933333333333333f, 0.564705882352941f };
const float color_pale_green[] =  { 0.596078431372549f, 0.984313725490196f, 0.596078431372549f };
const float color_dark_sea_green[] =  { 0.56078431372549f, 0.737254901960784f, 0.56078431372549f };
const float color_medium_spring_green[] =  { 0.0f, 0.980392156862745f, 0.603921568627451f };
const float color_spring_green[] =  { 0.0f, 1.0f, 0.498039215686275f };
const float color_sea_green[] =  { 0.180392156862745f, 0.545098039215686f, 0.341176470588235f };
const float color_medium_aqua_marine[] =  { 0.4f, 0.803921568627451f, 0.666666666666667f };
const float color_medium_sea_green[] =  { 0.235294117647059f, 0.701960784313725f, 0.443137254901961f };
const float color_light_sea_green[] =  { 0.125490196078431f, 0.698039215686274f, 0.666666666666667f };
const float color_dark_slate_gray[] =  { 0.184313725490196f, 0.309803921568627f, 0.309803921568627f };
const float color_teal[] =  { 0.0f, 0.501960784313725f, 0.501960784313725f };
const float color_dark_cyan[] =  { 0.0f, 0.545098039215686f, 0.545098039215686f };
const float color_aqua[] =  { 0.0f, 1.0f, 1.0f };
const float color_cyan[] =  { 0.0f, 1.0f, 1.0f };
const float color_light_cyan[] =  { 0.87843137254902f, 1.0f, 1.0f };
const float color_dark_turquoise[] =  { 0.0f, 0.807843137254902f, 0.819607843137255f };
const float color_turquoise[] =  { 0.250980392156863f, 0.87843137254902f, 0.815686274509804f };
const float color_medium_turquoise[] =  { 0.282352941176471f, 0.819607843137255f, 0.8f };
const float color_pale_turquoise[] =  { 0.686274509803922f, 0.933333333333333f, 0.933333333333333f };
const float color_aqua_marine[] =  { 0.498039215686275f, 1.0f, 0.831372549019608f };
const float color_powder_blue[] =  { 0.690196078431373f, 0.87843137254902f, 0.901960784313726f };
const float color_cadet_blue[] =  { 0.372549019607843f, 0.619607843137255f, 0.627450980392157f };
const float color_steel_blue[] =  { 0.274509803921569f, 0.509803921568627f, 0.705882352941177f };
const float color_corn_flower_blue[] =  { 0.392156862745098f, 0.584313725490196f, 0.929411764705882f };
const float color_deep_sky_blue[] =  { 0.0f, 0.749019607843137f, 1.0f };
const float color_dodger_blue[] =  { 0.117647058823529f, 0.564705882352941f, 1.0f };
const float color_light_blue[] =  { 0.67843137254902f, 0.847058823529412f, 0.901960784313726f };
const float color_sky_blue[] =  { 0.529411764705882f, 0.807843137254902f, 0.92156862745098f };
const float color_light_sky_blue[] =  { 0.529411764705882f, 0.807843137254902f, 0.980392156862745f };
const float color_midnight_blue[] =  { 0.0980392156862745f, 0.0980392156862745f, 0.43921568627451f };
const float color_navy[] =  { 0.0f, 0.0f, 0.501960784313725f };
const float color_dark_blue[] =  { 0.0f, 0.0f, 0.545098039215686f };
const float color_medium_blue[] =  { 0.0f, 0.0f, 0.803921568627451f };
const float color_blue[] =  { 0.0f, 0.0f, 1.0f };
const float color_royal_blue[] =  { 0.254901960784314f, 0.411764705882353f, 0.882352941176471f };
const float color_blue_violet[] =  { 0.541176470588235f, 0.168627450980392f, 0.886274509803922f };
const float color_indigo[] =  { 0.294117647058824f, 0.0f, 0.509803921568627f };
const float color_dark_slate_blue[] =  { 0.282352941176471f, 0.23921568627451f, 0.545098039215686f };
const float color_slate_blue[] =  { 0.415686274509804f, 0.352941176470588f, 0.803921568627451f };
const float color_medium_slate_blue[] =  { 0.482352941176471f, 0.407843137254902f, 0.933333333333333f };
const float color_medium_purple[] =  { 0.576470588235294f, 0.43921568627451f, 0.858823529411765f };
const float color_dark_magenta[] =  { 0.545098039215686f, 0.0f, 0.545098039215686f };
const float color_dark_violet[] =  { 0.580392156862745f, 0.0f, 0.827450980392157f };
const float color_dark_orchid[] =  { 0.6f, 0.196078431372549f, 0.8f };
const float color_medium_orchid[] =  { 0.729411764705882f, 0.333333333333333f, 0.827450980392157f };
const float color_purple[] =  { 0.501960784313725f, 0.0f, 0.501960784313725f };
const float color_thistle[] =  { 0.847058823529412f, 0.749019607843137f, 0.847058823529412f };
const float color_plum[] =  { 0.866666666666667f, 0.627450980392157f, 0.866666666666667f };
const float color_violet[] =  { 0.933333333333333f, 0.509803921568627f, 0.933333333333333f };
const float color_magenta[] =  { 1.0f, 0.0f, 1.0f };
const float color_orchid[] =  { 0.854901960784314f, 0.43921568627451f, 0.83921568627451f };
const float color_medium_violet_red[] =  { 0.780392156862745f, 0.0823529411764706f, 0.52156862745098f };
const float color_pale_violet_red[] =  { 0.858823529411765f, 0.43921568627451f, 0.576470588235294f };
const float color_deep_pink[] =  { 1.0f, 0.0784313725490196f, 0.576470588235294f };
const float color_hot_pink[] =  { 1.0f, 0.411764705882353f, 0.705882352941177f };
const float color_light_pink[] =  { 1.0f, 0.713725490196078f, 0.756862745098039f };
const float color_pink[] =  { 1.0f, 0.752941176470588f, 0.796078431372549f };
const float color_antique_white[] =  { 0.980392156862745f, 0.92156862745098f, 0.843137254901961f };
const float color_beige[] =  { 0.96078431372549f, 0.96078431372549f, 0.862745098039216f };
const float color_bisque[] =  { 1.0f, 0.894117647058824f, 0.768627450980392f };
const float color_blanched_almond[] =  { 1.0f, 0.92156862745098f, 0.803921568627451f };
const float color_wheat[] =  { 0.96078431372549f, 0.870588235294118f, 0.701960784313725f };
const float color_corn_silk[] =  { 1.0f, 0.972549019607843f, 0.862745098039216f };
const float color_lemon_chiffon[] =  { 1.0f, 0.980392156862745f, 0.803921568627451f };
const float color_light_golden_rod_yellow[] =  { 0.980392156862745f, 0.980392156862745f, 0.823529411764706f };
const float color_light_yellow[] =  { 1.0f, 1.0f, 0.87843137254902f };
const float color_saddle_brown[] =  { 0.545098039215686f, 0.270588235294118f, 0.0745098039215686f };
const float color_sienna[] =  { 0.627450980392157f, 0.32156862745098f, 0.176470588235294f };
const float color_chocolate[] =  { 0.823529411764706f, 0.411764705882353f, 0.117647058823529f };
const float color_peru[] =  { 0.803921568627451f, 0.52156862745098f, 0.247058823529412f };
const float color_sandy_brown[] =  { 0.956862745098039f, 0.643137254901961f, 0.376470588235294f };
const float color_burly_wood[] =  { 0.870588235294118f, 0.72156862745098f, 0.529411764705882f };
const float color_tan[] =  { 0.823529411764706f, 0.705882352941177f, 0.549019607843137f };
const float color_rosy_brown[] =  { 0.737254901960784f, 0.56078431372549f, 0.56078431372549f };
const float color_moccasin[] =  { 1.0f, 0.894117647058824f, 0.709803921568627f };
const float color_navajo_white[] =  { 1.0f, 0.870588235294118f, 0.67843137254902f };
const float color_peach_puff[] =  { 1.0f, 0.854901960784314f, 0.725490196078431f };
const float color_misty_rose[] =  { 1.0f, 0.894117647058824f, 0.882352941176471f };
const float color_lavender_blush[] =  { 1.0f, 0.941176470588235f, 0.96078431372549f };
const float color_linen[] =  { 0.980392156862745f, 0.941176470588235f, 0.901960784313726f };
const float color_old_lace[] =  { 0.992156862745098f, 0.96078431372549f, 0.901960784313726f };
const float color_papaya_whip[] =  { 1.0f, 0.937254901960784f, 0.835294117647059f };
const float color_sea_shell[] =  { 1.0f, 0.96078431372549f, 0.933333333333333f };
const float color_mint_cream[] =  { 0.96078431372549f, 1.0f, 0.980392156862745f };
const float color_slate_gray[] =  { 0.43921568627451f, 0.501960784313725f, 0.564705882352941f };
const float color_light_slate_gray[] =  { 0.466666666666667f, 0.533333333333333f, 0.6f };
const float color_light_steel_blue[] =  { 0.690196078431373f, 0.768627450980392f, 0.870588235294118f };
const float color_lavender[] =  { 0.901960784313726f, 0.901960784313726f, 0.980392156862745f };
const float color_floral_white[] =  { 1.0f, 0.980392156862745f, 0.941176470588235f };
const float color_alice_blue[] =  { 0.941176470588235f, 0.972549019607843f, 1.0f };
const float color_ghost_white[] =  { 0.972549019607843f, 0.972549019607843f, 1.0f };
const float color_honeydew[] =  { 0.941176470588235f, 1.0f, 0.941176470588235f };
const float color_ivory[] =  { 1.0f, 1.0f, 0.941176470588235f };
const float color_azure[] =  { 0.941176470588235f, 1.0f, 1.0f };
const float color_snow[] =  { 1.0f, 0.980392156862745f, 0.980392156862745f };
const float color_black[] =  { 0.0f, 0.0f, 0.0f };
const float color_dim_gray[] =  { 0.411764705882353f, 0.411764705882353f, 0.411764705882353f };
const float color_gray[] =  { 0.501960784313725f, 0.501960784313725f, 0.501960784313725f };
const float color_dark_gray[] =  { 0.662745098039216f, 0.662745098039216f, 0.662745098039216f };
const float color_silver[] =  { 0.752941176470588f, 0.752941176470588f, 0.752941176470588f };
const float color_light_gray[] =  { 0.827450980392157f, 0.827450980392157f, 0.827450980392157f };
const float color_gainsboro[] =  { 0.862745098039216f, 0.862745098039216f, 0.862745098039216f };
const float color_white_smoke[] =  { 0.96078431372549f, 0.96078431372549f, 0.96078431372549f };
const float color_white[] =  { 1.0f, 1.0f, 1.0f };

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