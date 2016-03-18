#include "include/functions.h"
#include <GL/glut.h>
#include <stdlib.h>

// int windowHeight = glutGet(GLUT_WINDOW_HEIGHT);
// int windowWidth = glutGet(GLUT_WINDOW_WIDTH);


/* Function Description:
 *
 *
 */
void initialize() {
	glClearColor(0.0, 0.0, 0.0, 1.0);
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT);
	glPointSize(5.0);
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_POINTS);
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
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB|GLUT_ALPHA|GLUT_DEPTH);
	glutCreateWindow("CG-Project");
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutFullScreen();
	glutDisplayFunc(display);
	glutMouseFunc(mouseClicked);
	glutKeyboardFunc(keyPressed);
	initialize();
	glutMainLoop();
	return 0;
}