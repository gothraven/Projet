#include "pdf.h"

void pdf_object_free(pdf_object_t* o){
  if(o == NULL)
    exit(EXIT_FAILURE);
  if(o->type == string)
      free(o->string);
  else if(o->type == nom)
    free(o->nom);
  else if(o->type == stream)
    pdf_stream_free(o);
  else if(o->type == tab)
    pdf_tab_free(o);
  else if(o->type == dict)
    pdf_dict_free(o);
}

void pdf_stream_free(pdf_object_t* o){
  if(o != NULL && o->type == stream){
    pdf_dict_free(o->stream->dict);
    free(o->stream->valeur);
  }
}

void pdf_dict_free(pdf_object_t* o){
  if(o != NULL && o->type == dict){
    pdf_dict_t* tet = o->dict;
    while(tet != NULL){
      free(tet->cle);
      pdf_object_free(tet->valeur);
      tet = tet->next;
    }
  }
}

void pdf_tab_free(pdf_object_t* o){
  if(o != NULL && o->type == tab){
    for(int i = 0; i < o->tab->size; i++){
      free(o->tab->tab[i]);
    }
  }
}

pdf_object_t* pdf_null(pdf_object_t* o){
  if(o==NULL)
    o = (pdf_object_t*)malloc(1 * sizeof(pdf_object_t));
  else
    pdf_object_free(o);
  o->type = null;
  return o;
}

pdf_object_t* pdf_bool(pdf_object_t* o, bool b){
  if(o==NULL)
    o = (pdf_object_t*)malloc(1 * sizeof(pdf_object_t));
  else
    pdf_object_free(o);
  o->type = boolean;
  o->boolean = b;
  return o;
}

pdf_object_t* pdf_int(pdf_object_t* o, int i){
  if(o==NULL)
    o = (pdf_object_t*)malloc(1 * sizeof(pdf_object_t));
  else
    pdf_object_free(o);
  o->type = entier;
  o->entier = i;
  return o;
}

pdf_object_t* pdf_real(pdf_object_t* o, float f){
  if(o==NULL)
    o = (pdf_object_t*)malloc(1 * sizeof(pdf_object_t));
  else
    pdf_object_free(o);
  o->type = reel;
  o->reel = f;
  return o;
}

pdf_object_t* pdf_reference(pdf_object_t* o, unsigned int id){
  if(o==NULL)
    o = (pdf_object_t*)malloc(1 * sizeof(pdf_object_t));
  else
    pdf_object_free(o);
  o->type = ref;
  o->ref = id;
  return o;
}

pdf_object_t* pdf_name(pdf_object_t* o, const char* n){
  if(o==NULL)
    o = (pdf_object_t*)malloc(1 * sizeof(pdf_object_t));
  else
    pdf_object_free(o);
  o->type = nom;
  o->nom = strdup(n);
  return o;
}

pdf_object_t* pdf_string(pdf_object_t* o, const char* s){
  if(o==NULL)
    o = (pdf_object_t*)malloc(1 * sizeof(pdf_object_t));
  else
    pdf_object_free(o);
  o->type = string;
  o->string = strdup(s);
  return o;
}

pdf_object_t* pdf_stream(pdf_object_t* o, size_t sz, void* data){
  if(o==NULL)
    o = (pdf_object_t*)malloc(1 * sizeof(pdf_object_t));
  else
    pdf_object_free(o);
  o->type = stream;
  o->stream = (pdf_stream_t*)malloc(1 * sizeof(pdf_stream_t));
  pdf_object_t* dict = NULL;
  o->stream->dict = pdf_dictionary(dict);
  pdf_int(pdf_dict_get(o->stream->dict, "Lenght"), sz);
  o->stream->valeur = data;
  o->stream->size = sz;
  return o;
}

pdf_object_t* pdf_array(pdf_object_t* o, size_t n){
  if(o==NULL)
    o = (pdf_object_t*)malloc(1 * sizeof(pdf_object_t));
  else
    pdf_object_free(o);
  o->type = tab;
  o->tab = (pdf_tab_t*)malloc(1 * sizeof(pdf_tab_t));
  o->tab->tab = (pdf_object_t**)malloc(n * sizeof(pdf_object_t*));
  for (int i = 0; i < n; ++i)
    o->tab->tab[i] = pdf_null(NULL);
  o->tab->size = n;
  return o;
}

