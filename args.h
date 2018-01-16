#ifndef ARGS_H
#define ARGS_H

typedef struct args_s{
  const char* key;
  enum {
    OptString = 1,
    OptFloat,
    OptFloatFLoat,
    OptInt,
    OptIntFloat,
    OptIntFloatFLoat
  } type;
  union {
    void (*opt_string)(const char*);
    void (*opt_float)(float);
    void (*opt_float_float)(float,float);
    void (*opt_int)(int);
    void (*opt_int_float)(int, float);
    void (*opt_int_float_float)(int, float);
  } fun;
  struct args_s* next;
}args_t;

#endif
