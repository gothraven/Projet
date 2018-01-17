#include "xml.h"

#define BUFF_SIZE 100
void	print_xelement(xelement_t* e)
{
	printf("print_xelement\n");
	save_xelement(stdout, e);
}
/*
int		espacement(char c)
{
	if (c == ' ' || c == '\n' || c == '\r' || c == '\t')
		return 1;
	return 0;
}

char	next_char(FILE *fd)
{
	char c = '\0';
	c = fgetc(fd);
	while (espacement(c) == 1)
	{
		if (c == EOF)
			quitter("fin de fichier","next_char");
		c = fgetc(fd);
	}
	if (c == '\0')
		quitter("c'est impossible","next_char");
	return c;
}

void	check_next_char(FILE *fd, char c)
{
	char ch;
	ch = next_char(fd);
	if (ch != c)
		quitter("","check_next_char");
}

bool	is_next_char(FILE* fd, char c, bool cons)
{
	char ch;
	ch = next_char(fd);
	if (ch != c || cons == false)
		ungetc(ch , fd);
	if (ch != c)
		return false;
	return true;
}

char*	next_word(FILE* fd)
{
	char*	word;
	int 	i;
	char	c;

	if ((word = (char*)malloc(BUFF_SIZE * sizeof(char))) == NULL)
		quitter("malloc","next_word");
	i = 0;
	c = next_char(fd);
	if (c == '<' || c == '>' || c == '/') {
		ungetc(c, fd);
		return NULL;
	}
	ungetc(c, fd);
	do {
		word[i] = fgetc(fd);
		c = word[i];
		i++;
		if ((i % BUFF_SIZE) == 0)
			if ((word = (char*)realloc(word, i + 1024)) == NULL)
				quitter("malloc","next_word");
	} while(c != '<' && c != '>' && c != '/' && c != '=' && espacement(c) != 1);
	ungetc(c, fd);
	word[i - 1] = '\0';
	return word;
}

void	check_next_word(FILE* fd, const char* w)
{
	char*	str = next_word(fd);

	if (strcmp(str, w) != 0)
		quitter("","check_next_word");
}
*/
char*	next_string(FILE* fd)
{
	char*	str;
	char 	c;
	int 	i;

	if ((str = (char*)malloc(BUFF_SIZE * sizeof(char))) == NULL) 
	//			quitter("malloc","next_string");
			exit(1);
	i = 0;
	c = '\0';
	c = next_char(fd);
	if (c != '"')
			exit(1);
				//quitter("","next_string");
	do {
		c = fgetc(fd);
		str[i] = c;
		i++;
		if ((i % BUFF_SIZE) == 0) {
			if ((str = (char*)realloc(str, i + BUFF_SIZE)) == NULL) {
			exit(1);
				//quitter("malloc","next_string");
			}
		}
	} while (c != '"');
	str[i - 1] = '\0';
	printf("string = %s\n",str );
	return str;
}
/*
char*	next_raw(FILE* fd) {
	char*	raw;
	char 	c;
	int 	i;

	if ((raw= (char*)malloc(BUFF_SIZE * sizeof(char))) == NULL) {
		quitter("malloc","next_raw");
	}

	i = 0;
	c = '\0';
	do {
		fread(&c, sizeof(char), 1, fd);
		raw[i] = c;
		i++;
		if ((i % BUFF_SIZE) == 0) {
			if ((raw = (char*)realloc(raw, i + BUFF_SIZE)) == NULL) {
				quitter("malloc","next_raw");
			}
		}
	} while (c != '<');
	ungetc(c, fd);
	raw[i - 1] = '\0';
	return raw;
}
*/
/*==================import fonction spicher=================================*/
xelement_t* load_xelement(FILE* fd, const char* end_tag);

void	load_xelement_raw(FILE* fd, xelement_t* e) {
	char* w = next_raw(fd);
	check_next_char(fd, '<');
	check_next_char(fd, '/');
	check_next_word(fd, e->nom);
	check_next_char(fd, '>');
	printf("%s***************************\n",w);
	add_raw(e,w);
	free(w);
}

void	load_xelement_sub(FILE* fd, xelement_t* e) {
	xelement_t* f = load_xelement(fd, e->nom);
	if (f != NULL) {
		add_sub_xelement(e,f);
		load_xelement_sub(fd, e);
	}
}

void	load_xelement_content(FILE* fd, xelement_t* e) {
	if (is_next_char(fd,'<',false)) {
		load_xelement_sub(fd, e);
	}
	else{
		load_xelement_raw(fd, e);
	}
}

xelement_t*	load_xelement(FILE* fd, const char* end_tag) {
	xelement_t* e = NULL;
	char c;

	check_next_char(fd,'<');
	if ((end_tag) && (is_next_char(fd,'/',true))) {
		check_next_word(fd,end_tag);
		check_next_char(fd,'>');
		return NULL;
	}	
	char* name = next_word(fd);
	if (name == NULL) {
		fprintf(stderr, "load_xelement: tag name expected\n");
		exit(EXIT_FAILURE);
	}
	e = create_xelement(name);
	free(name);
	while((name = next_word(fd)) != NULL) {
		check_next_char(fd,'=');
		char* value = next_string(fd);
		add_xattribute(e,name,value);
	}
	c = next_char(fd);
	
	if (c == '/') {
		check_next_char(fd,'>');
		return e;
	}
	if (c == '>') {
		load_xelement_content(fd, e);
		return e;
	}
	fprintf(stderr, "load_xelement: end of markup expected ('>' or '/>'), but got %c\n", c);
	exit(EXIT_FAILURE);
}

xelement_t*	load_xml(const char* fname)
{
	printf("load_xml debut\n");
	FILE* fd = fopen(fname, "r");
	if (fd == NULL)
		printf("probleme\n");
	//	quitter("fd","load_xml");
	xelement_t* e = load_xelement(fd, NULL);
	fclose(fd);
	return e;
}


