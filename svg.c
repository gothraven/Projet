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
    figure.color.r = 0;
    figure.color.g = 0;
    figure.color.b = 0;
    figure.color.c = '+';
    figure.centre.x = 0;
    figure.centre.y = 0;
    figure.angle = 0;
    figure.scale = 1;
    figure.type = CERCLE;
    figure.cercle.r = radius * radius;
    return figure;
}

figure_t rectangle(double width, double height){
    figure_t figure;
    figure.color.r = 0;
    figure.color.g = 0;
    figure.color.b = 0;
    figure.color.c = '+';
    figure.centre.x = 0;
    figure.centre.y = 0;
    figure.angle = 0;
    figure.scale = 1;
    figure.type = RECTANGLE;
    figure.rectangle.w = width / 2;
    figure.rectangle.h = height / 2;
    return figure;
}

figure_t line(double length){
    figure_t figure;
    figure.color.r = 0;
    figure.color.g = 0;
    figure.color.b = 0;
    figure.color.c = '+';
    figure.centre.x = 0;
    figure.centre.y = 0;
    figure.angle = 0;
    figure.scale = 1;
    figure.type = LIGNE;
    figure.line.l = length / 2;
    return figure;
}

figure_t text(char* string, char* font, unsigned int fontsize){
    figure_t figure;
    figure.color.r = 0;
    figure.color.g = 0;
    figure.color.b = 0;
    figure.color.c = '+';
    figure.centre.x = 0;
    figure.centre.y = 0;
    figure.angle = 0;
    figure.scale = 1;
    figure.type = TEXTE;
    figure.text.fontsize = fontsize;
    return figure;
}

figure_t photo(char* fname, unsigned int w, unsigned int h){
    figure_t figure;
    figure.color.r = 0;
    figure.color.g = 0;
    figure.color.b = 0;
    figure.color.c = '+';
    figure.centre.x = 0;
    figure.centre.y = 0;
    figure.angle = 0;
    figure.scale = 1;
    figure.type = PHOTO;
    figure.photo.w = w;
    figure.photo.h = h;
    return figure;
}

figure_t color(figure_t f, char c){
    f.color.c = c;
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

char intersect(coord_t p, figure_t f, double grain){
    p = inv_translate_point(p, f.centre);
	p = inv_rotate_point(p, f.angle);
	if(f.type == CERCLE){
		if(pow(p.x,2) + pow(p.y,2) <= f.cercle.r )
			return f.color.c;
	}else if(f.type == RECTANGLE){
		if(fabs(p.x) <= f.rectangle.w && fabs(p.y) <= f.rectangle.h)
			return f.color.c;
	}else if(f.type == LIGNE){
		if(fabs(p.x) <= f.line.l && fabs(p.y) <= grain)
			return f.color.c;
	}
	return 0;
}

image_t create_image(unsigned int width, unsigned int height, double grain){
  image_t image;
  image.w = width;
  image.h = height;
  image.g = grain;
  image.size = 0;
  return image;
}

image_t append(image_t img, figure_t f){
  if(img.size==0){
    img.size++;
    img.tab = (figure_t*)malloc(1 * sizeof(figure_t));
    img.tab[0] = f;
  }else{
    img.size++;
    img.tab = (figure_t*)realloc(img.tab, img.size * sizeof(figure_t));
    img.tab[img.size-1] = f;
  }
  return img;
}

void paint(image_t img) {
  int i;
  for (double x = 0; x < img.height; x+=img.grain) {
    for (double y = 0; y < img.width; y +=(img.grain*2)){
      i = 0;
      while (i < img.cpt){
        if((img.tab[i].type < TEXTE) && intersect(coordinate(y, x), img.tab[i], img.grain))
          break;
        i++;
      }
      if (i == img.cpt)
        printf(" ");
      else
        printf("%c",img.tab[i].color);
    }
    printf("\n");
  }
}

void get_transforms(xattribute_t* att,double *tx,double *ty,double *r, double *scale){
  char* transform = get_attribute_value(att,"transform");
  if(transform!=NULL){
      *tx = 0;
      *ty = 0;
      *r = 0;
      *scale = 0;
  }else{
    *tx = 0;
    *ty = 0;
    *r = 0;
    *scale = 0;
  }
}
