#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "color.h"
#include "json.h"
#define prl { printf("\n"); }


double clamp_0_1(double a) {
    if (a < 0) return a = 0 ;
    else if (a > 1)  return a = 1;
    else return a;
}


color_t color(double r, double g, double b) {
    color_t c;
    c.r = r;
    c.g = g;
    c.b = b;
    c.r = clamp_0_1(c.r);
    c.g = clamp_0_1(c.g);
    c.b = clamp_0_1(c.b);
    return c;
}


json_element_t* color_to_json(color_t c) {
    json_element_t* x = json_object();
    json_element_t* r = json_double(c.r);
    json_element_t* g = json_double(c.g);
    json_element_t* b = json_double(c.b);
    json_object_set(x, "r", r);
    json_object_set(x, "g", g);
    json_object_set(x, "b", b);
    return x;
}


color_t color_from_json(json_element_t* e) {
    if (json_type(e)!=JSON_OBJECT) exit(EXIT_FAILURE);
    json_element_t* r = json_object_get(e, "r");
    json_element_t* g = json_object_get(e, "g");
    json_element_t* b = json_object_get(e, "b");
    return color(json_as_double(r), json_as_double(g), json_as_double(b));
}


color_t color_add(color_t c1, color_t c2) {
    return color(c1.r + c2.r, c1.g + c2.g, c1.b + c2.b);
}


color_t color_sub(color_t c1, color_t c2) {
    return color(c1.r - c2.r, c1.g - c2.g, c1.b - c2.b);
}


color_t color_mul(color_t c1, color_t c2) {
    return color(c1.r * c2.r, c1.g * c2.g, c1.b * c2.b);
}


color_t color_scale(double k, color_t c) {
    return color(c.r * k, c.g * k, c.b * k);
}


double   clamp_0_255(double b) {
    if (b < 0) return b = 0;
    else if (b > 255)  return b = 255;
    else return b;
}


color_t color_255(double r, double g, double b) {
    color_t c;
    c.r = r;
    c.g = g;
    c.b = b;
    c.r = clamp_0_255(c.r);
    c.g = clamp_0_255(c.g);
    c.b = clamp_0_255(c.b);
    return c;
}


void color_to_BGR(color_t c, unsigned char* buffer) {
    color_t clr = color_255(c.r * 256, c.g * 256, c.b * 256);
    buffer[0] = clr.r;
    buffer[1] = clr.g;
    buffer[2] = clr.b;
}


unsigned char color_to_graylevel(color_t c) {
    color_t clr = color_255(c.r * 256, c.g * 256, c.b * 256);
    unsigned char graylevel = (clr.r + clr.g + clr.b) / 3;
    return graylevel;
}


bool color_to_BW(color_t c) {
    double BW = (c.r + c.g + c.b) / 3;
    if (BW > 0.5) return true;
    else return false;
}


void color_print(color_t c) {
    printf("<r=%g, g=%g, b=%g>",c.r, c.g, c.b);
}
