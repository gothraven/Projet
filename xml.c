#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include "xml.h"

/*
typedef struct xattribute xattribute_t;
typedef struct xelement xelement_t;

struct xattribute {
    char* nom;
    char* valeur;
    xattribute_t* next;
};

struct xelement {
    char* nom;
    xattribute_t* attributs;
    xelement_t* pere;
    xelement_t* frere;
    union contenu {
        char* raw;
        xelement_t* fils;
    } contenu;
};
*/

xelement_t* create_xelement(const char* name){
    xelement_t* e = malloc(sizeof(xelement_t));
    e->nom = malloc(strlen(name));
    strcpy(e->nom,name);
    e->pere = NULL;
    e->frere= NULL;
	e->attributs = NULL;
    e->type = RIEN;
    e->contenu = malloc(sizeof(xelement_t));
    e->contenu->fils = NULL;
    e->contenu->raw = NULL;
    return e;
}

xattribute_t* add_xattribute(xelement_t* e, const char* name, const char* value){
    /*; si l'élément possède déjà un attribut de ce nom, la précédente valeur sera écrasée (et la mémoire libérée)*/
    if(e == NULL || name == NULL || value == NULL)
        exit(0);
    xattribute_t *tmp = e->attributs;
    while(tmp != NULL){
        if(strcmp(tmp->nom,name) == 0){
            free(tmp->valeur);
            tmp->valeur = strdup(value);
            return tmp;
        }
		tmp = tmp->next;
    }
	//printf("%s et %s",name,value);
    xattribute_t *new = malloc(sizeof(xattribute_t));
    new->nom = strdup(name);
    new->valeur = strdup(value);
    new->next = e->attributs;
    e->attributs = new;
    return new;
}

void add_sub_xelement(xelement_t* e, xelement_t* s){
    /* ajoute élément s fils de e ; verif que s n'a pas de père */
    if(s!= NULL && s->pere == NULL && (e == NULL || e->type != RAW)){ 
        s->pere = e;
		if(e == NULL)
			return;
        if(e->contenu->fils == NULL)
            e->contenu->fils = s;
        else {
			xelement_t *tmp = e->contenu->fils;
            while(tmp->frere != NULL)
                tmp = tmp->frere;
            tmp->frere = s;
        }
		e->type = LISTE;
    }
}

void add_raw(xelement_t* e, const char* r){
    if(e != NULL && e->type != LISTE){
        if(e->contenu->raw != NULL)
			free(e->contenu->raw);
		e->contenu->raw = strdup(r);
		e->type = RAW;
    }
	else
		exit(0);
}

void delete_xelement(xelement_t* e){ 
    if(e-> pere == NULL) {
        free(e);
    }
}

void save_xelement(FILE* fd, xelement_t* e)
{
    if(e != NULL){
        fputc('<', fd);
        fputs(e->nom, fd);
        if(e->attributs != NULL){
            xattribute_t *a = e->attributs;
            while(a != NULL){
                fputc(' ',fd);
                fputs(a->nom, fd);
                fputc('=', fd);
                fputc('"', fd);
                fputs(a->valeur, fd);
                fputc('"', fd);
                a = a->next;
            }
        }
        if(e->type == RIEN){
        fputs(" />\n",fd);
        } else if(e->type == RAW){
            fputc('>',fd);
            fputs(e->contenu->raw, fd);
            fputs("</", fd);
            fputs(e->nom, fd);
            fputc('>',fd);
            fputc('\n',fd);
        } else {
            fputc('>',fd);
            fputc('\n',fd);
            save_xelement(fd,e->contenu->fils);
            fputc('<',fd);
            fputc('/',fd);
            fputs(e->nom, fd);
            fputc('>',fd);
            fputc('\n',fd);
            }
		if (e->frere != NULL)
			printf("frerede %s = %s\n",e->nom, e->frere->nom);
		save_xelement(fd, e->frere);
    }
}

char next_char(FILE* fd){
    char r;
    r = fgetc(fd);
    if(r != EOF){
        while(r == ' ' || r =='\n' || r =='\r' || r =='\t'){
            r = fgetc(fd);
			if(r == EOF)
				exit(0);
        }
		return r;
    }else exit(0);

/*prochain caractère dans fd si est pas un caractère d'espacement(ni ' ', ni '\n', ni '\r', ni '\t') : le programme échouera si la fin de fichier est rencontrée*/
}

void check_next_char(FILE* fd, char c){
    char r = next_char(fd);
    if (r!= c){
        exit(0);
    }
}

bool is_next_char(FILE* fd, char c, bool cons){
    char r = next_char(fd);
    bool b = false;
    if(cons == false || r != c)
        ungetc(r,fd);
     if(r != c)
        return b;
    return true;
}

/*vérifie si le prochain caractère (next_char) est c ; dans le cas contraire ou si cons est à faux, le prochain caractère ne sera pas consommé (utiliser ungetc)*/


char* next_word(FILE* fd){
    char *w=(char*)malloc(sizeof(char) * 1000);
    int i=0;
    char r = next_char(fd);

        while(r != '>' && r != '/' && r != '=' && r != ' '){
            if(r == EOF)
                exit(0);
            w[i]=r;
            r = fgetc(fd);
            i++;
        }ungetc(r,fd);
        if(i == 0){
            printf("vide %c\n",r); 
            return NULL;
		}
        w[i] = '\0';
        printf("word=%s\n",w);
        return w;
}

void check_next_word(FILE* fd, const char* w){
    char *r= next_word(fd);
    int i=strcmp(r,w);
    if(i!=0){
        exit(0);
    }
}

char* next_raw(FILE* fd){
    char* c = malloc(sizeof(char) * 1000);// a changer si la donne depasse
    int i=0;
    char r = fgetc(fd);
    while(r != '<'){
        c[i] = r;
        r = fgetc(fd);
        i++;
    }
    if(i == 0)
        exit(0);
    ungetc(r,fd);
    c[i] = '\0';
    printf("raw = %s\n",c );
    return c;
}

void save_xml(const char *fname, xelement_t* e){
    FILE *fd = fopen(fname,"w+");
    save_xelement(fd, e);
    fclose(fd);
}
