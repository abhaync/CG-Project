#include "include/functions.h"
#include "include/src/SOIL.h"
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

GLuint loadAndDisplayImage(const char *filename, int width, int height) {
	unsigned char *data;
    FILE *file;
 
    puts(filename);
    // open texture data
    file = fopen(filename, "r");
    if (file == NULL) return 0;
 
    // allocate buffer
    data = (unsigned char*) malloc(width * height * 4);
 
    //read texture data
    fread(data, width * height * 4, 1, file);
    fclose(file);
	GLuint textureBufferId;
	textureBufferId = SOIL_load_OGL_texture // load an image file directly as a new OpenGL texture
    (
        "img_test.bmp",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
    );
    if (textureBufferId == 0) {
    	printf("Loading image error\n %s", SOIL_last_result());
    }

	glGenTextures(1, &textureBufferId);
	glBindTexture(GL_TEXTURE_2D, textureBufferId);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 256, 256, 0,GL_RGBA, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	return textureBufferId;
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT);
	// glPointSize(100.0);
	GLuint id = loadAndDisplayImage("img_test.bmp", 256, 256);
	glEnable(GL_TEXTURE_2D);
	// glColor3f(1.0, 1.0, 1.0);
	
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