pdf_object_t* pdf_dictionary(pdf_object_t* o){
  if(o==NULL)
    o = (pdf_object_t*)malloc(1 * sizeof(pdf_object_t));
  else
    pdf_object_free(o);
  o->type = dict;
  o->dict = NULL;
  return o;
}

pdf_object_t* pdf_stream_from_file(pdf_object_t* o, const char* fname){
  FILE* file;
  file = fopen(fname, "r");
  if (file == NULL)
    exit(EXIT_FAILURE);
  fseek(file, 0, SEEK_END); //va a la fin
  size_t taille;
  taille = ftell(file); //voir la taille parcouru
  fseek(file, 0, SEEK_SET); //revenir au debut
  char* stream;
  stream = (char*)malloc(taille * sizeof(char));
  fread(stream, taille, sizeof(char), file);
  o = pdf_stream(o, taille, stream);
  return o;
}

pdf_object_t* pdf_stream_from_string(pdf_object_t* o, const char* str){
  size_t taille = strlen(str);
  char* copie = strdup(str);
  return pdf_stream(o, taille, copie);
}

pdf_object_t* pdf_array_get(pdf_object_t* a, size_t i){
  if(a == NULL)
    exit(EXIT_FAILURE);
  if (a->type != tab)
    exit(EXIT_FAILURE);
  pdf_object_t** t = a->tab->tab;
  if (i < 0 || i >= a->tab->size)
    exit(EXIT_FAILURE);
  return a->tab->tab[i];
}

pdf_object_t* pdf_dict_get(pdf_object_t* d, const char* cle){
  if(d == NULL || d->type != dict)
    exit(EXIT_FAILURE);
  pdf_dict_t* tet = d->dict;
  if (tet == NULL){ //dict vide
    d->dict = (pdf_dict_t*)malloc(1 * sizeof(pdf_dict_t));
    d->dict->cle = strdup(cle);
    d->dict->valeur = NULL;
    d->dict->valeur = pdf_null(d->dict->valeur);
    d->dict->next = NULL;
    return d->dict->valeur;
  }else{
    while(tet != NULL){ //parcour la list
      if(strcmp(tet->cle, cle) == 0) //meme cle
        return tet->valeur;
      if(tet->next == NULL){
        tet->next = (pdf_dict_t*)malloc(1 * sizeof(pdf_dict_t));
        tet->next->cle = strdup(cle);
        tet->next->valeur = NULL;
        tet->next->valeur = pdf_null(tet->next->valeur);
        tet->next->next = NULL;
        return tet->next->valeur;
      }
      tet = tet->next;
    }
    //prblem ici
    exit(EXIT_FAILURE);
  }
}

size_t pdf_fprint_object(FILE* fd, pdf_object_t* o){
  if(o == NULL)
    exit(EXIT_FAILURE);
  if(o->type == null){
    return fprintf(fd, "null");
  }else if(o->type == boolean){
    if(o->boolean){
      return fprintf(fd, "true");
    }else{
      return fprintf(fd, "false");
    }
  }else if(o->type == entier){
    return fprintf(fd, "%d", o->entier);
  }else if(o->type == reel){
    return fprintf(fd, "%f", o->reel);
  }else if(o->type == string){
    return fprintf(fd, "(%s)", o->string);
  }else if(o->type == nom){
    return fprintf(fd, "/%s", o->nom);
  }else if(o->type == ref){
    return fprintf(fd, "%d 0 R\n", o->ref);
  }else if(o->type ==  stream){
    return pdf_stream_fprint(fd, o->stream);
  }else if(o->type ==  tab){
    return pdf_tab_fprint(fd, o->tab);
  }else if(o->type ==  dict){
    return pdf_dict_fprint(fd, o->dict);
  }
}

size_t  pdf_stream_fprint(FILE* fd, pdf_stream_t* stream){
  size_t nb = 0;
  nb += pdf_fprint_object(fd, stream->dict);
  nb += fprintf(fd, "stream\n");
  nb += fwrite(stream->valeur, stream->size, sizeof(char), fd);
  nb += fprintf(fd, "endstream");
  return nb;
}

size_t  pdf_tab_fprint(FILE* fd, pdf_tab_t* tab){
  size_t nb = 0;
  nb += fputc('[', fd);
  for(int i = 0; i < tab->size; i++){ //imprimer toutes les objets du tableau
    nb += pdf_fprint_object(fd, tab->tab[i]); //verifier en cas la case est null
    if(i < tab->size-1)
      nb += fputc(' ', fd);
  }
  nb += fputc(']', fd);
  return nb;
}

