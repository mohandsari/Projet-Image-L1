#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "json.h"
#include "color.h"
#include "scene3D.h"
#define prl { printf("\n"); } 


void coord3D_print(struct coord3D_s c) {
    printf("<%g,%g,%g>",c.x,c.y,c.z);
}


vector3D_t vector3D(double x, double y, double z) {
    vector3D_t vector3D;
    vector3D.x = x;
    vector3D.y = y;
    vector3D.z = z;
    return vector3D;
}


vector3D_t vector3D_add(vector3D_t u, vector3D_t v) {
    return vector3D(u.x + v.x, u.y + v.y, u.z + v.z);
}


vector3D_t vector3D_sub(vector3D_t u, vector3D_t v) {
    return vector3D(u.x - v.x, u.y - v.y, u.z - v.z);
}


vector3D_t vector3D_scale(double a, vector3D_t u) {
    return vector3D(u.x * a, u.y * a, u.z * a);
}


double vector3D_dot_product(vector3D_t u, vector3D_t v) {
    double result = u.x * v.x + u.y * v.y + u.z * v.z;
    return result;
}


vector3D_t vector3D_cross_product(vector3D_t u, vector3D_t v) {
    return vector3D(u.y * v.z - u.z * v.y, u.z * v.x - u.x * v.z, u.x * v.y - u.y * v.x);
}


double vector3D_norm2(vector3D_t u) {
    return vector3D_dot_product(u,u);
}


double vector3D_norm(vector3D_t u) {
    return sqrt(vector3D_norm2(u));
}


vector3D_t vector3D_normalize(vector3D_t u) {
    return vector3D(u.x / vector3D_norm(u), u.y / vector3D_norm(u), u.z / vector3D_norm(u));
}


point3D_t point3D(double x, double y, double z) {
    point3D_t p;
    p.x = x;
    p.y = y;
    p.z = z;
    return p;
}


point3D_t point3D_translate(point3D_t p, vector3D_t u) {
    return point3D(p.x + u.x, p.y + u.y, p.z + u.z);
}


vector3D_t point3D_subtract(point3D_t q, point3D_t p) {
    return vector3D(q.x - p.x, q.y - p.y, q.z - p.z);
}


ray_t ray(point3D_t p, vector3D_t d) {
    ray_t ray;
    ray.p = p;
    ray.d = vector3D_normalize(d);
    return ray;
}


sphere_t sphere(point3D_t c, double r) {
    sphere_t sphere;
    sphere.c = c;
    sphere.r = fabs(r);
    return sphere;
}


sphere_t sphere_translate(sphere_t s, vector3D_t v) {
   s.c = point3D_translate(s.c,v);
   return s;
}


bool sphere_intersect(sphere_t s, ray_t r, double* pl, point3D_t* pq, vector3D_t* pn) {
    double b = vector3D_dot_product(r.d ,point3D_subtract(r.p , s.c));
    double c = vector3D_norm2(point3D_subtract(r.p , s.c)) - s.r * s.r;
    double l = -b - sqrt(b * b - c);
    point3D_t q = point3D_translate(r.p ,vector3D_scale(l , r.d));
    vector3D_t n = vector3D_scale(1 / vector3D_norm(point3D_subtract(q,s.c)),point3D_subtract(q,s.c));
    if ((b * b - c >= 0) && (l >= 0)) {
        if (pl != NULL) *pl = l;
        if (pq != NULL) *pq = q;
        if (pn != NULL) *pn = n;
        return true;
    } else return false;
}


plane_t plane(point3D_t o, vector3D_t n) {
    plane_t p;
    p.o = o;
    p.n = vector3D_normalize(n);
    return p;
}


plane_t plane_translate(plane_t p, vector3D_t v) {
    p.o = point3D_translate(p.o,v);
    return p;
}


bool plane_intersect(plane_t p, ray_t r, double* pl, point3D_t* pq, vector3D_t* pn) {
    double l = vector3D_dot_product(point3D_subtract(p.o , r.p) , p.n ) / vector3D_dot_product(r.d , p.n);
    point3D_t q = point3D_translate(r.p ,vector3D_scale(l , r.d));
    vector3D_t n = p.n;
    if ((vector3D_dot_product(r.d , p.n) != 0) && (l >= 0)) {
        if (pl != NULL) *pl = l;
        if (pq != NULL) *pq = q;
        if (pn != NULL) *pn = n;
        return true;
    } else return false;
}


