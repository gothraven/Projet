#include "menu.h"


void addMenuAction(menu_t* m, const char* text, void(*f)()){
  if (m == NULL || m->size >= 9)
    exit(EXIT_FAILURE);
  m->items[m->size] = (item_t*)malloc(sizeof(item_t));
  m->items[m->size]->action = (action_t*)malloc(sizeof(action_t));
  m->items[m->size]->action->title = strdup(text);
  m->items[m->size]->tag = ACTION;
  m->items[m->size]->action->fun = f;
  m->size++;
}

menu_t* createMenu(const char* text){
  menu_t* menu = (menu_t*)malloc(sizeof(menu_t));
  menu->root = NULL;
  menu->title = strdup(text);
  menu->size = 0;
  return menu;
}

void deleteMenu(menu_t* m){
  if (m->root == NULL){
    free(m->title);
    int i = 0;
    while (i < m->size){
      if (m->items[i]->tag == MENU){
        m->items[i]->menu->root = NULL;
        deleteMenu(m->items[i]->menu);
      }else if (m->items[i]->tag == ACTION){
        free(m->items[i]->action->title);
        free(m->items[i]->action->fun);
        free(m->items[i]->action);
      }
      i++;
    }
    free(m);
  }
}

void  addMenuDesr(menu_t* m, const char* desr){
  if(m!=NULL){
    m->desr = strdup(desr);
  }
}

void  addActionDesr(action_t* a, const char* desr){
  if(a!=NULL){
    a->desr = strdup(desr);
  }
}

void  addSubMenu(menu_t* m, menu_t* sm){
  if (m == NULL || m->size >= 9 || sm == NULL)
    exit(EXIT_FAILURE);
  m->items[m->size] = (item_t*)malloc(sizeof(item_t));
  m->items[m->size]->menu = sm;
  sm->root = m;
  m->items[m->size]->tag = MENU;
  m->size++;
}

menu_t* projectMenu(){
  menu_t* menu = createMenu("Menu Principale");

  menu_t* sm01 = createMenu("Fichier");
  addMenuDesr(sm01, "manipulation de fichier, Ouvrir, sauvgarder ..etc");
  addSubMenu(menu, sm01);
  menu_t* sm02 = createMenu("Figure de base");
  addMenuDesr(sm02, "creation et suppression de figures, ligne, text, rectangle ..etc");
  addSubMenu(menu, sm02);
  menu_t* sm03 = createMenu("Transformation et groupe");
  addMenuDesr(sm03, "manipulation de figure, translater, rotation ..etc");
  addSubMenu(menu, sm03);
  addMenuAction(menu, "Quitter", fonction);
  addMenuAction(sm01, "Ouvrir", fonction);
  menu_t* sm11 = createMenu("Enregistrer");
  addSubMenu(sm01, sm11);
  addMenuAction(sm11, "SVG", fonction);
  addMenuAction(sm11, "PDF", fonction);
  addMenuAction(sm01, "Information", fonction);
  addMenuAction(sm01, "Affichage", fonction);
  addMenuAction(sm02, "Ajouter cercle", fonction);
  addMenuAction(sm02, "Ajouter rectangle", fonction);
  addMenuAction(sm02, "Ajouter ligne", fonction);
  addMenuAction(sm02, "Ajouter texte", fonction);
  addMenuAction(sm02, "Ajouter image", fonction);
  addMenuAction(sm02, "Supprimer", fonction);
  addMenuAction(sm03, "Translater", fonction);
  addMenuAction(sm03, "Tourner", fonction);
  addMenuAction(sm03, "Etirer", fonction);
  addMenuAction(sm03, "Grouper", fonction);
  addMenuAction(sm03, "Dégrouper", fonction);
  return menu;
}

void  launchMenu(menu_t* m) {
  int choix = 0;
  system("clear");
  printf("(( %s ))\n\n",m->title);
  int i = 0;
  while(i < m->size){
    if (m->items[i]->tag == ACTION){
      printf("%d- %s\n",(i + 1), m->items[i]->action->title);
      if(m->items[i]->action->desr != NULL){
        printf("\t%s\n\n",m->items[i]->action->desr);
      }
    }else if (m->items[i]->tag == MENU){
      printf("%d- %s\n",(i + 1), m->items[i]->menu->title);
      if(m->items[i]->menu->desr != NULL){
        printf("\t%s\n\n",m->items[i]->menu->desr);
      }
    }
    i++;
  }
  if (m->root != NULL){
    printf("%d- Retour\n",(m->size + 1));
  }
  printf("\nchoice? : ");
  scanf("%d",&choix);
  if (((choix <= 0 || choix > (m->root != NULL ? (m->size+1) : m->size)))){
    launchMenu(m);
  }
  system("clear");
  if (choix == (m->size + 1)) {
    if (m->root != NULL){
      launchMenu(m->root);
    }else{
      launchMenu(m);
    }
  }
  else if (m->items[choix - 1]->tag == MENU){
    launchMenu(m->items[choix - 1]->menu);
  }else if (m->items[choix - 1]->tag == ACTION) {
    m->items[choix - 1]->action->fun();
    if (m->root != NULL){
      launchMenu(m->root);
    }
    else{
      launchMenu(m);
    }
  }
}

void  fonction(){
  printf("ajouter la fonction\n");
  sleep(1);
}

/*int main(int argc, char const *argv[])
{
  menu_t* m = projectMenu();
  launchMenu(m);
  return 0;
}*/