size_t  pdf_dict_fprint(FILE* fd, pdf_dict_t* dict){
  size_t nb = 0;
  nb += fprintf(fd, "<<\n");
  if (dict != NULL){
   pdf_dict_t* tet = dict;
   while(tet != NULL){
    nb += fprintf(fd, "\t/%s ",tet->cle);
    nb += pdf_fprint_object(fd, tet->valeur);
    nb += fprintf(fd, "\n");
    tet = tet->next;
   }
  }
  nb += fprintf(fd, ">>\n");
  return nb;
}

/*#define prl { printf("\n"); }

int main() {
  pdf_object_t* o = NULL;
  o = pdf_null(o); printf("%p: ", o); pdf_fprint_object(stdout, o); prl;
  o = pdf_bool(o, true); printf("%p: ", o); pdf_fprint_object(stdout, o); prl;
  o = pdf_bool(o, false); printf("%p: ", o); pdf_fprint_object(stdout, o); prl;
  o = pdf_int(o, 123); printf("%p: ", o); pdf_fprint_object(stdout, o); prl;
  o = pdf_real(o, 123.456); printf("%p: ", o); pdf_fprint_object(stdout, o); prl;
  o = pdf_name(o, "Root"); printf("%p: ", o); pdf_fprint_object(stdout, o); prl;
  o = pdf_string(o, "Helloworld !!!!"); printf("%p: ", o); pdf_fprint_object(stdout, o); prl;
  o = pdf_reference(o, 123); printf("%p: ", o); pdf_fprint_object(stdout, o); prl;
  o = pdf_array(o, 3); printf("%p: ", o); pdf_fprint_object(stdout, o); prl;
  pdf_int(pdf_array_get(o, 0), 123);
  pdf_real(pdf_array_get(o, 1), 123.456);
  pdf_name(pdf_array_get(o, 2), "Root");
  printf("%p: ", o); pdf_fprint_object(stdout, o); prl;
  o = pdf_dictionary(o); printf("%p: ", o); pdf_fprint_object(stdout, o); prl;
  pdf_int(pdf_dict_get(o, "Key1"), 123);
  pdf_real(pdf_dict_get(o, "Key2"), 123.456);
  pdf_name(pdf_dict_get(o, "Key3"), "Root");
  printf("%p: ", o); pdf_fprint_object(stdout, o); prl;
  o = pdf_stream_from_string(o, "q 2 0 0 2 0 0 cm Q"); printf("%p: ", o); pdf_fprint_object(stdout, o); prl;
  pdf_object_free(o);
  free(o);

  return EXIT_SUCCESS;
}*/

pdf_object_t* pdf_default_fonts(pdf_object_t* f){
  f = pdf_dictionary(f);
  pdf_object_t* font;

  font = pdf_dictionary(pdf_dict_get(f, "FT"));
  pdf_name(pdf_dict_get(font, "Type"), "Font");
  pdf_name(pdf_dict_get(font, "Subtype"), "Type1");
  pdf_name(pdf_dict_get(font, "BaseFont"), "Times-Roman");

  font = pdf_dictionary(pdf_dict_get(f, "FTB"));
  pdf_name(pdf_dict_get(font, "Type"), "Font");
  pdf_name(pdf_dict_get(font, "Subtype"), "Type1");
  pdf_name(pdf_dict_get(font, "BaseFont"), "Times-Bold");

  font = pdf_dictionary(pdf_dict_get(f, "FTI"));
  pdf_name(pdf_dict_get(font, "Type"), "Font");
  pdf_name(pdf_dict_get(font, "Subtype"), "Type1");
  pdf_name(pdf_dict_get(font, "BaseFont"), "Times-Italic");

  font = pdf_dictionary(pdf_dict_get(f, "FTBI"));
  pdf_name(pdf_dict_get(font, "Type"), "Font");
  pdf_name(pdf_dict_get(font, "Subtype"), "Type1");
  pdf_name(pdf_dict_get(font, "BaseFont"), "Times-BoldItalic");

  font = pdf_dictionary(pdf_dict_get(f, "FV"));
  pdf_name(pdf_dict_get(font, "Type"), "Font");
  pdf_name(pdf_dict_get(font, "Subtype"), "Type1");
  pdf_name(pdf_dict_get(font, "BaseFont"), "Helvetica");

  font = pdf_dictionary(pdf_dict_get(f, "FVB"));
  pdf_name(pdf_dict_get(font, "Type"), "Font");
  pdf_name(pdf_dict_get(font, "Subtype"), "Type1");
  pdf_name(pdf_dict_get(font, "BaseFont"), "Helvetica-Bold");

  font = pdf_dictionary(pdf_dict_get(f, "FVI"));
  pdf_name(pdf_dict_get(font, "Type"), "Font");
  pdf_name(pdf_dict_get(font, "Subtype"), "Type1");
  pdf_name(pdf_dict_get(font, "BaseFont"), "Helvetica-Oblique");

  font = pdf_dictionary(pdf_dict_get(f, "FVBI"));
  pdf_name(pdf_dict_get(font, "Type"), "Font");
  pdf_name(pdf_dict_get(font, "Subtype"), "Type1");
  pdf_name(pdf_dict_get(font, "BaseFont"), "Helvetica-BoldOblique");

  return f;
}

