#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include "json.h"
#define json_booleans() (json_element_t*)e->type=JSON_BOOLAN
#define json_null() (json_element_t*)NULL
#define prl { printf("\n"); }
#define TAILLE_MAX 1000


struct json_element_s{
    json_type_t type;
    union{
        bool boolean;
        struct{
            enum{
                LONG,
                DOUBLE,
            } longORdouble;
            union{
                long  LONG;
                double DOUBLE;
            };
        } number;
        char* string;
        struct{
            json_element_t* *tab;
            size_t sz;
        } array;
        list_t* list;
    };
};


json_type_t json_type(const json_element_t* e) {
    return e->type;
}



json_element_t* json_boolean(bool b) {
    json_element_t* x = malloc(sizeof(json_element_t));
    x->type = JSON_BOOLEAN;
    x->boolean = b;
    return x;
}


bool json_as_boolean(const json_element_t* e) {
    if (e->type != JSON_BOOLEAN) {
        fprintf(stderr, "It is the wrong type\n");
        exit(EXIT_FAILURE);
    }
    return e->boolean;
}



json_element_t* json_integer(long i) {
    json_element_t* x = malloc(sizeof(json_element_t));
    x->type = JSON_NUMBER;
    x->number.longORdouble = LONG;
    x->number.LONG = i;
    return x;    
}


json_element_t* json_double(double d) {
    json_element_t* x = malloc(sizeof(json_element_t));
    x->type = JSON_NUMBER;
    x->number.longORdouble = DOUBLE;
    x->number.DOUBLE = d;
    return x;    
}


bool json_is_integer(const json_element_t* e) {
    if (json_type(e) == JSON_NUMBER) {
        if (e->number.longORdouble == LONG) return true;
    } else {
        printf("e is not good type");
        return EXIT_FAILURE;
    }
    return EXIT_FAILURE;
}


bool json_is_double(const json_element_t* e) {
    if (json_type(e) == JSON_NUMBER) {
        if (e->number.longORdouble == DOUBLE) return true;
    } else {
        printf("e is not good type");
        return EXIT_FAILURE;
    }
    return EXIT_FAILURE;
}


long json_as_integer(const json_element_t* e) {
    if (json_type(e) == JSON_NUMBER) {
        if (e->number.longORdouble == LONG) return e->number.LONG;
        else return (long)e->number.DOUBLE;
    } else {
        printf("e is not good type");
        return EXIT_FAILURE;
    }
}


double json_as_double(const json_element_t* e) {
    if (json_type(e) == JSON_NUMBER) {
        if (e->number.longORdouble == LONG) return (double)e->number.LONG;
        else return e->number.DOUBLE;
    } else {
        printf("e is not good type");
        return EXIT_FAILURE;
    }    
}


json_element_t* json_string(const char* s) {
    json_element_t* e = malloc(sizeof(json_element_t));
    e->type = JSON_STRING;
    e->string = malloc(strlen(s)+1);
    e->string = strcpy(e->string, s);
    return e ;
}


const char* json_as_string(const json_element_t* e) {
    if (json_type(e) == JSON_STRING) return e->string;
    else {
        printf("e is not good type");
        return (char*)EXIT_FAILURE;
    }
}



size_t json_array_size(json_element_t* e) {
    return e->array.sz;
}


json_element_t* json_array() {
    json_element_t* x = malloc(sizeof(json_element_t));
    x->type = JSON_ARRAY;
    x->array.sz = 0;
    x->array.tab = NULL;
    return x;
}


json_element_t* json_array_get(const json_element_t* e, size_t i) {
    if (json_type(e) == JSON_ARRAY) {
        if (i >= e->array.sz) return json_null();
        else return e->array.tab[i];
    } else {
         printf("e is not good type");
        return (json_element_t*)EXIT_FAILURE;
    }
}


struct list_s{
    char* key;
    json_element_t* value;
    list_t* next;
};


json_element_t* json_object() {
    json_element_t* e = malloc(sizeof(json_element_t));
    e->type = JSON_OBJECT;
    e->list = NULL;
    return e;
}


void json_array_set(json_element_t* e, size_t i, json_element_t* se) {
    if (json_type(e) != JSON_ARRAY) {
        printf("Error : wrong type");prl;
        exit(EXIT_FAILURE);
    } else if (e->array.sz == 0 && i == 0) {
        e->array.tab = realloc (e->array.tab, sizeof(json_element_t*)*(e->array.sz));
        e->array.tab[i] = se;
        e->array.sz = 1;
    } else if ((size_t)i <= e->array.sz) e->array.tab[i] = se;
    else if ((size_t)i > e->array.sz) {
        e->array.tab = realloc(e->array.tab, sizeof(json_element_t*)*(e->array.sz+i+1)+1);
        for (int j = e->array.sz+1; j < (int)i; j ++) {
            e->array.tab[j] = json_null();
        }
        e->array.tab[i] = se;
        e->array.sz = i+1;
    }
}


