#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "raytracing.h"
#include "scene3D.h"
#include "color.h"
#include "menu.h"
#include "json.h"
#include "args.h"
#include "bmp.h"


unsigned int scanf_u() {
    unsigned int x;
    scanf("%u", &x);
    return x;
}


double scanf_g() {
    double x;
    scanf("%lg", &x);
    return x;
}


void fileOPen() {
    char* jsonName = "";
    printf("Enter json file name :\n");
    scanf("%s", jsonName);
    const char* jsonName2 = jsonName;
    json_element_t* e = json_load(jsonName2);
    scene3D_t* x = scene3D_from_json(e);
    scene3D_free(x);  
}


void fileSave(scene3D_t* x) {
    char* jsonName = "";
    printf("Enter json file name :\n");
    scanf("%s", jsonName);
    const char* jsonName2 = jsonName;
    json_element_t* e = scene3D_to_json(x);
    json_save(jsonName2, e);
}


void information(scene3D_t* a, const char* name) {
    printf("Informations of the scene 3D :\nSize of the scene 3D : %d\nNumber of primitive : %d\nNumber of light : %d\n", a->tabp_length + a->tabl_length, a->tabp_length, a->tabl_length);
    if (name != NULL) printf("json file name : %s\n", name);
}


void fileQuit(scene3D_t* s) {
    if (s != NULL) {
        printf("Do you want to save the scene3D? (tap 0 for NO and 1 for YES)\n");
        unsigned int x = scanf_u();
        if (x == 1) {
            fileSave(s);
            exit(EXIT_FAILURE);
        }
    } else exit(EXIT_FAILURE);    
}


color_t newColor() {
    printf("Enter the red color :\n");
    double r = scanf_g();
    printf("Enter the green color :\n");
    double g = scanf_g();
    printf("Enter the blue color :\n");    
    double b = scanf_g();
    return color(r, g, b);
}


point3D_t newPoint() {
    printf("Enter the coordinate x of the point :\n");
    double x = scanf_g();
    printf("Enter the coordinate y of the point :\n");
    double y = scanf_g();
    printf("Enter the coordinate z of the point :\n");    
    double z = scanf_g();
    return point3D(x, y, z);
}


vector3D_t newVector() {
    printf("Enter the coordinate x of the vector :\n");
    double x = scanf_g();
    printf("Enter the coordinate y of the vector :\n");
    double y = scanf_g();
    printf("Enter the coordinate z of the vector :\n");    
    double z = scanf_g();
    return vector3D(x, y, z);
}


void add_sphere(scene3D_t* s) {
    printf("Enter the radius of the sphere :\n");
    scene3D_sphere_add(s, newColor(), newPoint(), scanf_g());
}


void add_Plane(scene3D_t* s) {
    scene3D_plane_add(s, newColor(), newPoint(), newVector());
}


void modify(scene3D_t* s) {
    scene3D_primitives_print(s);
    printf("Enter a primitiveId\n");
    unsigned int x = scanf_u();
    if (scene3D_primitive_type(s, x) == SPHERE) {
        scene3D_set_point(s, x, newPoint());
        printf("Enter the radius of the sphere :\n");
        scene3D_sphere_set_radius(s, x, scanf_g());
        scene3D_primitive_set_color(s, x, newColor());
    } else if (scene3D_primitive_type(s, x) == PLANE) {
        scene3D_set_point(s, x, newPoint());
        scene3D_plane_set_normal(s, x, newVector());
        scene3D_primitive_set_color(s, x, newColor());
    } else {
        printf("It's not a primitive\n");prl;
        exit(EXIT_FAILURE);
    }
}

    
void translate(scene3D_t* s) {
    scene3D_primitives_print(s);
    printf("Enter a primitiveId\n");
    scene3D_primitive_translate(s, scanf_u(), newVector());
}
    
    
void delete(scene3D_t* s) {
    scene3D_primitives_print(s);
    printf("Enter a primitiveId\n");
    scene3D_primitive_delete(s, scanf_u());
}


void Ambient_Light(scene3D_t* s) {
    scene3D_ambient_add(s, newColor());
}


void add_Unidirectional_Light(scene3D_t* s) {
    scene3D_uni_add(s, newColor(), newVector());
}


void add_Omnidirectional_Light(scene3D_t* s) {
    scene3D_omni_add(s, newColor(), newPoint());
}


