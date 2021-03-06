#include "args.h"

args_t* arg_Void(args_t* arg, const char* key, void (*fun)(void)){
  args_t* ar = (args_t*)malloc(sizeof(args_t));
  ar->key = key;
  ar->type = ArgVoid;
  ar->fun.arg_void = fun;
  ar->next = arg;
  return ar;
}

args_t* arg_String(args_t* arg, const char* key, void (*fun)(const char*)){
  args_t* ar = (args_t*)malloc(sizeof(args_t));
  ar->key = key;
  ar->type = ArgString;
  ar->fun.arg_string = fun;
  ar->next = arg;
  return ar;
}

args_t* arg_Float(args_t* arg, const char* key, void (*fun)(float)){
  args_t* ar = (args_t*)malloc(sizeof(args_t));
  ar->key = key;
  ar->type = ArgFloat;
  ar->fun.arg_float = fun;
  ar->next = arg;
  return ar;
}

args_t* arg_Float_Float(args_t* arg, const char* key, void (*fun)(float, float)){
  args_t* ar = (args_t*)malloc(sizeof(args_t));
  ar->key = key;
  ar->type = ArgFloatFLoat;
  ar->fun.arg_float_float = fun;
  ar->next = arg;
  return ar;
}

args_t* arg_Int(args_t* arg, const char* key, void (*fun)(int)){
  args_t* ar = (args_t*)malloc(sizeof(args_t));
  ar->key = key;
  ar->type = ArgInt;
  ar->fun.arg_int = fun;
  ar->next = arg;
  return ar;
}

args_t* arg_Int_Float(args_t* arg, const char* key, void (*fun)(int, float)){
  args_t* ar = (args_t*)malloc(sizeof(args_t));
  ar->key = key;
  ar->type = ArgIntFloat;
  ar->fun.arg_int_float = fun;
  ar->next = arg;
  return ar;
}

args_t* arg_Int_Float_Float(args_t* arg, const char* key, void (*fun)(int, float, float)){
  args_t* ar = (args_t*)malloc(sizeof(args_t));
  ar->key = key;
  ar->type = ArgIntFloatFLoat;
  ar->fun.arg_int_float_float = fun;
  ar->next = arg;
  return ar;
}

void launchArgs(args_t* l, int argc, const char ** argv){
  int i;
  int nb1;
  float f1, f2;
  args_t* tmp;
  for(i = 1; i < argc ; i++){
    tmp = l;
    while(tmp != NULL){
      if(strcmp(tmp->key, argv[i]) == 0){
        switch (tmp->type){
          case ArgVoid:
            tmp->fun.arg_void();
            break;
          case ArgString:
            tmp->fun.arg_string(argv[i+1]);
            i++;
            break;
          case ArgFloat:
            f1 = (float)atof(argv[i+1]);
            tmp->fun.arg_float(f1);
            i++;
            break;
          case ArgInt:
            nb1 = atoi(argv[i+1]);
            tmp->fun.arg_int(nb1);
            i++;
            break;
          case ArgFloatFLoat:
            f1 = atof(argv[i+1]);
            f2 = atof(argv[i+2]);
            tmp->fun.arg_float_float(f1,f2);
            i+=2;
            break;
          case ArgIntFloat:
            nb1 = atoi(argv[i+1]);
            f2 = atof(argv[i+2]);
            tmp->fun.arg_int_float(nb1,f2);
            i+=2;
            break;
          case ArgIntFloatFLoat:
            nb1 = atoi(argv[i+1]);
            f1 = atoi(argv[i+2]);
            f2 = atof(argv[i+3]);
            tmp->fun.arg_int_float_float(nb1,f1,f2);
            i+=3;
            break;
          default: printf("Arguments not found");
            exit(EXIT_FAILURE);
        }
      }
      tmp = tmp->next;
    }
  }
}

void projectArgs(int argc, const char** argv){
  args_t* arg = NULL;
  arg = arg_String(arg, "-i", argOpen);
  arg = arg_String(arg, "-o", argSave);
  arg = arg_Float(arg, "-c", argCircle);
  arg = arg_Float_Float(arg, "-r", argRect);
  arg = arg_Float(arg, "-l", argLine);
  arg = arg_String(arg, "-x",argText);
  arg = arg_String(arg, "-j",argPhoto);
  arg = arg_Void(arg, "-h",argHelp);
  launchArgs(arg, argc, argv);
  free(arg);
}

void argOpen(const char* fname){
  if(strstr(fname, ".svg")!=NULL){
    xelement_t* xml = load_xml(fname);
    if(xml==NULL){
      exit(EXIT_FAILURE);
    }
    svg = xml_to_svg(xml);
    file_exist = true;
  }else{
    printf("c'est pas un fichier .svg\n");
  }
}

void argSave(const char* fname){
  if (file_exist) {
    if(strstr(fname, ".pdf")!=NULL){
      pdf_t* pdf = svg_to_pdf(svg);
      pdf_save(fname, pdf);
    }else{
      printf("c'est pas un fichier .pdf\n");
    }
  }else{
    printf("il n'exist pas de fichier ouvert\n");
  }
}

void argCircle(float r){
  if (file_exist) {
    figure_t f = circle(r);
    svg.im = append(svg.im, f);
  }else{
    printf("il n'exist pas de fichier ouvert\n");
  }
}

void argRect(float w, float h){
  if (file_exist) {
    figure_t f = rectangle(w, h);
    svg.im = append(svg.im, f);
  }else{
    printf("il n'exist pas de fichier ouvert\n");
  }
}

void argLine(float l){
  if (file_exist) {
    figure_t f = line(l);
    svg.im = append(svg.im, f);
  }else{
    printf("il n'exist pas de fichier ouvert\n");
  }
}

void argText(const char* string){
  if (file_exist) {
    figure_t f = text((char*)string, "Times-Roman", 3);
    svg.im = append(svg.im, f);
  }else{
    printf("il n'exist pas de fichier ouvert\n");
  }
}

void argPhoto(const char* fname){
  if (file_exist) {
    figure_t f = photo((char*)fname, 400, 400);
    svg.im = append(svg.im, f);
  }else{
    printf("il n'exist pas de fichier ouvert\n");
  }
}

void argHelp(void){
  printf("-i + nom_de_fichier : charge en image courante limage contenue dans le fichier de nom nom_de_fichier\n-o + nom_de_fichier : sauvegarde limage courante dans le fichier de nom nom_de_fichier\n-c + flottant : ajoute un cercle centre en (0,0) avec le rayon donne\n-r + flottant + flottant : ajoute un rectangle centre en (0,0) avec les dimensions données\n-l + flottant : ajoute une ligne horizontale centre en (0,0) de longueur donnee\n-x + texte_entre_guillement : ajoute le texte donne à la position (0,0)\n-j + nom_fichier : ajoute limage contenu dans le fichier a la position (0,0)\n");
}

/*int main(int argc, char const *argv[]) {
  projectArgs(argc, argv);
  return 0;
}*/
