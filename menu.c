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
  addMenuAction(menu, "Quitter", fin);
  addMenuAction(sm01, "Ouvrir", ouvrir);
  menu_t* sm11 = createMenu("Enregistrer");
  addSubMenu(sm01, sm11);
  addMenuAction(sm11, "SVG", sauvgarder_svg);
  addMenuAction(sm11, "PDF", sauvgarder_pdf);
  addMenuAction(sm01, "Information", information);
  addMenuAction(sm01, "Affichage", affichage);
  addMenuAction(sm02, "Ajouter cercle", ajouter_circle);
  addMenuAction(sm02, "Ajouter rectangle", ajouter_rect);
  addMenuAction(sm02, "Ajouter ligne", ajouter_line);
  addMenuAction(sm02, "Ajouter texte", ajouter_text);
  addMenuAction(sm02, "Ajouter image", ajouter_image);
  addMenuAction(sm02, "Supprimer", supprimer);
  addMenuAction(sm03, "Translater", translater);
  addMenuAction(sm03, "Tourner", tourner);
  addMenuAction(sm03, "Etirer", etirer);
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

void fin(){
  exit(EXIT_SUCCESS);
}

void ouvrir(){
  if(!file_exist){
    char fname[100];
    printf("nom de ficher: ");
    scanf("%s",fname);
    if(strstr(fname, ".svg")!=NULL){
      xelement_t* xml = load_xml(fname);
      if(xml==NULL)
        exit(EXIT_FAILURE);
      svg = xml_to_svg(xml);
      file_exist = true;
    }
  }else{
    printf("il exist deja un ficher svg\n");
  }
  sleep(1);
}

void sauvgarder_svg(){
  if(file_exist){
    char fname[100];
    printf("nom de ficher: ");
    scanf("%s",fname);
    if(strstr(fname, ".svg")!=NULL){
      save_xml(fname, svg.e);
      file_exist = false;
    }
  }else{
    printf("il n'exist pas de fichier ouvert\n");
  }
  sleep(1);
}

void sauvgarder_pdf(){
  if(file_exist){
    char fname[100];
    printf("nom de ficher: ");
    scanf("%s",fname);
    if(strstr(fname, ".pdf")!=NULL){
      pdf_t* pdf = svg_to_pdf(svg);
      pdf_save(fname, pdf);
      file_exist = false;
    }
  }else{
    printf("il n'exist pas de fichier ouvert\n");
  }
  sleep(1);
}

void information(){
  if(file_exist){
    printf("SVG info\n");
    image_info(svg.im);
  }else{
    printf("il n'exist pas de fichier ouvert\n");
  }
  sleep(3);
}

void affichage(){
  if(file_exist){
    paint(svg.im);
  }else{
    printf("il n'exist pas de fichier ouvert\n");
  }
  sleep(1);
}

void ajouter_circle(){
  if(file_exist){
    double x = 0;
    printf("x: ");
    scanf("%lf", &x);
    double y = 0;
    printf("y: ");
    scanf("%lf", &y);
    double r = 0;
    printf("rayon: ");
    scanf("%lf", &r);
    figure_t f = circle(r);
    f.centre.x = x;
    f.centre.y = y;
    svg.im = append(svg.im, f);
  }else{
    printf("il n'exist pas de fichier ouvert\n");
  }
  sleep(1);
}

void ajouter_rect(){
  if(file_exist){
    double x = 0;
    printf("x: ");
    scanf("%lf", &x);
    double y = 0;
    printf("y: ");
    scanf("%lf", &y);
    double w = 0;
    printf("longeur: ");
    scanf("%lf", &w);
    double h = 0;
    printf("hauteur: ");
    scanf("%lf", &h);
    figure_t f = rectangle(w, h);
    f.centre.x = x;
    f.centre.y = y;
    svg.im = append(svg.im, f);
  }else{
    printf("il n'exist pas de fichier ouvert\n");
  }
  sleep(1);
}

void ajouter_line(){
  if(file_exist){
    double x1 = 0;
    printf("x1: ");
    scanf("%lf", &x1);
    double y1 = 0;
    printf("y1: ");
    scanf("%lf", &y1);
    double x2 = 0;
    printf("x2: ");
    scanf("%lf", &x2);
    double y2 = 0;
    printf("y2: ");
    scanf("%lf", &y2);
    double g = 0;
    printf("grain: ");
    scanf("%lf", &g);
    double length = sqrt(((x2 - x1)*(x2 - x1)) + ((y2 - y1)*(y2 - y1)));
    figure_t f = line(length);
    svg.im = append(svg.im, f);
  }else{
    printf("il n'exist pas de fichier ouvert\n");
  }
  sleep(1);
}

void ajouter_text(){
  if(file_exist){
    char string[1024];
    printf("string: ");
    scanf("%s", string);
    char font[1024];
    printf("font: ");
    scanf("%s", font);
    unsigned int size = 0;
    printf("size: ");
    scanf("%u", &size);
    figure_t f = text(string, font, size);
    svg.im = append(svg.im, f);
  }else{
    printf("il n'exist pas de fichier ouvert\n");
  }
  sleep(1);
}

void ajouter_image(){
  if(file_exist){
    char image[1024];
    printf("image: ");
    scanf("%s", image);
    unsigned int w = 0;
    printf("longeur: ");
    scanf("%u", &w);
    unsigned int h = 0;
    printf("largeur: ");
    scanf("%u", &h);
    figure_t f = photo(image, w, h);
    svg.im = append(svg.im, f);
  }else{
    printf("il n'exist pas de fichier ouvert\n");
  }
  sleep(1);
}

void supprimer() {
  if(file_exist){
    image_info(svg.im);
    unsigned int id = 0;
    printf("ID: ");
    scanf("%u", &id);
    if(id>0 && id<=svg.im.size)
      svg.im.tab[id-1].type = 0;
  }else{
    printf("il n'exist pas de fichier ouvert\n");
  }
  sleep(1);
}

void translater() {
  if(file_exist){
    image_info(svg.im);
    unsigned int id = 0;
    printf("ID: ");
    scanf("%u", &id);
    double tx = 0;
    printf("tx: ");
    scanf("%lf", &tx);
    double ty = 0;
    printf("ty: ");
    scanf("%lf", &ty);
    if(id>0 && id<=svg.im.size)
      svg.im.tab[id-1] = translate(svg.im.tab[id-1], tx, ty);
  }else{
    printf("il n'exist pas de fichier ouvert\n");
  }
  sleep(1);
}

void tourner() {
  if(file_exist){
    image_info(svg.im);
    unsigned int id = 0;
    printf("ID: ");
    scanf("%u", &id);
    double r = 0;
    printf("tourner: ");
    scanf("%lf", &r);
    if(id>0 && id<=svg.im.size)
      svg.im.tab[id-1] = rotate(svg.im.tab[id-1], r);
  }else{
    printf("il n'exist pas de fichier ouvert\n");
  }
  sleep(1);
}

void etirer() {
  if(file_exist){
    image_info(svg.im);
    unsigned int id = 0;
    printf("ID: ");
    scanf("%u", &id);
    double s = 0;
    printf("etirer: ");
    scanf("%lf", &s);
    if(id>0 && id<=svg.im.size)
      svg.im.tab[id-1].scale *= s;
  }else{
    printf("il n'exist pas de fichier ouvert\n");
  }
  sleep(1);
}

void fonction(){
  printf("dans la prochaine version ...\n");
  sleep(1);
}


/*int main(int argc, char const *argv[])
{
  menu_t* m = projectMenu();
  launchMenu(m);
  return 0;
}*/
