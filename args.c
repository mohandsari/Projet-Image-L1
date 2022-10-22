#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "args.h"
#define NOOPTION ((option_t*)NULL)


option_t* opt_void(option_t* l, const char* kw, void (*f)()) {
    option_t* curr = l;
    option_t* prev = NULL;
    while (curr != NULL) {
        prev = curr;
        curr = curr->next;
    }
    if (prev == NULL) {
        l = (option_t*) malloc(sizeof(struct option_s));
        if (l == NULL) {
            printf("memory allocation failed");
            exit(EXIT_FAILURE);
        }
        l->keyword = kw;
        l->spec = OptVoid;
        l->fct.opt_void = f;
        l->next = NULL;
    } else {
        prev->next = (option_t*) malloc(sizeof(struct option_s));
        if (prev->next == NULL) {
            printf("memory allocation failed");
            exit(EXIT_FAILURE);
        }
        prev->next->keyword = kw;
        prev->next->spec = OptVoid;
        prev->next->fct.opt_void = f;
        prev->next->next = NULL;
    }
    return l;
}


option_t* opt_int(option_t* l, const char* kw, void (*f)(int)) {
    option_t* curr = l;
    option_t* prev = NULL;
    while (curr != NULL) {
        prev = curr;
        curr = curr->next;
    }
    if (prev == NULL) {
        l = (option_t*) malloc(sizeof(struct option_s));
        if (l == NULL) {
            printf("memory allocation failed");
            exit(EXIT_FAILURE);
        }
        l->keyword = kw;
        l->spec = OptInt;
        l->fct.opt_int = f;
        l->next = NULL;
    } else {
        prev->next = (option_t*) malloc(sizeof(struct option_s));
        if (prev->next == NULL) {
            printf("memory allocation failed");
            exit(EXIT_FAILURE);
        }
        prev->next->keyword = kw;
        prev->next->spec = OptInt;
        prev->next->fct.opt_int = f;
        prev->next->next = NULL;
    }
    return l;
}


option_t* opt_float(option_t* l, const char* kw, void (*f)(float)) {
    option_t* curr = l;
    option_t* prev = NULL;
    while (curr != NULL) {
        prev = curr;
        curr = curr->next;
    }
    if (prev == NULL) {
        l = (option_t*) malloc(sizeof(struct option_s));
        if (l == NULL) {
            printf("memory allocation failed");
            exit(EXIT_FAILURE);
        }
        l->keyword = kw;
        l->spec = OptFloat;
        l->fct.opt_float = f;
        l->next = NULL;
    } else {
        prev->next = (option_t*) malloc(sizeof(struct option_s));
        if (prev->next == NULL) {
            printf("memory allocation failed");
            exit(EXIT_FAILURE);
        }
        prev->next->keyword = kw;
        prev->next->spec = OptFloat;
        prev->next->fct.opt_float = f;
        prev->next->next = NULL;
    }
    return l;
}


option_t* opt_string(option_t* l, const char* kw, void (*f)(const char*)) {
    option_t* curr = l;
    option_t* prev = NULL;
    while (curr != NULL) {
        prev = curr;
        curr = curr->next;
    }
    if (prev == NULL) {
        l = (option_t*) malloc(sizeof(struct option_s));
        if (l == NULL) {
            printf("memory allocation failed");
            exit(EXIT_FAILURE);
        }
        l->keyword = kw;
        l->spec = OptString;
        l->fct.opt_str = f;
        l->next = NULL;
    } else {
        prev->next = (option_t*) malloc(sizeof(struct option_s));
        if (prev->next == NULL) {
            printf("memory allocation failed");
            exit(EXIT_FAILURE);
        }
        prev->next->keyword = kw;
        prev->next->spec = OptString;
        prev->next->fct.opt_str = f;
        prev->next->next = NULL;
    }
    return l;
}


void opt_delete(option_t* l) {
    if (l->next != NULL) opt_delete(l->next);
    free(l);
}


void process_options(option_t* l, int argc, char* *argv) {
    option_t* curr = NULL;
    for (int i = 1; i < argc; i++) {
        curr = l;
        if (argv[i][0] == '-') {
            while (strcmp(curr->keyword,argv[i])) curr = curr->next;
            switch(curr->spec) {
                case (OptVoid) : curr->fct.opt_void(); break;
                case (OptInt) : i++; curr->fct.opt_int(atoi(argv[i])); break;
                case (OptFloat) : i++; curr->fct.opt_float(atof(argv[i])); break;
                case (OptString) : i++; curr->fct.opt_str(argv[i]); break;
            }
        }
    }
}
