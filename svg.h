
#ifndef SVG_H
#define SVG_H

#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<math.h>

typedef char* color_t;

typedef struct coord_s
{
    double x;
    double y;
}coord_t;

typedef struct circle_s{
    double r;
}circle_t;

typedef struct rectangle_s{
    double w; // demie longueur
    double h; // demie hauteur
}rectangle_t;

typedef struct line_s{
     double l; // demie longueur
}line_t;

typedef struct photo_s{
    char     *name;
}photo_t;

typedef struct text_s{
    char     *name;

}text_t;

typedef struct	figure_s{
    color_t color;
    coord_t centre;
    double angle;
    enum {
        RIEN,
        CERCLE,
        RECTANGLE,
        LIGNE,
        PHOTO,
        GROUPE
    } type;
    union {
      circle_t cercle;
      rectangle_t rectangle;
      line_t line;
      text_t text;
    	photo_t photo;
    };
}figure_t;

typedef struct	image_s
{
	unsigned int w; //width
	unsigned int h; //height
	double g;
	figure_t**	tab;
	unsigned	size;
}image_t;



#endif
