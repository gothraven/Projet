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



/*
  "font_size": 11,
  "ignored_packages":
  [
    "Vintage"
  ],
  "tab_size": 2,
    "translate_tabs_to_spaces": true,
    "keys": ["alt+shift+c"], "command": "reindent", "args": {"single_line": false}
*/
#endif
