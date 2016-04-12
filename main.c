#include <stdio.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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
	glClearColor(0.0, 0.0, 0.0, 0.0);
	gluOrtho2D(0.0, (float)(screenWidth - 1), 0.0, (float)(screenHeight -1));
}

void renderMenu() {
	float midX = screenWidth / 2;
	midX = (midX + midX - 1) / 2;
	float midY = screenHeight / 2;
	midY = (midY + midY - 1) / 2;
	int margin = 30;
	printf("X: %f Y: %f\n", midX, midY);
	glColor3f(1.0, 1.0, 1.0);
	// Play button Begin
	glBegin(GL_QUADS);
		glVertex2f(midX - 245, midY + 50);
		glVertex2f(midX - 245, midY - 50);
		glVertex2f(midX - 145, midY - 50);
		glVertex2f(midX - 145, midY + 50);
	glEnd();
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_TRIANGLES);
		glVertex2f(midX - 170, midY);
		glVertex2f(midX - 220, midY + 25);
		glVertex2f(midX - 220, midY - 25);
	glEnd();
	// Play Button End
	glColor3f(1.0, 1.0, 1.0);
	// High Score Button Begin
	glBegin(GL_QUADS);
		glVertex2f(midX - 115, midY + 50);
		glVertex2f(midX - 115, midY - 50);
		glVertex2f(midX - 15, midY - 50);
		glVertex2f(midX - 15, midY + 50);
	glEnd();
	glColor3f(0.0, 0.0, 0.0);
	glLineWidth(10.0);
	glBegin(GL_LINES);
		glVertex2f(midX - 100, midY);
		glVertex2f(midX - 90, midY);
		glVertex2f(midX - 85, midY);
		glVertex2f(midX - 25, midY);
		
		glVertex2f(midX - 100, midY + 20);
		glVertex2f(midX - 90, midY + 20);
		glVertex2f(midX - 85, midY + 20);
		glVertex2f(midX - 25, midY + 20);
		
		glVertex2f(midX - 100, midY - 20);
		glVertex2f(midX - 90, midY - 20);
		glVertex2f(midX - 85, midY - 20);
		glVertex2f(midX - 25, midY - 20);
	glEnd();
	// High Scores Button End
	glColor3f(1.0, 1.0, 1.0);

	glBegin(GL_QUADS);
		glVertex2f(midX + 115, midY + 50);
		glVertex2f(midX + 115, midY - 50);
		glVertex2f(midX + 15, midY - 50);
		glVertex2f(midX + 15, midY + 50);
	glEnd();
	glBegin(GL_QUADS);
		glVertex2f(midX + 245, midY + 50);
		glVertex2f(midX + 245, midY - 50);
		glVertex2f(midX + 145, midY - 50);
		glVertex2f(midX + 145, midY + 50);
	glEnd();
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT);
	// glPointSize(100.0);
	glEnable(GL_TEXTURE_2D);
	glColor3f(1.0, 1.0, 1.0);
	glRasterPos2i(((screenWidth / 2) - strlen(title)), screenHeight - 100);
	for (int i = 0; i < strlen(title); i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, title[i]);
	}
	
	glRasterPos2i(screenWidth - 200, 100);
	for (int i = 0; i < strlen(devBy); i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, devBy[i]);
	}
	
	glRasterPos2i(screenWidth - 200, 80);
	for (int i = 0; i < strlen(names); i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, names[i]);
	}
	renderMenu();
	glFlush();
}
	
/* Function Description:
 *
 *
 */
void mouseClicked(int btn, int state, int x, int y) {
	float midX = screenWidth / 2;
	midX = (midX + midX - 1) / 2;
	float midY = screenHeight / 2;
	midY = (midY + midY - 1) / 2;
	int xMin = midX - 245; 
	int xMax = midX - 145;
	int yMin = midY - 50;
	int yMax = midY + 50;
	printf("x is :%d and y is :%d\n", x, y);
	if ((btn == 'q' || btn == 'Q') && state == GLUT_UP) {
		exit(0);
	} else if(x > xMin && x < xMax && y > yMin && y < yMax && btn == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		// Play button is clicked
		pid_t gamePid;
		if ((gamePid = fork()) < 0) {
			printf("Game could not be launched.\n");
		} else if(gamePid == 0) {
			printf("Game successfully launched.\n");
			system("g++ objss.cpp -lglut -lGL -lGLU -o game && ./game");
		} else {
			exit(0);
		}
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
