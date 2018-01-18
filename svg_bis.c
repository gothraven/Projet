#include"svg.h"


figure_t  add_transforms(figure_t f,double x, double y, double r, double sc){
  f.centre.x += x;
  f.centre.y += y;
  f.angle += r;
  if(sc != 0)
    f.scale *= sc;
  printf("%f--%f--\n",sc,f.scale);
  return f;
}


double distance(double x1,double y1,double x2,double y2){
  double dist = pow(x2 - x1,2) + pow(y2 - y1,2);
  dist = sqrt(dist);
	return dist;
}

figure_t  get_color(figure_t f, char *col){
	if (col != NULL)
		f.color.c = col[0];
	f.color.r = 1;
	f.color.g = 1;
	f.color.b = 1;
	return f;
}

svg_t line_in_svg(svg_t s,xattribute_t* a){printf("line_in_svg\n");
  figure_t f;
  char* tmp = NULL;
  double x = 0;
  double y = 0;
  double x2 = 0;
  double y2 = 0;
  double r = 0;
  double sc = 0;
  double g = 0;

  super_double(a, "x1", &x);
  super_double(a, "y1", &y);
  super_double(a, "x2", &x2);
  super_double(a, "y2", &y2);
  double l = distance(x,y,x2,y2);
  f = line(l); // creer la ligne
  f.centre.x = (x + x2)/2;
  f.centre.y = (y + y2)/2;// deplace le centre

  x = 0;
  y = 0;
  tmp = get_attribute_value(a,"transform");
  if(tmp != NULL){// fait toute les transformation
    get_transforms(a,&x,&y,&r,&sc);
   f =  add_transforms(f,x,y,r,sc);
  }
  super_double(a,"stroke-width",&g);
  if (g != s.im.g && g != 0){
	  printf("grain a etait modifier\n");
	  s.im.g = g;
  }
	// manque plus aue couleur
  tmp = get_attribute_value(a,"stroke");
  f = get_color(f,tmp);
  s.im = append(s.im,f);
  return s;
}

svg_t text_in_svg(svg_t s, xattribute_t *a,char *string){
  figure_t f;
  char* tmp = NULL;
  char* font = NULL;
  double x = 0;
  double y = 0;
  double r = 0;
  double size = 0;
  double sc = 0;

  font = get_attribute_value(a, "font-family");
  super_double(a,"font-size",&size);
  if(size <= 0)
    printf("probleme de taille\n");
  f = text(string,font,size); // creer le texte

  super_double(a, "x", &x);
  super_double(a, "y", &y);
  f.centre.x = x;
  f.centre.y = y;// deplace le centre

  x = 0;
  y = 0;
  tmp = get_attribute_value(a,"transform");
  if(tmp != NULL){// fait toute les transformation SI IL Y'EN A
    get_transforms(a,&x,&y,&r,&sc);
   f =  add_transforms(f,x,y,r,sc);
  }
	// manque plus aue couleur
  tmp = get_attribute_value(a,"fill");
  f = get_color(f,tmp);
  s.im = append(s.im,f);
	printf("text_in_svg\n");
  return s;
}


svg_t rectangle_in_svg(svg_t s,xattribute_t* a){printf("rectangle_in_svg\n");
  figure_t f;
  char* tmp = NULL;
  double x = 0;
  double y = 0;
  double w = 0;
  double h = 0;
  double r = 0;
  double sc = 0;

  super_double(a, "whidth", &w);
  super_double(a, "height", &h);
  f = rectangle(w,h); // creer le rectangle en 0,0

  super_double(a, "x", &x);
  super_double(a, "y", &y);
  f.centre.x += x;// deplace le centre
  f.centre.y += y;

  x = 0;
  y = 0;
  tmp = get_attribute_value(a,"transform");
  if(tmp != NULL){// fait toute les transformation
    get_transforms(a,&x,&y,&r,&sc);
   f =  add_transforms(f,x,y,r,sc);
  }
		/* manque plus aue couleur */
  tmp = get_attribute_value(a,"stroke");
  f = get_color(f,tmp);
  s.im = append(s.im,f);
  return s;
}

svg_t cercle_in_svg(svg_t s, xattribute_t* a){printf("cercle_in_svg\n");
  figure_t f;
  char* tmp = NULL;
  double x = 0;
  double y = 0;
  double r = 0;
  double sc = 0;

  super_double(a, "r", &r);
  f = circle(r); // creer le rectangle en 0,0

  super_double(a, "cx", &x);
  super_double(a, "cy", &y);
  f.centre.x += x;// deplace le centre
  f.centre.y += y;

  x = 0;
  y = 0;
  r = 0;
  tmp = get_attribute_value(a,"transform");
  if(tmp != NULL){// fait toute les transformation
    get_transforms(a,&x,&y,&r,&sc);
   f =  add_transforms(f,x,y,r,sc);
  }
		/* manque plus aue couleur*/
  tmp = get_attribute_value(a,"stroke");
  f = get_color(f,tmp);
  s.im = append(s.im,f);
  return s;
}