list_t* createObject(const char* key, json_element_t* value) {
    list_t* e = malloc(sizeof(list_t));
    char* copy = malloc(strlen(key)+1);
    e->key = malloc(strlen(key)+1);
    e->key = strcpy(copy, key);
    e->value = value;
    e->next = NULL;
    return e;    
}


void json_object_set(json_element_t* e, const char* key, json_element_t* value) {
    if (e->list == NULL) {
        e->list = createObject(key, value);
        return;
    }
    list_t* first = e->list;
    list_t* second;
    while (first != NULL) {
        if (!strcmp(first->key, key)) {
            json_free(first->value);
            first->value = value;
            return;
        }
        second = first;
        first = first->next;
    }
    second->next = (list_t*)createObject(key, value);
}


json_element_t* json_object_get(const json_element_t* e, const char* key) {
    list_t* o = e->list;
    while (o != NULL) {
        if (!strcmp(o->key, key)) return o->value;
        o = o->next;
    }
    return json_null();
}


void json_put(FILE* fd, const json_element_t* e) {
    if (e == json_null()) fprintf(fd, "null");
    else if (json_type(e) == JSON_NUMBER) {
        if (e->number.longORdouble == DOUBLE) fprintf(fd, "%.8f", json_as_double(e));
        else if (e->number.longORdouble == LONG) fprintf(fd, "%ld", json_as_integer(e));
    } else if (e->type == JSON_BOOLEAN) fprintf(fd, json_as_boolean(e)?"true":"false");
    else if (json_type(e) == JSON_STRING) fprintf(fd, "\"%s\"", json_as_string(e));
    else if (json_type(e) == JSON_ARRAY) {
        size_t i;
        fprintf(fd, "[");
        int compteur = 0;
        for (i = 0; i < e->array.sz; i++) {
            json_put(fd, e->array.tab[i]);
            compteur++;
            if ((size_t)compteur != e->array.sz) fprintf(fd, ",");
        }
        fprintf(fd, "]");
    } else if (json_type(e) == JSON_OBJECT) {
        list_t* o = e->list;
        list_t* a = e->list;
        fprintf(fd, "{");
        int compteur = 0;
        int compteur2 = 0;
        while (a != NULL) {
            compteur++;
            a = a->next;
        } while (o != NULL) {
            fprintf(fd, "\"%s\":", o->key);
            json_put(fd, o->value);
            o = o->next;
            if (compteur2 != compteur-1) {
                compteur2++;
                fprintf(fd, ",");
            }
        }
        fprintf(fd, "}");
    }
}


void json_save(const char* fname, json_element_t* e) {
    FILE* fichier = NULL;
    fichier = fopen(fname, "w");
    if (fichier != NULL) json_put(fichier, e);
    else printf("file can't open.txt");
}


char next_char(FILE* fd) {
    char x;
        while ((x = fgetc(fd)) != EOF) {
            if (x != '\n' && x != '\t' && x != '\r' && x != ' ' ) return x;
        }
    exit(EXIT_FAILURE);
}


void json_free(json_element_t* e) {
    if (e == (json_element_t*)JSON_BOOLEAN) free(e);
    else if (e == (json_element_t*)JSON_NUMBER) free(e);
    else if (e == (json_element_t*)JSON_STRING) {
        free(e->string);
        free(e);
    } else if (e == (json_element_t*)JSON_ARRAY) {
        size_t i = 0;
        while (i < e->array.sz) {
            json_free(e->array.tab[i]);
            i++;
        }
        free(e->array.tab);
        free(e);
    } else if (e == (json_element_t*)JSON_OBJECT) {
        list_t* o = e->list;
        while (o != NULL) {
            free(o->key);
            json_free(o->value);
            o = o->next;
        }
        free(e);
    }
}


void check_next_char(FILE* fd, char c) {
    if (next_char(fd) != c) EXIT_FAILURE;
}


void check_next_word(FILE* fd, const char* w) {
    char chaine[strlen(w)+1];
    fgets(chaine, strlen(w)+1, fd);
    chaine[strlen(w)] = '\0';
    if (fd != NULL && strcmp(chaine, w) != 0) exit (EXIT_FAILURE);
}


bool is_next_char(FILE* fd, char c, bool cons) {
        char x;
        x = next_char(fd);
        if (x != c || cons == false) {
            ungetc(x, fd);
            return false;
        }
    return true;
}