primitive_t p_sphere(color_t c, sphere_t sphere) {
    primitive_t p;
    p.c = c;
    p.t = SPHERE;
    p.sphr = sphere;
    return p;
}


primitive_t p_plane(color_t c, plane_t plane) {
    primitive_t p;
    p.c = c;
    p.t = PLANE;
    p.pln = plane;
    return p;
}


color_t primitive_get_color(primitive_t* p) {
    return p -> c;
}


void primitive_set_color(primitive_t* p, color_t c) {
    p -> c = c;
}


bool primitive_intersect(primitive_t* p, ray_t* r, double* pl, point3D_t* pq, vector3D_t* pn) {
    if (p -> t == SPHERE) return sphere_intersect(p -> sphr, *r, pl, pq, pn);
    else if (p -> t == PLANE) return plane_intersect(p -> pln, *r, pl, pq, pn);
    return false;
}


light_t ambient(color_t c) {
    light_t l;
    l.c = c;
    l.t = AMBIENT;
    return l;
}


light_t omni(color_t c, point3D_t p) {
    light_t l;
    l.c = c;
    l.t = OMNI;
    l.p = p;
    return l;
}


light_t uni(color_t c, vector3D_t d) {
    light_t l;
    l.c = c;
    l.t = UNI;
    l.d = d;
    return l;
}


ray_t light_ray(light_t l, point3D_t q) {
  if (l.t == OMNI) return ray(q, point3D_subtract(l.p,q));
  else if (l.t == UNI) return ray(q, vector3D_scale(-1,l.d));
  else {
      printf("the light is not  or unidirectional");prl;
      exit(EXIT_FAILURE);
  }
}


scene3D_t* scene3D_new() {
    scene3D_t* new = NULL;
    new = malloc(sizeof(scene3D_t));
    return new;
}    


void scene3D_free(scene3D_t* s) {
    free(s->tabp);
    free(s->tabl);
    free(s);
}


void scene3D_primitive_print(scene3D_t* s, unsigned int primitiveId) {
    if ((int)primitiveId >= s->tabp_length || (int)primitiveId < 0) {
        printf("wrong ID");prl;
    } else if (s->tabp[primitiveId].t == SPHERE) {
        printf("(%u) : Sphere of radius %g centered on ",primitiveId,s->tabp[primitiveId].sphr.r);
        coord3D_print(s->tabp[primitiveId].sphr.c);
        printf(" with color ");
        color_print(primitive_get_color(&s->tabp[primitiveId]));
        prl;
    } else if (s->tabp[primitiveId].t == PLANE) {
        printf("(%u) : Plane of normal ",primitiveId);
        coord3D_print(s->tabp[primitiveId].pln.n);
        printf(" passing through ");
        coord3D_print(s->tabp[primitiveId].pln.o);
        printf(" with color ");
        color_print(primitive_get_color(&s->tabp[primitiveId]));
        prl;
    }
}


void scene3D_primitives_print(scene3D_t* s) {
    if (s->tabp_length == 0) {
        printf("no primitive");prl;
    } else {
        for (int i = 0; i < s->tabp_length; i++) {
            scene3D_primitive_print(s,i);
        }
    }
}


void scene3D_light_print(scene3D_t* s, unsigned int lightId) {
    if ((int)lightId >= s->tabl_length || (int)lightId < 0) {
        printf("wrong ID");prl;
    } else if (s->tabl[lightId].t == AMBIENT) {
        printf("(%u) : Ambient light with color ",lightId);
        color_print(s->tabl[lightId].c);
        prl;
    } else if (s->tabl[lightId].t == OMNI) {
        printf("(%u) : Omnidirectional light centered on ",lightId);
        coord3D_print(s->tabl[lightId].p);
        printf(" with color ");
        color_print(s->tabl[lightId].c);
        prl;
    } else if (s->tabl[lightId].t == UNI) {
        printf("(%u) : Unidirectional light of direction ",lightId);
        coord3D_print(s->tabl[lightId].d);
        printf(" with color ");
        color_print(s->tabl[lightId].c);
        prl;
    }
}


