#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "raytracing.h"
#include "scene3D.h"
#include "json.h"
#include "menu.h"


menu_t* createMenu(const char* text) {
  menu_t* new = NULL;
  new = malloc(sizeof(menu_t));
  (*new).parent = NULL;
  (*new).definition = malloc(strlen(text)+1);
  strcpy((*new).definition,text);
  (*new).nb_item = 0;
  return new;
}


void addMenuAction(menu_t* m, const char* text, void(*f)()) {
  if ((*m).nb_item >= 9) printf("ARRAY ARE COMPLETED");
  else {
    action_t action1;
    action1.function = f;
    action1.definition = malloc(strlen(text)+1);
    strcpy(action1.definition, text);
    item_t item1;
    item1.action = action1;
    item1.type = ACT;
    (*m).tab[(*m).nb_item] = item1;
    (*m).nb_item++;  
   }
}


void addSubMenu(menu_t* m, menu_t* sm) {
  if ((*sm).parent == NULL && (*m).nb_item < 9) {
    (*sm).parent = m; 
    (*m).tab[(*m).nb_item].type = SM;
    (*m).tab[(*m).nb_item].submenu = malloc(sizeof(menu_t));
    (*m).tab[(*m).nb_item].submenu = sm;
    (*m).nb_item++;
  }
}


void deleteMenu(menu_t* m) {
  if ((*m).parent == NULL) {
    for(int i = 0; i < (*m).nb_item ; i++) {
      if ((*m).tab[i].type == ACT ) free((*m).tab[i].action.definition);
      else if ((*m).tab[i].type == SM) deleteMenu((*m).tab[i].submenu);
    }
    free((*m).definition);
    free(m);	      
    }
}  
  

void launchMenu(menu_t* m) {
  //display the details of the menu
  printf("%s\n", (*m).definition);
  //display the different choice/items of the menu preceded by a number
  for(int i = 0; i < (*m).nb_item; i++) {
      if ((*m).tab[i].type == ACT ) printf("%d - %s\n", i+1, (*m).tab[i].action.definition);
      else printf("%d - %s\n", i+1, (*(*m).tab[i].submenu).definition);    
  }
  //wait for the user presss a key
  char choice;
  printf(" ?\n");
  do scanf("%c", &choice);
  while (choice == '\n');
  //if the numeral is between 1 and 9, depending on the type of the corresponding item:
  if (choice == 'p') {
      if ((*m).parent != NULL) launchMenu((*m).parent);
      else launchMenu(m);
  } else if (choice <= '0' + (*m).nb_item && choice >= '1') {
      if ((*m).tab[choice-'1'].type == ACT) (*m).tab[choice-'1'].action.function();
      else if ((*m).tab[choice-'1'].type == SM) launchMenu((*m).tab[choice-'1'].submenu);
  } else if (choice < 0 || choice > 9) {
    printf("Error try again\n");
    launchMenu(m);
  } else {
    printf("Error try again\n");
    launchMenu(m);
  }
}
