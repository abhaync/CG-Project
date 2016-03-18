#include "include/functions.h"
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>

//int windowHeight = glutGet(GLUT_SCREEN_HEIGHT);
//int windowWidth = glutGet(GLUT_SCREEN_WIDTH);


/* Function Description:
 *
 *
 */
void initialize() {
	glClearColor(1.0, 1.0, 1.0, 1.0);
	gluOrtho2D(0.0,499.0,0.0,499.0);
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT);
	glPointSize(5.0);
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_LINES);
		glVertex2f(115.0, 115.0);
		glVertex2f(215.0, 215.0);
	glEnd();
	// printf("Height: %d\n   Width: %d\n", windowHeight, windowWidth);
	glFlush();
}

/* Function Description:
 *
 *
 */
void mouseClicked(int btn, int state, int x, int y) {
	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		exit(0);
	}
}

/* Function Description:
 *
 *
 */
void keyPressed(unsigned char key, int x, int y) {

}

int main(int argc, char *argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB|GLUT_DEPTH);
	glutCreateWindow("CG-Project");
	glutInitWindowSize(500,500);
	glutInitWindowPosition(0,0);
	initialize();
	glutFullScreen();
	glutDisplayFunc(display);
	glutMouseFunc(mouseClicked);
	//glutKeyboardFunc(keyPressed);
	//initialize();
	glutMainLoop();
	return 0;
}