void scene3D_lights_print(scene3D_t* s) {
    if (s->tabl_length == 0) {
        printf("no light");prl;
    } else {
        for (int i = 0; i < s->tabl_length; i++) {
            scene3D_light_print(s,i);
        }
    }
}


void scene3D_sphere_add(scene3D_t* s, color_t color, point3D_t c, double r) {
    s->tabp = realloc(s->tabp,sizeof(primitive_t)*(s->tabp_length+1));
    s->tabp[s->tabp_length] = p_sphere(color,sphere(c,r)); 
    s->tabp_length++;
}


void scene3D_plane_add(scene3D_t* s, color_t color, point3D_t o, vector3D_t n) {
    s->tabp = realloc(s->tabp,sizeof(primitive_t)*(s->tabp_length+1));
    s->tabp[s->tabp_length] = p_plane(color,plane(o,n));
    s->tabp_length++;
}


primitive_type_t scene3D_primitive_type(scene3D_t* s, unsigned int primitiveId) {
    return s->tabp[primitiveId].t;
}


void scene3D_sphere_set_radius(scene3D_t* s, unsigned int primitiveId, double radius) {
    if (scene3D_primitive_type(s,primitiveId) == SPHERE) s->tabp[primitiveId].sphr.r = radius;
    else {
        printf("the primitive is not a sphere");prl;
        exit(EXIT_FAILURE);
    }
}


void scene3D_set_point(scene3D_t* s, unsigned int primitiveId, point3D_t p) {
    if (scene3D_primitive_type(s,primitiveId) == SPHERE) s->tabp[primitiveId].sphr.c = p;
    else if (scene3D_primitive_type(s,primitiveId) == PLANE) s->tabp[primitiveId].pln.o = p;
    else {
        printf("It's not a primitive");prl;
        exit(EXIT_FAILURE);
    }
}


void scene3D_plane_set_normal(scene3D_t* s, unsigned int primitiveId, vector3D_t n) {
    if (scene3D_primitive_type(s,primitiveId) == PLANE) s->tabp[primitiveId].pln.n = vector3D_normalize(n);
    else {
        printf("the primitive is not a plane");prl;
        exit(EXIT_FAILURE);
    }
}


void scene3D_primitive_set_color(scene3D_t* s, unsigned int primitiveId, color_t color) {
    s->tabp[primitiveId].c = color;
}


void scene3D_primitive_translate(scene3D_t* s, unsigned int primitiveId, vector3D_t v) {
    if (scene3D_primitive_type(s,primitiveId) == PLANE) s->tabp[primitiveId].pln = plane_translate(s->tabp[primitiveId].pln,v);
    else if (scene3D_primitive_type(s,primitiveId) == SPHERE) s->tabp[primitiveId].sphr = sphere_translate(s->tabp[primitiveId].sphr,v);
}


void scene3D_primitive_delete(scene3D_t* s, unsigned int primitiveId) {
    for (int i = primitiveId; i < s->tabp_length; i++) {
        s->tabp[i]=s->tabp[i+1];
    }
    s->tabp = realloc(s->tabp,sizeof(primitive_t)*(s->tabp_length-1));
    s->tabp_length--;
}


void scene3D_ambient_add(scene3D_t* s, color_t color) {
    s->tabl = realloc(s->tabl,sizeof(light_t)*(s->tabl_length+1));
    s->tabl[s->tabl_length] = ambient(color);
    s->tabl_length++;
}


void scene3D_omni_add(scene3D_t* s, color_t color, point3D_t p) {
    s->tabl = realloc(s->tabl,sizeof(light_t)*(s->tabl_length+1));
    s->tabl[s->tabl_length] = omni(color,p); 
    s->tabl_length++;
    
}


void scene3D_uni_add(scene3D_t* s, color_t color, vector3D_t d) {
    s->tabl = realloc(s->tabl,sizeof(light_t)*(s->tabl_length+1));
    s->tabl[s->tabl_length] = uni(color,d);
    s->tabl_length++;
}


light_type_t scene3D_light_type(scene3D_t* s, unsigned int lightId) {
    return s->tabl[lightId].t;
}


void scene3D_omni_set_position(scene3D_t* s, unsigned int lightId, point3D_t p) {
    if (scene3D_light_type(s,lightId) == OMNI) s->tabl[lightId].p = p;
    else {
        printf("the light is not omnidirectional"); prl;
        exit(EXIT_FAILURE);
    }
}


