#ifndef _ARGS_Mohand_ou_kaci_and_Baptiste_Projet2018_L2Info_
#define _ARGS_Mohand_ou_kaci_and_Baptiste_Projet2018_L2Info_
#include <stdlib.h>
#include <stdio.h>


struct option_s {
  const char* keyword;
  enum { OptVoid, OptInt, OptString, OptFloat } spec;
  union {
    void (*opt_void)(), (*opt_int)(int), (*opt_str)(const char*), (*opt_float)(float);
  } fct;
  struct option_s* next;
};

typedef struct option_s option_t;

option_t* opt_void(option_t* l, const char* kw, void (*f)());
option_t* opt_int(option_t* l, const char* kw, void (*f)(int));
option_t* opt_float(option_t* l, const char* kw, void (*f)(float));
option_t* opt_string(option_t* l, const char* kw, void (*f)(const char*));
void opt_delete(option_t* l);
void process_options(option_t* l, int argc, char* *argv);


#endif
