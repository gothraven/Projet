#ifndef PDF_H
#define PDF_H

#include <stdio.h>

typedef struct object pdf_object_t;
typedef struct object_dict pdf_dict_t;
typedef struct object_tab pdf_tab_t;
typedef struct object_stream pdf_stream_t;

struct object_dict
{
	char* nom;
	pdf_object_t* valeur;
	struct object_dict* next;
};

struct object_tab
{
	pdf_object_t** tab;
	int size;
};

struct object_stream
{
	struct object_dict* dict;
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
	}
};

#endif