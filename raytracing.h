#ifndef _RAYTRACING_Mohand_ou_kaci_and_Baptiste_Projet2018_L2Info_
#define _RAYTRACING_Mohand_ou_kaci_and_Baptiste_Projet2018_L2Info_
#include "scene3D.h"
#include "json.h"
 

typedef struct {
    double focal;
    point3D_t pos;
    vector3D_t dir, vert, hori;    
    size_t width, height;
} camera_t;
    
camera_t camera(point3D_t p, vector3D_t dir, vector3D_t vert, double f, size_t w, size_t h);
json_element_t* camera_to_json(camera_t c);
camera_t camera_from_json(const json_element_t* e);
    
void camera_raytracing(const char* fname, size_t depth, camera_t c, scene3D_t* s);


#endif
