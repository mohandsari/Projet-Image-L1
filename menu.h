#ifndef _MENU_Mohand_ou_kaci_and_Baptiste_Projet2018_L2Info_
#define _MENU_Mohand_ou_kaci_and_Baptiste_Projet2018_L2Info_
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


typedef struct menu_s menu_t;

typedef struct {
  char* definition;
  void (*function)();
} action_t;

typedef struct {
  enum {SM, ACT} type;
  union {
    menu_t* submenu;
    action_t action;
  };
} item_t;

struct menu_s {
  menu_t* parent;
  char* definition;
  item_t tab[9];
  int nb_item;
};

menu_t* createMenu(const char* text);
void addMenuAction(menu_t* m,const char* text, void(*f)());
void addSubMenu(menu_t* m, menu_t* sm);
void deleteMenu(menu_t* m);
void launchMenu(menu_t* m);


#endif
