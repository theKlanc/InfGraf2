#include <stdio.h> 
#include <iostream> 
#include <string.h> 
#include <stdlib.h> 
#define _USE_MATH_DEFINES
#include <cmath>
#include <GL/glut.h> 
#include "Vista.h" 
#include "Escena.h" 
#include "Raig.h" 

using namespace std;

#define MAXX 400 
#define MAXY 400 

Color image[MAXX*MAXY];

Vector3d v1 = Vector3d(0, 0, 20); // observador 
Vector3d v2 = Vector3d(0, 0, 0);  // centre 
Vector3d v3 = Vector3d(0, 1, 0);  // up 
Escena e;

int ima, rays;
int angleH = 0;
int angleV = 0;
float dist = v1.z;

void
rayTracing(Color *image) {
	Raig r;
	Hit hit;
	int x, y, i = 0;

	Vector3d p1 = Vector3d(1, 1, 0), p2 = Vector3d(0, 1, 0),
		p3 = Vector3d(0, 0, 0), p4 = Vector3d(1, 0, 0);
	// same camera as in OpenGL ... 

	e.defVista(Vista(v1, v2, v3, 45.0, 1.0, MAXX, MAXY));
	Esfera *esf = new Esfera(Vector3d(0, 0, 0), 2, Color(1, 0, 0), 50, 0);
	e.afegirObjecte(new Esfera(Vector3d(0, 0, 0), 2, Color(1, 0, 0), 50, 0));
	e.afegirObjecte(new Esfera(Vector3d(-4, 0, -6), 2, Color(0, 1, 0), 10, 0));
	e.defLlumAmbient(Color(0.1f, 0.1f, 0.1f));
	e.defColorFons(Color(0, 0, 0));

	Vector3d v4 = Vector3d(10, 10, 0);
	Color c1 = Color(1, 1, 1);
	Focus *f1 = new Focus(c1, v4);

	e.afegirFocus(f1);
	//e.afegirFocus(new Focus(Color(1,1,1),Vector3d(-10,-10,10))); 
	//e.configura(e.AMBIENT | e.DIFOSA | e.ESPECULAR); 
	e.configura(e.DIFOSA | e.ESPECULAR);

	// comenza ray casting 
	for (y = 0; y < MAXX; y++) {
		for (x = 0; x < MAXY; x++, i++) {
			r = e.definirRaig(x, y);
			// raycasting 
			Color c = e.ilumina(r);
			image[i] = c;
		}
	}
}

void
fwritePPM(int resX, int resY, char *nameFileOut, Color *image) {
	FILE     *fdOutput;
	int       fila, col, i = 0;
	int       num_255 = 255;
	char      string[80];
	unsigned  char     r = 0, g = 0, b = 0; /* colores de los pixels */

	printf("salida en: %s\n", nameFileOut);
	printf("\nWriting output file              : [...\n");
	if ((fdOutput = fopen(nameFileOut, "w")) == NULL)
		printf("can not open output file\n");

	fwrite("P6 ", sizeof(char), 3, fdOutput); /* P6: the "magic number" */
	sprintf(string, "%ld ", resX);
	fwrite(string, sizeof(char), strlen(string), fdOutput);
	sprintf(string, "%ld ", resY);
	fwrite(string, sizeof(char), strlen(string), fdOutput);
	sprintf(string, "%d ", num_255);
	fwrite(string, sizeof(char), strlen(string), fdOutput);

	for (fila = resY; fila > 0; fila--)
		for (col = 1; col <= resX; col++, i++) {
			r = (unsigned  char)(image[i].r*num_255);
			g = (unsigned  char)(image[i].g*num_255);
			b = (unsigned  char)(image[i].b*num_255);
			fwrite(&r, sizeof(unsigned char), 1, fdOutput);
			fwrite(&g, sizeof(unsigned char), 1, fdOutput);
			fwrite(&b, sizeof(unsigned char), 1, fdOutput);
		}
	fclose(fdOutput);
	fprintf(stderr, "]\n");
}


/*  Initialize material property and light source.
 */

void
myinit(void) {
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);
}

// Display funcs
void
displayScene() {
	// orden: camara, llum, obj 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(v1.x, v1.y, v1.z, v2.x, v2.y, v2.y, v3.x, v3.y, v3.z);
	e.lightGL();
	e.dibuixa();

	glutSwapBuffers();
}

void
displayRT() {
	int i, j, cont = 0;
	glPointSize(1);
	glBegin(GL_POINTS);
	for (i = 0; i < MAXY; i++)
		for (j = 0; j < MAXX; j++, cont++) {
			glColor3f(image[cont].r, image[cont].g, image[cont].b);
			glVertex2d(j, i);
		}
	glEnd();
	glutSwapBuffers();
}


void
reshape2D(int w, int h) {
	w = MAXX;
	h = MAXY;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, w, 0, h);
	glMatrixMode(GL_MODELVIEW);
}

void
myReshape(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(e.getVista().obertura, e.getVista().rati, 1.0, 1000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


void
keyboard(unsigned char key, int x, int y) {
	switch (key) {
		/* agregados para LookAt */
	case '1':
		angleV = (angleV + 5) % 360;
		break;
	case '2':
		angleV = (angleV - 5) % 360;
		break;
	case '3':
		angleH = angleH - 5;
		break;
	case '4':
		angleH = angleH + 5;
		break;
	case '5':
		dist = dist - 1;
		break;
	case '6':
		dist = dist + 1;
		break;
	case 'r':
		rayTracing(image);
		glutSetWindow(ima);
		glutPostRedisplay();
		break;
	case 'q':
		exit(1);
		break;
	}
	// update cam 
	v1.x = sin((angleH*M_PI) / 180)*(cos((angleV*M_PI) / 180))*dist;
	v1.y = sin((angleV*M_PI) / 180)*dist;
	v1.z = cos((angleH*M_PI) / 180)*(cos((angleV*M_PI) / 180)*dist);
	v3.y = cos((angleV*M_PI) / 180);
	//printf("angv %d\n - up %f", angleV, v3.y); 

	glutPostRedisplay();
}

int
main(int argc, char **argv) {
	//Color image[MAXX*MAXY]; 

	rayTracing(image);
	//fwritePPM (MAXX, MAXY, "prueba.ppm", image); 

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(MAXX, MAXY);

	// visu 3d OpenGL 
	glutCreateWindow("Scene");
	glutReshapeFunc(myReshape);
	glutDisplayFunc(displayScene);
	glutKeyboardFunc(keyboard);
	myinit();

	// ray casting 
	ima = glutCreateWindow("Image");
	glutReshapeFunc(reshape2D);
	glutDisplayFunc(displayRT);

	glutMainLoop();
	return 0;
}
