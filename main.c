#include "pdf.h"
#include "menu.h"

int main(){
  menu_t* m = projectMenu();
  launchMenu(m);
  return EXIT_SUCCESS;
}