bool jpeg_info(const void* i, size_t* width, size_t* height, size_t* bpc, const char** cs){
  const unsigned char* im = (const unsigned char*)i;
  if (im[0]!=0xff || im[1]!=0xd8) return false;
  im += 2;

  while(im[0] == 0xff) {
    unsigned char frame_id = im[1];
    if (frame_id == 0xc0) break;
    size_t frame_length = 256 * im[2] + im[3];
    im += frame_length + 2;
  }
  size_t h = im[5] * 256 + im[6];
  size_t w = im[7] * 256 + im[8];
  size_t b = im[4];
  const char* c;
  switch(im[9]) {
    case 1: c = "DeviceGray"; break;
    case 3: c = "DeviceRGB"; break;
    case 4: c = "DeviceCMYK"; break;
    default: return false;
  }
  if (width) *width = w;
  if (height) *height = h;
  if (bpc) *bpc = b;
  if (cs) *cs = c;
  return true;
}

pdf_t* pdf_create(unsigned int nb, float w, float h){
  pdf_t* pdf = (pdf_t*)malloc(1 * sizeof(pdf_t));
  pdf->w = w;
  pdf->h = h;
  unsigned int nb_objs = nb + 4; //4 = nombre d'objets de base
  pdf->taille = nb_objs;
  pdf->tab_objs = (pdf_object_t**)malloc(nb_objs * sizeof(pdf_object_t*));
  pdf_object_t* catalog = NULL; //cree le catalog
  catalog = pdf_dictionary(catalog);
  pdf_name(pdf_dict_get(catalog, "Type"), "Catalog"); //ajouter les cle valeur
  pdf_reference(pdf_dict_get(catalog, "Pages"), 3); //3 = indice de pages
  pdf->tab_objs[0] = catalog;  //catalog indice 0 mais 1 sur le pdf
  pdf_object_t* pages = NULL; //cree le neoud pasges
  pages = pdf_dictionary(pages);
  pdf_name(pdf_dict_get(pages, "Type"), "Pages");
  pdf_int(pdf_dict_get(pages, "Count"), nb);
  pdf_array(pdf_dict_get(pages, "Kids"), nb);
  unsigned int i = 0;
  while (i < nb){
    //ajouter les reference pardefaut des neoud pages
    pdf_reference(pdf_array_get(pdf_dict_get(pages, "Kids"), i),i + 4 + 1);
    i++;
  }
  pdf->tab_objs[2] = pages; //noeud pages est d'indice 2 dans le tab mais 3 dans pdf
  pdf_object_t* fonts = NULL; //cree noeud fonts
  fonts = pdf_default_fonts(fonts); //utiliser la fonction du prof
  pdf->tab_objs[1] = fonts; //fonts indice 0 mais 1 sur le pdf
  pdf_object_t* xobject = NULL; //cree le noeud des xobject
  xobject = pdf_dictionary(xobject);
  pdf->tab_objs[3] = xobject; //les xobject dans le pdf est a 4 mais dans le tab est a 3
  i = 0;
  while (i < nb){
    //cree toutes les noeud pages demander un par un
    pdf_object_t* page = NULL;
    page = pdf_dictionary(page);
    pdf_name(pdf_dict_get(page, "Type"), "Page");
    pdf_reference(pdf_dict_get(page, "Parent"), 3); //3 = indice de pages
    pdf_object_t* mediaBox = pdf_array(pdf_dict_get(page, "MediaBox"), 4);
    pdf_int(pdf_array_get(mediaBox, 0), 0);
    pdf_int(pdf_array_get(mediaBox, 1), 0);
    pdf_real(pdf_array_get(mediaBox, 2), w);
    pdf_real(pdf_array_get(mediaBox, 3), h);
    pdf_object_t* resources = pdf_dictionary(pdf_dict_get(page, "Resources"));
    pdf_reference(pdf_dict_get(resources, "Font"), 2); //2 = indice de fonts
    pdf_reference(pdf_dict_get(resources, "XObject"), 4); //4 = indice de xobject
    pdf_null(pdf_dict_get(page, "Contents"));
    pdf->tab_objs[3 + i + 1] = page; //indice de xobject + l'indice de page (i + 1)
    i++;
  }
  return pdf;
}

