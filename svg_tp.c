#include "svg.h"


void print_coord(coord_t c){
   printf("(%f,%f)",c.x,c.y);
   prl;
}

coord_t coordinate(double x, double y){
    coord_t coord;
    coord.x = x;
    coord.y = y;
    return coord;
}

coord_t inv_translate_point(coord_t p, coord_t v){
    coord_t nouv;
    nouv.x = p.x - v.x;
    nouv.y = p.y - v.y;
    return nouv;
}

coord_t inv_rotate_point(coord_t p, double t){
    coord_t nouv;
    nouv.x = p.x * cos(-t) - p.y * sin(-t);
    nouv.y = p.x * sin(-t) + p.y * cos(-t);
    return nouv;
}

figure_t circle(double radius){
    figure_t figure;
    figure.color = strdup("+");
    figure.centre.x = 0;
    figure.centre.y = 0;
    figure.angle = 0;
    figure.type = CERCLE;
    figure.figure.cercle = radius * radius;
    return figure;
}

figure_t rectangle(double width, double height){
    figure_t figure;
    figure.color = strdup("+");
    figure.centre.x = 0;
    figure.centre.y = 0;
    figure.angle = 0;
    figure.type = RECTANGLE;
    figure.figure.rectangle.w = width;
    figure.figure.rectangle.h = height;
    return figure;
}

figure_t line(double length){
    figure_t figure;
    figure.color = strdup("+");
    figure.centre.x = 0;
    figure.centre.y = 0;
    figure.angle = 0;
    figure.type = LIGNE;
    figure.figure.length = length;
    return figure;
}

figure_t color(figure_t f, color_t c){
    f.color = strdup(c);
    return f;
}

figure_t translate(figure_t f, double dx, double dy){
    f.centre.x += dx;
    f.centre.y += dy;
    return f;
}

figure_t rotate(figure_t f, double dt){
    f.angle += dt;
    return f;
}
