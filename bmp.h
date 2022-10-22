#ifndef _BMP_Mohand_ou_kaci_and_Baptiste_Projet2018_L2Info_
#define _BMP_Mohand_ou_kaci_and_Baptiste_Projet2018_L2Info_
#include <stdlib.h>
#include <stdio.h>
#include "color.h"


void bmp_save(const char* fname, size_t width, size_t height, size_t depth, color_t (*pixel)(size_t i, size_t j, void** args), void** args);


#endif