void scene3D_uni_set_direction(scene3D_t* s, unsigned int lightId, vector3D_t d) {
    if (scene3D_light_type(s,lightId) == UNI) s->tabl[lightId].d = vector3D_normalize(d);
    else {
        printf("the light is not unidirectional");prl;
        exit(EXIT_FAILURE);
    }
}


void scene3D_light_set_color(scene3D_t* s, unsigned int lightId, color_t color) {
    s->tabl[lightId].c = color;
}


void scene3D_light_delete(scene3D_t* s, unsigned int lightId) {
    int i;
    for (i = lightId; i < s->tabl_length; i++) {
        s->tabl[i] =s->tabl[i+1];
    }
    s->tabl = realloc(s->tabl,sizeof(light_t)*(s->tabl_length-1));
    s->tabl_length--;
}


json_element_t* scene3D_to_json(scene3D_t* s) {
    json_element_t* p = json_array();
    for (int i = 0; i < s->tabp_length; i++) {
        if (scene3D_primitive_type(s,i) == PLANE) {
            json_element_t* e = json_object();
            json_object_set(e, "color", color_to_json(s->tabp[i].c));
            json_object_set(e, "type", json_string("PLANE"));
            json_object_set(e, "point", coord3D_to_json(s->tabp[i].pln.o));
            json_object_set(e, "normal", coord3D_to_json(s->tabp[i].pln.n));
            json_array_set(p, i, e);
        } else if (scene3D_primitive_type(s,i) == SPHERE) {
            json_element_t* e = json_object();
            json_object_set(e, "color", color_to_json(s->tabp[i].c));
            json_object_set(e, "type", json_string("SPHERE"));
            json_object_set(e, "radius", json_double(s->tabp[i].sphr.r));
            json_object_set(e, "center", coord3D_to_json(s->tabp[i].sphr.c));
            json_array_set(p, i, e);
        }
    }
    json_element_t* l = json_array();
    for (int i = 0; i < s->tabl_length; i++) {
        if (scene3D_light_type(s,i) == AMBIENT) {
            json_element_t* e = json_object();
            json_object_set(e, "color", color_to_json(s->tabl[i].c));
            json_object_set(e, "type", json_string("AMBIENT"));
            json_array_set(l, i, e);
        } else if (scene3D_light_type(s,i) == OMNI) {
            json_element_t* e = json_object();
            json_object_set(e, "color", color_to_json(s->tabl[i].c));
            json_object_set(e, "type", json_string("OMNI"));
            json_object_set(e, "position", coord3D_to_json(s->tabl[i].p));
            json_array_set(l, i, e);
        } else if (scene3D_light_type(s,i) == UNI) {
            json_element_t* e = json_object();
            json_object_set(e, "color", color_to_json(s->tabl[i].c));
            json_object_set(e, "type", json_string("UNI"));
            json_object_set(e, "direction", coord3D_to_json(s->tabl[i].d));
            json_array_set(l, i, e);
        }
    }
    json_element_t* json = json_object();
    json_object_set(json, "primitives", p);
    json_object_set(json, "lights", l);
    return json;
}


scene3D_t* scene3D_from_json(json_element_t* e) {
    scene3D_t* s = scene3D_new();
    json_element_t* p = json_object_get(e, "primitives");
    for ( size_t i = 0; i < json_array_size(p); i++) {
        if (!strcmp(json_as_string(json_object_get(json_array_get(p, i), "type")), "PLANE")) {
            scene3D_plane_add(s, color_from_json(json_object_get(json_array_get(p, i), "color")), coord3D_from_json(json_object_get(json_array_get(p, i), "point")), coord3D_from_json(json_object_get(json_array_get(p, i), "normal")));
        } else if (!strcmp(json_as_string(json_object_get(json_array_get(p, i), "type")), "SPHERE")) {
            scene3D_sphere_add(s, color_from_json(json_object_get(json_array_get(p, i), "color")), coord3D_from_json(json_object_get(json_array_get(p, i), "center")), json_as_double(json_object_get(json_array_get(p, i), "radius")));
        }
    }
    json_element_t* l = json_object_get(e, "lights");
    for ( size_t i = 0; i < json_array_size(l); i++) {
        if (!strcmp(json_as_string(json_object_get(json_array_get(l, i), "type")), "AMBIENT")) {
            scene3D_ambient_add(s, color_from_json(json_object_get(json_array_get(l, i), "color")));
        } else if (!strcmp(json_as_string(json_object_get(json_array_get(l, i), "type")), "OMNI")) {
            scene3D_omni_add(s, color_from_json(json_object_get(json_array_get(l, i), "color")), coord3D_from_json(json_object_get(json_array_get(l, i), "position")));
        } else if (!strcmp(json_as_string(json_object_get(json_array_get(l, i), "type")), "UNI")) {
            scene3D_uni_add(s, color_from_json(json_object_get(json_array_get(l, i), "color")), coord3D_from_json(json_object_get(json_array_get(l, i), "direction")));
        }
    }
    return s;
}


