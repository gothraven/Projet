#include "pdf.h"
#include "menu.h"
#include "args.h"
#include "xml.h"

int main(int argc, char const *argv[]){
  if(argc>1){
    projectArgs(argc, argv);
  }else{
  menu_t* m = projectMenu();
  launchMenu(m);
  }
  return EXIT_SUCCESS;
}
