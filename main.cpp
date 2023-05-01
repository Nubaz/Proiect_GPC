/*
* Proiect Grafica Pe Calculator
* UNIBUC, FMI, CTI, Anul III, Grupa 361
* Croitoru Eduard-Adrian
* Florian Luca-Paul
*/
#include <windows.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <GL/freeglut.h>
#include <iostream>

using namespace std;

GLfloat idxGropi = 0.;
GLfloat viteza = 7.;
GLfloat fazaFar = 1.;
GLfloat sirena = 0.5;
GLuint listaVitezometru;
const GLfloat TWO_PI = (GLfloat)6.2831853;
GLfloat acVit = 60.0;
GLdouble scaleFactor = 1;

GLint xcar = -110, ycar = -150;
GLfloat j = 1.0;
GLsizei winWidth = 800, winHeight = 600;

class scrPt {
	public:
		GLfloat x, y;
};

void drawCircle(GLuint regHex, int x, int y, int size, GLfloat RGB[3]) {
	scrPt vertex;
	GLfloat theta;
	GLint k;
	
	glNewList(regHex, GL_COMPILE);
		glColor3f(RGB[0], RGB[1], RGB[2]);
		glBegin(GL_POLYGON);
			for (k = 0; k < 5000; k++) {
				theta = TWO_PI * k / 5000;
				vertex.x = x + size * cos(theta);
				vertex.y = y + size * sin(theta);
				glVertex2f(vertex.x, vertex.y);
			}
		glEnd();
	glEndList();
}

void funcTranzitii(void) {
	glutPostRedisplay();
}

static void init(void) {
	// Culoarea spatiului nedefinit
	glClearColor((GLclampf)0.6235294, (GLclampf)0.239215, (GLclampf)0, (GLclampf)1);
	gluOrtho2D(-winWidth / 2, winWidth / 2, -winHeight / 2, winHeight / 2);

	// Amplasarea vitezometrului
	listaVitezometru = glGenLists(1);
	GLfloat RGB[3] = { 1.0, 1.0, 1.0 };
	drawCircle(listaVitezometru, 177, 177, 70, RGB);

	// Opacitatea culorilor
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

GLfloat margine(GLfloat x, GLfloat y, int semn) {
	if (semn == -1) return y -  1.125 * x - 278.125; // Marginea stanga
	else return y + 1.125 * x - 143.125; // Marginea dreapta
}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
		case 'w':
			/*if (margine(xcar, ycar + 5, -1) <= 0 && margine(xcar + 100, ycar + 5, 1) <= 0) {
				ycar += 5;
				scaleFactor -= 0.1;
			}
			if (margine(xcar, ycar + 5, -1) > 0) {
				xcar += 5;
				ycar += 5;
				scaleFactor -= 0.1;
			}
			if (margine(xcar + 100, ycar + 5, 1) > 0) {
				xcar -= 5;
				ycar += 5;
				scaleFactor -= 0.1;
			}
			if (ycar > 70)
				ycar = 70;*/
			scaleFactor -= 0.1;
			break;
		case 's':
			/*if (margine(xcar, ycar - 5, -1) <= 0 && margine(xcar + 100, ycar - 5, 1) <= 0) {
				ycar -= 5;
				scaleFactor += 0.1;
			}
			if (ycar < -200)
				ycar = -200;*/
			scaleFactor += 0.1;
			break;
		case 'a':
			if(margine(xcar - 5, ycar / scaleFactor, -1) <= 0)
				xcar -= 5;
			break;
		case 'd':
			if (margine(xcar + 105, ycar / scaleFactor, 1) <= 0)
				xcar += 5;
			break;
		case 'k':
			viteza += 5.;
			acVit -= 15.;
			if(acVit <= -90.) {
				viteza = 90.;
				acVit = -90.;
			}
			break;
		case 'l':
			viteza -= 5.;
			acVit += 15.;
			if(acVit >= 90.) {
				viteza = 0.;
				acVit = 90.;
			}
			break;
		default:
			break;
	}
}

// idee scalare : adaugat / scazut aceeasi val. la bmare, bmica si h
void drawTrapez(int x, int y, int bmare, int bmica, int h) {
	glBegin(GL_POLYGON);
		glVertex2i(x, y);
		glVertex2i(x + bmare, y);
		glVertex2i(x + bmare - (bmare - bmica) / 2, y + h);
		glVertex2i(x + (bmare - bmica) / 2, y + h);
	glEnd();
}

void desenDrum(void) {
	glClear(GL_COLOR_BUFFER_BIT);

	// Incrementare pentru girofar si alte translatii
	glutIdleFunc(funcTranzitii);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// ----- DRUMUL -----
	// Drumul efectiv
	glColor3f((GLfloat)0.3, (GLfloat)0.3, (GLfloat)0.3);
	drawTrapez(-450, -400, 900, 200, 400);

	// Linia care separa benzile
	glColor3f((GLfloat)1, (GLfloat)1, (GLfloat)1);
	drawTrapez(-40, -400, 90, 20, 400);

	// ----- CERUL -----
	glColor3f((GLfloat)0, (GLfloat)0, (GLfloat)0.7);
	glRecti(-450, -30, 900, 700);

	// Initializare pentru tranzitii
	glPushMatrix();
	glScaled(scaleFactor, scaleFactor, 0);
	glTranslated(xcar, ycar, 0);

	// ----- MASINA -----
	// Sasiu
	glColor3f((GLfloat)0, (GLfloat)0, (GLfloat)0);
	drawTrapez(60, -100, 100, 80, 40);

	// Geam spate
	glColor3f((GLfloat)0, (GLfloat)0.3, (GLfloat)0.6);
	drawTrapez(70, -60, 80, 60, 20);

	// ----- VITEZOMETRU -----
	// Vitezometrul efectiv
	glLoadIdentity();
	glCallList(listaVitezometru);

	// Animatia vitezometrului
	glPushMatrix();
	glTranslated(177, 177, 0);
	glRotated(acVit, 0, 0, 1);
	glTranslated(-177, -177, 0);

	// Acul
	glColor3f(1.0, 0.0, 0.0);
	glLineWidth(7.0);
	glBegin(GL_LINES);
		glVertex2f(177, 177);
		glVertex2f(177, 237);
	glEnd();

	glutSwapBuffers();
	glFlush();
}

void winReshapeFcn(GLint newWidth, GLint newHeight) {
	glViewport(0, 0, (GLsizei)newWidth, (GLsizei)newHeight);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluOrtho2D(-winWidth / 2, winWidth / 2, -winHeight /2, winHeight / 2);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glClear(GL_COLOR_BUFFER_BIT);
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowPosition(150, 150);
	glutInitWindowSize(winWidth, winHeight);
	glutCreateWindow("Need for Speed Chiajna");

	init();

	glutDisplayFunc(desenDrum);
	glutReshapeFunc(winReshapeFcn);
	glutKeyboardFunc(keyboard);

	glutMainLoop();
}