bool scene3D_intersect(scene3D_t* s, ray_t* r, double* ret_l, point3D_t* ret_q, vector3D_t* ret_n, color_t* ret_c) {
    double min = -1;
    int indice = 0;
    for (int i = 0; i < s->tabp_length; i++) { 
        if (primitive_intersect(&(s->tabp[i]), r, ret_l, ret_q, ret_n)) {
            if (*ret_l < min || min ==-1) {
                min = *ret_l;
                indice = i;
            }
        }
    }
    if (ret_c != NULL) *ret_c = s->tabp[indice].c;
    return primitive_intersect(&(s->tabp[indice]), r, ret_l, ret_q, ret_n);
}

    
ray_t scene3D_light_ray(scene3D_t* s, unsigned int lightId, point3D_t q) {
    return ray(q,s->tabl[lightId].d);
}


color_t scene3D_light_color(scene3D_t* s, unsigned int lightId) {
    return s->tabl[lightId].c;
}


point3D_t scene3D_omni_position(scene3D_t* s, unsigned int lightId) {
    if (scene3D_light_type(s,lightId) == OMNI) return s->tabl[lightId].p;
        else {
        printf("the light is not omnidirectional"); prl;
        exit(EXIT_FAILURE);
    }
}


vector3D_t scene3D_uni_direction(scene3D_t* s, unsigned int lightId) {
    if (scene3D_light_type(s,lightId) == UNI) return s->tabl[lightId].d;
    else {
        printf("the light is not unidirectional"); prl;
        exit(EXIT_FAILURE);
    }
}


color_t scene3D_enlightement(scene3D_t* s, point3D_t q, vector3D_t n, color_t intra) {
  q = point3D_translate(q, vector3D_scale(1e-13, n));
  color_t c = color(0,0,0);
  ray_t r;
  double l, ll;
  for (int i = 0; i < s->tabl_length; i++) {
    switch(scene3D_light_type(s,i)) {
    case AMBIENT:
      c = color_add(c, color_mul(scene3D_light_color(s,i), intra));
      break;
    case UNI:
      r = scene3D_light_ray(s, i, q);
      if (!scene3D_intersect(s,&r,&l,NULL,NULL,NULL))
        c = color_add(c, color_mul(color_scale(-vector3D_dot_product(scene3D_uni_direction(s,i), n), scene3D_light_color(s,i)), intra));
      break;
    case OMNI:
      r = scene3D_light_ray(s, i, q);
      ll = vector3D_norm(point3D_subtract(q, scene3D_omni_position(s,i)));
      if (!scene3D_intersect(s,&r,&l,NULL,NULL,NULL) || l > ll)
        c = color_add(c, color_mul(color_scale(vector3D_dot_product(vector3D_normalize(point3D_subtract(scene3D_omni_position(s,i), q)), n), scene3D_light_color(s,i)), intra));
      break;
    default:
      break;
    }
  }
  return c;
}


json_element_t* coord3D_to_json(struct coord3D_s c) {
    json_element_t* e = json_object();
    json_object_set(e, "x", json_double(c.x));
    json_object_set(e, "y", json_double(c.y));
    json_object_set(e, "z", json_double(c.z));
    return e;
}


struct coord3D_s coord3D_from_json(const json_element_t* e) {
    return point3D(
    json_as_double(json_object_get(e, "x")),
    json_as_double(json_object_get(e, "y")),
    json_as_double(json_object_get(e, "z")));
}
