#ifndef MENU_H
#define MENU_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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

menu_t* createMenu(const char* text);
void  addMenuAction(menu_t* m, const char* text, void(*f)());
void  deleteMenu(menu_t* m);
menu_t* projectMenu();
void  launchMenu(menu_t* m);
void  addMenuDesr(menu_t* m, const char* desr);
void  addActionDesr(action_t* a, const char* desr);
void fonction();
#endif
