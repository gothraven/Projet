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
  //pdf_int(pdf_dict_get(o->stream->dict, "Lenght"), sz);
  o->stream->valeur = data;
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
  char* stream;
  size_t taile;

  file = fopen(fname, "r");
  if (file == NULL)
    exit(EXIT_FAILURE);
  fseek(fd, 0, SEEK_END); //va a la fin
  size = ftell(fd); //voir la taille parcouru
  fseek(fd, 0, SEEK_SET); //revenir au debut
  stream =
}

int main(int argc, char const *argv[])
{
  return 0;
}
