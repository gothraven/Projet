#include "pdf.h"


void pdf_object_free(pdf_object_t* o){

}

pdf_object_t* pdf_null(pdf_object_t* o){
  if(o==NULL){
    o = (pdf_object_t*)malloc(1 * sizeof(pdf_object_t));
  } else {
    //
  }
}