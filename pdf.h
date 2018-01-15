#ifndef PDF_H
#define PDF_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct object pdf_object_t;
typedef struct object_dict pdf_dict_t;
typedef struct object_tab pdf_tab_t;
typedef struct object_stream pdf_stream_t;
typedef struct object_pdf pdf_t;

struct object_dict
{
	char* cle;
	pdf_object_t* valeur;
	struct object_dict* next;
};

struct object_tab
{
	pdf_object_t** tab;
	size_t size;
};

struct object_stream
{
	pdf_object_t* dict;
	void* valeur;
  size_t size;
};

struct object
{
	enum {
		null = 1,
		boolean,
		entier,
		reel,
		string,
		nom,
		stream,
		tab,
		dict,
		ref
	}	type;
	union {
		bool boolean;
		int entier;
		float reel;
		char* string;
		char* nom;
		int ref;
		struct object_stream* stream;
		struct object_tab* tab;
		struct object_dict* dict;
	};
};

struct object_pdf
{
  unsigned int w; //largeur
  unsigned int h; //hauteur
  unsigned int taille; //taille de tableau
  struct object** tab_objs; //tableau des objets pdf
};

void pdf_object_free(pdf_object_t* o);
void pdf_stream_free(pdf_object_t* o);
void pdf_dict_free(pdf_object_t* o);
void pdf_tab_free(pdf_object_t* o);
pdf_object_t* pdf_null(pdf_object_t* o);
pdf_object_t* pdf_bool(pdf_object_t* o, bool b);
pdf_object_t* pdf_int(pdf_object_t* o, int i);
pdf_object_t* pdf_real(pdf_object_t* o, float f);
pdf_object_t* pdf_reference(pdf_object_t* o, unsigned int id);
pdf_object_t* pdf_name(pdf_object_t* o, const char* n);
pdf_object_t* pdf_string(pdf_object_t* o, const char* s);
pdf_object_t* pdf_stream(pdf_object_t* o, size_t sz, void* data);
pdf_object_t* pdf_array(pdf_object_t* o, size_t n);
pdf_object_t* pdf_dictionary(pdf_object_t* o);
pdf_object_t* pdf_stream_from_file(pdf_object_t* o, const char* fname);
pdf_object_t* pdf_stream_from_string(pdf_object_t* o, const char* str);
pdf_object_t* pdf_array_get(pdf_object_t* a, size_t i);
pdf_object_t* pdf_dict_get(pdf_object_t* d, const char* cle);
size_t pdf_fprint_object(FILE* fd, pdf_object_t* o);
size_t  pdf_dict_fprint(FILE* fd, pdf_dict_t* dict);
void pdf_set_content(pdf_t* p, unsigned int n, const char* instr);
const char* pdf_load_image(pdf_t* p, const char* fname);

size_t  pdf_tab_fprint(FILE* fd, pdf_tab_t* tab);
size_t  pdf_stream_fprint(FILE* fd, pdf_stream_t* stream);
pdf_object_t* pdf_default_fonts(pdf_object_t* f);
bool jpeg_info(const void* i, size_t* width, size_t* height, size_t* bpc, const char** cs);
void pdf_delete(pdf_t* p);
size_t pdf_new_id(pdf_t* p);
void pdf_save(const char* fname, pdf_t* p);

#endif
