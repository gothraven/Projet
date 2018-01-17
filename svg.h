
#ifndef SVG_H
#define SVG_H

#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<math.h>

#define prl { printf("\n"); }

typedef char* color_t;

typedef struct coord_s
{
    double x;
    double y;
}   coord_t;

typedef enum type_figure_s{
    RIEN,
    CERCLE,
    RECTANGLE,
    LIGNE,
    PHOTO,
    GROUPE
}   figure_e;

typedef struct circle_s{
    double r;
}   circle_t;

typedef struct rectangle_s{
    double w;// demie longueur
    double h;// demie hauteur
}   rectangle_t;

typedef struct line_s{
     double longueur,length;// demie longueur
}    line_t;

typedef struct photo_s{
    char     *name;

}   photo_t;

typedef struct figure_s figure_t;

typedef struct groupe_s{
    struct figure_s val;
    struct groupe_s next;
}   groupe_t;


typedef union type_s{
    circle_t    cercle;
    rectangle_t rectangle;
    line_t      line;
    groupe_t    groupe;
	photo_t     photo;
}               type_u;

struct	figure_s{
    color_t     color;
    coord_t     centre;
    double      angle;
    figure_e    type;
    type_u      figure;
};
/*
typedef struct	image_s
{
	unsigned int longueur;	//width
	unsigned int hauteur;	//height
	double		granularite;
	figure_t	tab[10];
	unsigned	nombre_fig;
}	image_t;
*/


#endif