svg_t xml_to_svg_(svg_t s,xelement_t *e, double tx, double ty, double rotate, double scale){
	if(e == NULL)
    return s;
	printf("voici %s\n",e->nom);
  if(strcmp(e->nom,"g") == 0)
  {
    double x = 0;
    double y = 0;
    double r = 0;
    double sc = 0;
	printf("x %f y %f r %f, sc %f\n",x,y,r,sc);
    get_transforms(e->attributs,&x,&y,&r,&sc);
	printf("x %f y %f r %f, sc %f\n",x,y,r,sc);
    s =xml_to_svg_(s,e->contenu->fils,tx + x, ty + y, rotate + r, scale * sc);
  }
  else if(strcmp(e->nom,"rect") == 0){
    s= rectangle_in_svg(s,e->attributs);
   s.im.tab[s.im.size - 1] = add_transforms(s.im.tab[s.im.size - 1],tx,ty,rotate,scale);
  }else if(strcmp(e->nom,"circle") == 0){
    s= cercle_in_svg(s,e->attributs);
    s.im.tab[s.im.size - 1] = add_transforms(s.im.tab[s.im.size - 1],tx,ty,rotate,scale);
  }else if(strcmp(e->nom,"line") == 0){
    s= line_in_svg(s,e->attributs);
    s.im.tab[s.im.size - 1] = add_transforms(s.im.tab[s.im.size - 1],tx,ty,rotate,scale);
  }else if(strcmp(e->nom,"text") == 0){
    if(e->type == RAW){
      s= text_in_svg(s,e->attributs,e->contenu->raw);
    s.im.tab[s.im.size - 1] = add_transforms(s.im.tab[s.im.size - 1],tx,ty,rotate,scale);
    }else{
      printf("RAW\n");
      exit(0);}
  }
  else{
    printf("nom %s\n",e->nom);
  }
printf("wetfgh\n");
  return xml_to_svg_(s,e->frere,tx,ty,rotate,scale);
}

svg_t xml_to_svg(xelement_t *e){
  svg_t s;
  s.e = e;
  unsigned w = 1300;
  unsigned h = 520;
  char* tmp;
  if(e->attributs != NULL && get_attribute_value(e->attributs,"xmlns") != NULL){
    tmp = get_attribute_value(e->attributs,"width");
    if(tmp != NULL)
      w = atoi(tmp);
    tmp = get_attribute_value(e->attributs,"height");
      if(tmp != NULL)
        h = atoi(tmp);
  }else
    printf("Attention ce n'est sans doute pas un svg%s\n",e->nom);
  s.im = image(w,h,1);
  printf("/**************************************/\n");
  return xml_to_svg_(s,e->contenu->fils,0,0,0,1);
}

int  super_double(xattribute_t *a,char* str,double *x){
  char* tmp = get_attribute_value(a,str);
  if(tmp != NULL)
  {
    *x = atof(tmp);
    return 1;
  }
  else
    x = 0;
  return 0;
}
#define pi  3.14159265359

char	*ajout_cercle_pdfbis(double tx, double ty, double cx, double cy, double r, double rg1, double rg2, double rg3)
{
	char *res;
	//              rg1 rg2 rg3    cx cy      tx ty    r      r
	asprintf(&res," q %f %f %f rg 1 %f %f 1 %f %f cm %f 0 0 %f 0 0 cm"
			" -1 0 m -1 0.553 -0.553 1 0 1 c 0.553 1 1 0.553 1 0 c 1 -0.553 0.553"
			" -1 0 -1 c -0.553 -1 -1 -0.553 -1 0 c F Q",rg1,rg2,rg3,cx,cy,tx,ty,r,r);
	printf("%s\n",res);
	return res;
}

char	*ajout_rectangle_pdfbis(double tx, double ty, double cx, double cy, double ro, double rg1, double rg2, double rg3,double w,double h)
{
	char *res;
	//              rg1 rg2 rg3    cx cy      tx ty    r      r
	asprintf(&res," q %f %f %f rg 1 0 0 1 %f %f cm %f %f %f %f 0 0 cm %f %f %f %f re F Q"
			,rg1,rg2,rg3,tx,ty,  cos(ro*pi/180), cos(ro*pi/180), sin(-1 * ro * pi / 180), sin(ro*pi/180),
			cx,cy,w,h);
	printf("%s\n",res);
	return res;
}

char	*ajout_line_pdfbis(double tx, double ty, double x1, double y1,double x2,double y2, double ro, double rg1, double rg2, double rg3,double gr)
{
	char *res;
	//                 rg1 rg2 rg3          tx ty    r      r
	asprintf(&res,"q %f w %f %f %f RG 1 0 0 1 %f %f cm %f %f %f %f 0 0 cm %f %f m %f %f l S Q"
			,gr,rg1,rg2,rg3,tx,ty,  cos(ro*pi/180), sin(ro*pi/180), sin(-1 * ro * pi / 180),cos(ro*pi/180),
			x1,y1,x2,y2);
	printf("%s\n",res);
	return res;
}


pdf_t*	svg_to_pdf(svg_t s){
	figure_t f;
	char* content = NULL;
	char* str = malloc(10024);
	*str = '\0';
	str = strcat(str,"q 1 0 0 -1 0 520 cm");
	for(unsigned i = 0; i < s.im.size; i++){
		f = s.im.tab[i];
		double x = f.centre.x;
		double y = f.centre.y;
		double ro = f.angle;
		if (f.type == CERCLE){
			printf("cercle\n");
			content = ajout_cercle_pdfbis(x,y,0,0,f.cercle.r,0,1,0);
		}else if (f.type == RECTANGLE){printf("rectangle\n");
			content = ajout_rectangle_pdfbis(x,y,0,0,ro,0,1,0,f.rectangle.w,f.rectangle.h);
		}else if (f.type == LIGNE){printf("line\n");
			double t = f.line.l;
			content = ajout_line_pdfbis(0,0,x + t,y,x-t,y,ro,0,1,0,10);
		}
		str = strcat(str,content);
	}
	str = strcat(str," Q ");
	pdf_t* p = pdf_create(2, s.im.w,s.im.h);
	pdf_set_content(p,1,str);
	pdf_save("oooooo.pdf",p);
	//pdf_delete(p);
	return p;

}

