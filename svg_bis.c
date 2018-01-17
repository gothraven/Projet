#include"svg.h"

svg_t xml_to_svg_(svg_t s,xelement_t *e, double tx, double ty, double rotate, double scale){
  if(e == NULL)
    return s;
  if(strcmp(e->nom,"g"))
  {
    double x = 0;
    double y = 0;
    double r = 0;
    double sc = 0;
    try_to_translate(e->attributs,&x,&y);
    try_to_rotate(e->attributs,&r);
    try_to_scale(e->attributs,&sc);
    return xml_to_svg_(s,e->contenu->fils,tx + x, ty + y, rotate + r, scale + sc);
  }
  if(strcmp(e->nom,"rect")){
    s= rectangle_in_svg(s,e->attributs);
  }else if(strcmp(e->nom,"circle")){
    s= cercle_in_svg(s,e->attributs);
  }else if(strcmp(e->nom,"line")){
    s= line_in_svg(s,e->attributs);
  }else if(strcmp(e->nom,"text")){
    s= text_in_svg(s,e->attributs);
  }else if(strcmp(e->nom,"image")){
    s= cercle_in_svg(s,e->attributs);
  }else{
    exit(0);
  }
  return xml_to_svg_(s,e->frere,tx,ty,rotate,scale);
}

svg_t xml_to_svg(xelement_t *e){
  svg_t s;
  s.e = e;
  unsigned w = 1300;
  unsigned h = 520;
  char* tmp;
  if(e != NULL && strcmp(e->nom,"xmlns") == 0){
    tmp = get_attribute_value(e->attributs,"width");
    if(tmp != NULL)
      w = atoi(tmp);
    tmp = get_attribute_value(e->attributs,"height");
      if(tmp != NULL)
        h = atoi(tmp);
  }else
    printf("Attention ce n'est sans doute pas un svg\n");
  s.im = image(w,h,1);
  return xml_to_svg_(s,e,0,0,0,0);
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
