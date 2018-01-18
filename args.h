#ifndef ARGS_H
#define ARGS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct args_s{
  const char* key;
  enum {
    ArgVoid = 1,
    ArgString,
    ArgFloat,
    ArgFloatFLoat,
    ArgInt,
    ArgIntFloat,
    ArgIntFloatFLoat
  } type;
  union {
    void (*arg_void)(void);
    void (*arg_string)(const char*);
    void (*arg_float)(float);
    void (*arg_float_float)(float,float);
    void (*arg_int)(int);
    void (*arg_int_float)(int, float);
    void (*arg_int_float_float)(int, float, float);
  } fun;
  struct args_s* next;
}args_t;

extern svg_t svg;
extern bool file_exist;

args_t* arg_String(args_t* arg, const char* key, void (*fun)(const char*));
args_t* arg_Float(args_t* arg, const char* key, void (*fun)(float));
args_t* arg_Float_Float(args_t* arg, const char* key, void (*fun)(float, float));
args_t* arg_Int(args_t* arg, const char* key, void (*fun)(int));
args_t* arg_Int_Float(args_t* arg, const char* key, void (*fun)(int, float));
args_t* arg_Int_Float_Float(args_t* arg, const char* key, void (*fun)(int, float, float));
void launchArgs(args_t* l, int argc, const char ** argv);
void projectArgs(int argc, const char** argv);
void argOpen(const char* fname);
void argSave(const char* fname);
void argCircle(float r);
void argRect(float w, float h);
void argLine(float l);
void argText(const char* text);
void argPhoto(const char* fname);
void argHelp(void);

#endif
