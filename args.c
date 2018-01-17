#include "args.h"


args_t* arg_String(args_t* arg, const char* key, void (*fun)(const char*)){
  args_t* ar = (args_t*)malloc(sizeof(args_t));
  ar->key = key;
  ar->type = ArgString;
  ar->fun.arg_string = fun;
  ar->next = arg
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

void launchArgs(args_t* l, int argc, char ** argv){
  int i;
  int nb1;
  float f1, f2,;
  args_t* tmp;
  printf("\n");
  for(i = 1; i < argc ; i++){
    tmp = l;
    while(tmp != NULL){
      if(strcmp(tmp->keyword, argv[i]) == 0){
        switch (tmp->type){
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
            tmp->fun.arg_int_float_float(nb1,f2,f3);
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

void projectArgs(int argc, char** argv){

  args_t* arg = NOOPTION;

//  arg = arg_String(arg, "-i", argOpen);
//  arg = arg_String(arg, "-o", argSave);
//  arg = arg_Float(arg, "-c", argCircle);
  arg = opt_void(arg, "-r", optReverse);
  arg = opt_2unsignedInt(arg, "-c", optCrop);
  arg = opt_float(arg, "-s",optScale);
  arg = opt_unsignedInt(arg, "-vp",optVolumeUp);
  arg = opt_unsignedInt(arg, "-vd",optVolumeDown);
  arg = opt_void(arg, "-h",optHelp);

  process_arguments(arg, argc, argv);

  free(opt);

}

void argOpen(char* fname) {
  printf("ouvrir %s\n",fname);
}

void argSave(char* fname) {
  printf("sauvgarder %s\n",fname);
}

void argCircle(float r) {
  printf("cirlce de rayon %f\n",r);
}
