#ifndef XML_H
#define XML_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define BUFF_SIZE 100

typedef struct xattribute xattribute_t;
typedef struct xelement xelement_t;

struct xattribute {
    char* nom;
    char* valeur;
    xattribute_t* next;
};

union contenu {
        char* raw;
        xelement_t* fils;
    };

struct xelement {
    char* nom;
    xattribute_t* attributs;
    xelement_t* pere;
    xelement_t* frere;
    enum name {
        RIEN,
        LISTE,
        RAW
    } type;
    union contenu *contenu;
};

typedef struct xattribute xattribute_t;
typedef struct xelement xelement_t;

struct xattribute;
struct xelement;
xelement_t* create_xelement(const char* name);
xattribute_t* add_xattribute(xelement_t* e, const char* name, const char* value);
void add_sub_xelement(xelement_t* e, xelement_t* s);
void add_raw(xelement_t* e, const char* r);
void save_xelement(FILE* fd, xelement_t* e);
void delete_xelement(xelement_t* e);
char next_char(FILE* fd);
void check_next_char(FILE* fd, char c);
bool is_next_char(FILE* fd, char c, bool cons);
char* next_word(FILE* fd);
void check_next_word(FILE* fd, const char* w);
char* next_raw(FILE* fd);
char* get_attribute_value(xattribute_t* a, char* str);
xelement_t* load_xml(const char* fname);

#endif