char* next_string(FILE* fd) {
    char x = next_char(fd);
    char* chaine = NULL;
    while (x != '"' && x != EOF) x = next_char(fd);
    int i = 0;
    if (x == '"') {
    while ((x = fgetc(fd)) != '"' && x != EOF) {
        chaine = realloc(chaine, sizeof(char)*(i+2));
        if (chaine == NULL) exit(EXIT_FAILURE);
        chaine[i] = x;
        i++;
    }
    chaine[i] = '\0';
    return chaine;
    } else exit(EXIT_FAILURE);
}


char* next_digits(FILE* fd) {
    char x = next_char(fd);
    char* chaine = NULL;
    while(!(x == '0' || x == '1' || x == '2' || x == '3' || x == '4' || x == '5' || x == '6' || x == '7' || x == '8' || x == '9') && x != EOF ) {
        x=next_char(fd);
    }
    int i = 0;
    while((x == '0' || x == '1' || x == '2' || x == '3' || x == '4' || x == '5' || x == '6' || x == '7' || x == '8' || x == '9') && x != EOF ) {
        chaine = realloc(chaine, sizeof(char)*(i+2));
        if(chaine == NULL) exit(EXIT_FAILURE);
        chaine[i] = x;
        i++;
        x = fgetc(fd);
    }
    ungetc(x, fd);
    chaine[i+1] = '\0';
    return chaine;
}


json_element_t* json_get(FILE* fd) {
    int c = next_char(fd);
    if (c == 'n') {
        ungetc(c, fd);
        check_next_word(fd, "null");
        return json_null();
    }
    if (c == 't') {
        ungetc(c, fd);
        check_next_word(fd, "true");
        return json_boolean(true);
    }
    if (c == 'f') {
        ungetc(c, fd);
        check_next_word(fd, "false");
        return json_boolean(false);
    }
    if (c == '-') {
        json_element_t* se = json_get(fd);
        if (json_type(se) != JSON_NUMBER) {
            fprintf(stderr, "A number is expected after character '-'\n");
            exit(EXIT_FAILURE);
        }
        if (json_is_double(se)) {
            double d = json_as_double(se);
            json_free(se);
            return json_double(-d);
        }
        if (json_is_integer(se)) {
            long i = json_as_integer(se);
            json_free(se);
            return json_integer(-i);
        }
    }
    if ('0' <= c && c <= '9') {
        ungetc(c, fd);
        char* number = next_digits(fd);
        c = fgetc(fd);
        if (c == '.') {
            char* decimal = next_digits(fd);
            number = realloc(number, strlen(number) + strlen(decimal) + 2);
            strcat(number, ".");
            strcat(number, decimal);
            json_element_t* e = json_double(strtod(number, NULL));
            free(number);
            free(decimal);
            return e;
        } else {
            ungetc(c, fd);
            json_element_t* e = json_integer(strtol(number, NULL, 10));
            free(number);
            return e;
        }
    }
    if (c == '"') {
        ungetc(c, fd);
        char* str = next_string(fd);
        json_element_t* e = json_string(str);
        free(str);
        return e;
    }
    if (c == '[') {
        json_element_t* e = json_array();
        size_t i =0;
        if (is_next_char(fd, ']', true)) return e;
        while (1) {
            json_element_t* se = json_get(fd);
            json_array_set(e, i++, se);
            c = next_char(fd);
            if (c == ']') return e;
            if (c != ',') {
                fprintf(stderr, "Unexpected character '%c'; ']' or ',' excepted\n", c);
                exit(EXIT_FAILURE);
            }
        }
    }
    if (c == '{') {
        json_element_t* e = json_object();
        if (is_next_char(fd, '}', true)) return e;
        while (1) {
            check_next_char(fd, '"');
            ungetc('"', fd);
            char* key = next_string(fd);
            check_next_char(fd, ':');
            json_element_t* value = json_get(fd);
            json_object_set(e, key, value);
            c = next_char(fd);
            if (c == '}') return e;
            if (c != ',') {
                fprintf(stderr, "Unnexpected character '%c'; ']' or ',' excepted\n", c);
                exit(EXIT_FAILURE);
            }
        }
    }
    fprintf(stderr, "Unexpected character '%c'; 'n', 't', 'f', [0-9], '\"', '[' or '{' excepted\n", c);
    exit(EXIT_FAILURE);
}


json_element_t* json_load(const char* fname) {
    FILE* fd = fopen(fname, "r");
    json_element_t* x = json_get(fd);
    fclose(fd);
    return x;
}