void pdf_delete(pdf_t* p){
  if(p == NULL)
    return;
  unsigned int i = 0;
  while(i < p->taille){
    pdf_object_free(p->tab_objs[i]);
    free(p->tab_objs[i]);
    i++;
  }
  free(p);
  p = NULL;
}

size_t pdf_new_id(pdf_t* p){
  p->taille++;
  p->tab_objs = (pdf_object_t**)realloc(p->tab_objs, p->taille * sizeof(pdf_object_t*));
  size_t indice = p->taille - 1;
  pdf_null(p->tab_objs[indice]);
  return indice;
}

void pdf_save(const char* fname, pdf_t* p){
  FILE* fd = fopen(fname, "wb+");
  if(fd == NULL)
    exit(EXIT_FAILURE);
  //faut compter le nombre de char ecrit dans le fichier
  size_t nb = 0;
  //pdf header
  fprintf(fd, "%%PDF-1.7\n%%");
  unsigned int x = 0xE2E3CFD3;
  fwrite(&x, sizeof(unsigned int), 1, fd);
  fputc('\n',fd);
  nb = 15; //catalog commence en octet 15
  //imprime toutes les objets de pdf
  unsigned int i = 0;
  unsigned int xref[p->taille];
  while(i < p->taille){
    xref[i] = nb;
    nb += fprintf(fd, "%d 0 obj\n", i + 1);
    nb += pdf_fprint_object(fd, p->tab_objs[i]);
    nb += fprintf(fd, "endobj\n");
    i++;
  }
  //imprimer xref
  size_t debut_xref = nb;
  fprintf(fd, "xref\n0 %d\n", p->taille);
  fprintf(fd, "0000000000 65535 f\n");
  i = 0;
  while(i < p->taille){
    fprintf(fd, "%10d 00000 n\n", xref[i]);
    i++;
  }
  fprintf(fd, "trailer\n");
  pdf_object_t* trailer = NULL;
  trailer = pdf_dictionary(trailer);
  pdf_reference(pdf_dict_get(trailer, "Root"), 1); //induce de catalog
  pdf_int(pdf_dict_get(trailer, "Size"), p->taille + 1); //taille de xref
  pdf_fprint_object(fd, trailer);
  fprintf(fd, "startxref\n%lu\n", debut_xref);
  //pdf footer
  fprintf(fd, "%%EOF\n");
}

/*int main(int argc, char const *argv[])
{
  pdf_t* p = pdf_create(1, 1300, 520);
  pdf_save("testing.pdf",p);
  pdf_delete(p);
  return 0;
}*/

void pdf_set_content(pdf_t* p, unsigned int n, const char* instr){
  size_t id = pdf_new_id(pdf);
  //pdf_object_t* ref_content = NULL;
  //4 = neoud de base, n = 0 1er page, n = 1 2eme page ...etc
  /*ref_content = pdf_dict_get(p->tab_objs[4 + n], "Contents")
  ref_content = pdf_reference(ref_content, id + 1);*/
  pdf_reference(pdf_dict_get(p->tab_objs[4 + n], "Contents"), id+1);
  /*pdf_object_t* stream = NULL;
  stream = pdf_stream_from_string(stream, instr);
  p->tab_objs[id] = steam*/
  p->tab_objs[id] = pdf_stream_from_string(p->tab_objs[id], instr);
}

const char* pdf_load_image(pdf_t* p, const char* fname){
  FILE* im = fopen(fname, "r");
  if(im == NULL)
    exit(EXIT_FAILURE);

}

int main(int argc, char const *argv[])
{
  return 0;
}
