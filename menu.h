#ifndef MENU_H
#define MENU_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include "svg.h"
#include "xml.h"

typedef struct action {
  char* title;
  char* desr;
  void  (*fun)();
} action_t;

typedef struct item {
  union {
    struct menu_s*  menu;
    struct action*  action;
  };
  enum tag {
    ACTION = 1,
    MENU
  } tag;
} item_t;

typedef struct menu_s {
  struct menu_s* root;
  char* title;
  char* desr;
  struct item* items[9];
  int size;
} menu_t;

extern svg_t svg;
extern bool file_exist;

menu_t* createMenu(const char* text);
void addMenuAction(menu_t* m, const char* text, void(*f)());
void deleteMenu(menu_t* m);
menu_t* projectMenu();
void launchMenu(menu_t* m);
void addMenuDesr(menu_t* m, const char* desr);
void addActionDesr(action_t* a, const char* desr);
void fin();
void ouvrir();
void sauvgarder_svg();
void sauvgarder_pdf();
void information();
void affichage();
void ajouter_circle();
void ajouter_rect();
void ajouter_line();
void ajouter_text();
void ajouter_image();
void supprimer();
void translater();
void tourner();
void etirer();

void fonction();

#endif
