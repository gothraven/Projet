#ifndef ARGS_H
#define ARGS_H

typedef struct args_s{
  const char* key;
  enum {
    ArgString = 1,
    ArgFloat,
    ArgFloatFLoat,
    ArgInt,
    ArgIntFloat,
    ArgIntFloatFLoat
  } type;
  union {
    void (*arg_string)(const char*);
    void (*arg_float)(float);
    void (*arg_float_float)(float,float);
    void (*arg_int)(int);
    void (*arg_int_float)(int, float);
    void (*arg_int_float_float)(int, float, float);
  } fun;
  struct args_s* next;
}args_t;

#endif