void light_modify(scene3D_t* s) {
    scene3D_lights_print(s);
    printf("Enter a lightId\n");
    unsigned int x = scanf_u();
    if (scene3D_light_type(s, x) == AMBIENT) scene3D_light_set_color(s, x, newColor());
    else if (scene3D_light_type(s, x) == OMNI) {
        scene3D_light_set_color(s, x, newColor());
        scene3D_omni_set_position(s, x, newPoint());
    } else if (scene3D_light_type(s, x) == UNI) {
        scene3D_light_set_color(s, x, newColor());
        scene3D_uni_set_direction(s, x, newVector());
    } else {
        printf("It's not a light");prl;
        exit(EXIT_FAILURE);
    }
}


void light_delete(scene3D_t* s) {
    scene3D_primitives_print(s);
    printf("Enter a lightId");
    scene3D_light_delete(s, scanf_u());
}


void export() {
  printf("Functionality export is called\n");
}


camera_t cam;

scene3D_t scene;


void position() {
    printf("New positon off the camera :\nDo you want default values? (0 for NO and anything else for YES)\n");
    unsigned int def = scanf_u();
    if (def == 0) cam.pos = newPoint();
    else cam.pos = point3D(0,0,0);
}
  
  
void direction() {
    printf("New direction off the camera :\nDo you want default values? (0 for NO and anything else for YES)\n");
    unsigned int def = scanf_u();
    if (def == 0) cam.dir = newVector();
    else cam.dir = vector3D(0,0,-1);
}


void mire() {
    printf("New positon to watch:\nDo you want default values? (0 for NO and anything else for YES)\n");
    unsigned int def = scanf_u();
    if (def == 0) {
        cam.vert = newVector();
        cam.hori = newVector();
    }
}


void focal() {
    printf("Distance from the screen to the camera :\nDo you want default values? (0 for NO and anything else for YES)\n");
    unsigned int def = scanf_u();
    if (def == 0) cam.focal = scanf_g();
    else cam.focal = 1;
}
 
 
void dimension() {
    printf("Dimension of the image :\nDo you want default values? (0 for NO and anything else for YES)\n");
    unsigned int def = scanf_u();
    if (def == 0) {
        printf("Enter the width :");
        cam.width = scanf_g();
        printf("Enter the height :");
        cam.height = scanf_g();
    } else {
        cam.width = 800;
        cam.height = 600;
    }
}


size_t depth;


void depth_f() {
    printf("Number of bits per pixel : (1, 8 or 24)\nDo you want default values? (0 for NO and anything else for YES)\n");
    unsigned int def = scanf_u();
    if (def == 0) depth = scanf_u();
    else depth = 24;
}


bool cont = true;


void quit() {
  cont = false;
}


int main() {
    menu_t* m;
    menu_t* sm;
    menu_t* smm;
    
    m = createMenu("Main menu");
    
    
    sm = createMenu("File");
    addMenuAction(sm, "Open", fileOPen);
    addMenuAction(sm, "Save", fileSave);
    addMenuAction(sm, "Information", information);
    addMenuAction(sm, "Quitter", fileQuit);
    addSubMenu(m, sm);
    
    
    sm = createMenu("3D scene");
    
    smm = createMenu("Primitives");
    addMenuAction(smm, "Add Sphere", add_sphere);
    addMenuAction(smm, "Add Plane", add_Plane);
    addMenuAction(smm, "Modify", modify);
    addMenuAction(smm, "Translate", translate);        
    addMenuAction(smm, "Delete", delete);
    addSubMenu(sm, smm);
    
    smm = createMenu("Lights");
    addMenuAction(smm, "Ambiant Light", Ambient_Light);
    addMenuAction(smm, "Add Unidirectional Light", add_Unidirectional_Light);
    addMenuAction(smm, "Add Omnidirectional Light", add_Omnidirectional_Light);
    addMenuAction(smm, "Modify", light_modify);
    addMenuAction(smm, "Delete", light_delete);
    addSubMenu(sm, smm);
    addSubMenu(m, sm);


    sm = createMenu("Generation");
    
    addMenuAction(sm, "Export", export);
    
    smm = createMenu("Camera");
    addMenuAction(smm, "Position", position);
    addMenuAction(smm, "Direction", direction);
    addMenuAction(smm, "Mire", mire);
    addSubMenu(sm, smm);
    
    smm = createMenu("Image");
    addMenuAction(smm, "Focal", focal);
    addMenuAction(smm, "Dimension", dimension);
    addMenuAction(smm, "Depth", depth_f);
    
    addSubMenu(sm, smm);
    addSubMenu(m, sm);

    addMenuAction(m, "Quit", quit);

    while (cont) launchMenu(m);

    deleteMenu(m);   

    return EXIT_SUCCESS;
}
