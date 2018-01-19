#include "pdf.h"
#include "menu.h"
#include "args.h"
#include "xml.h"

svg_t svg;
bool file_exist;

int main(int argc, char const *argv[]){
  file_exist = false;
  if(argc>1){
    projectArgs(argc, argv);
  }else{
    menu_t* m = projectMenu();
    launchMenu(m);
  }
  return EXIT_SUCCESS;
}
