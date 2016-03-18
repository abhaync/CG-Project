#include "include/functions.h"
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int screenHeight = 0;
int screenWidth = 0;
char *title = "<Project_Name>";
char *centreText = "Press Enter to begin";
char *devBy = "Developed By:";
char *names = "Abhay & Sandeep";

/* Function Description:
 *
 *
 */
void initialize() {
	glClearColor(0.0, 0.0, 0.0, 1.0);
	gluOrtho2D(0.0, (float)(screenWidth - 1), 0.0, (float)(screenHeight -1));
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT);
	glPointSize(100.0);
	glColor3f(1.0, 1.0, 1.0);
	
	glRasterPos2i(((screenWidth / 2) - strlen(title)), screenHeight - 100);
	for (int i = 0; i < strlen(title); ++i) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, title[i]);
	}
	
	glRasterPos2i((screenWidth / 2) - strlen(centreText)/2 , screenHeight / 2);
	for (int i = 0; i < strlen(centreText); ++i) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, centreText[i]);
	}
	
	glRasterPos2i(screenWidth - 200, 100);
	for (int i = 0; i < strlen(devBy); ++i) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, devBy[i]);
	}
	
	glRasterPos2i(screenWidth - 200, 80);
	for (int i = 0; i < strlen(names); ++i) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, names[i]);
	}
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
	switch (key) {
	case 27: // Escape key
			exit (0);
			break;
	}
}

int main(int argc, char *argv[]) {
	glutInit(&argc, argv);
	screenWidth = glutGet(GLUT_SCREEN_WIDTH);
	screenHeight = glutGet(GLUT_SCREEN_HEIGHT);
	printf("Height: %d\nWidth: %d\n", screenHeight, screenWidth);
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB|GLUT_DEPTH);
	glutCreateWindow("CG-Project");
	initialize();
	glutInitWindowSize(screenWidth, screenHeight);
	glutInitWindowPosition(0, 0);
	glutFullScreen();
	glMatrixMode(GL_PROJECTION);
	glutDisplayFunc(display);
	glutMouseFunc(mouseClicked);
	glutKeyboardFunc(keyPressed);
	glutMainLoop();
	return 0;
}
