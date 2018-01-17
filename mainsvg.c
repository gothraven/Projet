

#include "svg.h"
int main() {
	figure_t f;
	char  c;

	f = rectangle(10,7);
	f = translate(f, 10, 8);
	f = rotate(f, 45 * M_PI / 180);

	c = intersect(coordinate(5,5),f,1);
	if (c) printf("Intersection with color %c\n",c); else printf("No intersection\n");
	c = intersect(coordinate(10,10),f,1);
	if (c) printf("Intersection with color %c\n",c); else printf("No intersection\n");
	c = intersect(coordinate(18,18),f,1);
	if (c) printf("Intersection with color %c\n",c); else printf("No intersection\n");

	f = circle(12);
	f = translate(f, 20, 13);
	f = color(f, 'O');

	c = intersect(coordinate(5,5),f,1);
	if (c) printf("Intersection with color %c\n",c); else printf("No intersection\n");
	c = intersect(coordinate(10,10),f,1);
	if (c) printf("Intersection with color %c\n",c); else printf("No intersection\n");
	c = intersect(coordinate(18,18),f,1);
	if (c) printf("Intersection with color %c\n",c); else printf("No intersection\n");

	f = line(20);
	f = translate(f, 18, 18);
	f = rotate(f, -60 * M_PI / 180);
	f = color(f, '*');

	c = intersect(coordinate(5,5),f,1);
	if (c) printf("Intersection with color %c\n",c); else printf("No intersection\n");
	c = intersect(coordinate(10,10),f,1);
	if (c) printf("Intersection with color %c\n",c); else printf("No intersection\n");
	c = intersect(coordinate(18,18),f,1);
	if (c) printf("Intersection with color %c\n",c); else printf("No intersection\n");

	return EXIT_SUCCESS;
}
