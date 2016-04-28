/* 
 * Simulating water (with textures) 
 */

/*
 *	Copyright (C) 2012 Andrey Mirtchovski <mirtchovski at gmail>
 *
 *	Permission is hereby granted, free of charge, to any person obtaining a copy of
 *	this software and associated documentation files (the "Software"), to deal in
 *	the Software without restriction, including without limitation the rights to
 *	use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 *	of the Software, and to permit persons to whom the Software is furnished to do
 *	so, subject to the following conditions:
 *
 *	The above copyright notice and this permission notice shall be included in all
 *	copies or substantial portions of the Software.
 *
 *	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *	SOFTWARE.
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GL/glut.h>
#include <math.h>
#include "texture.h"


#define BOUNDS	1



#define WATERSIZE 50
#define DAMP 20


static GLuint texname;

float water[2][WATERSIZE][WATERSIZE];


int spin_x, spin_y, spin_z; /* x-y rotation and zoom */
int h, w;					/* height, width  of window */
int old_x, old_y, move_z;
int depth = 3;
int i = 0;

unsigned *teximage;
int t = 0, f = 1;

void LoadTexture()
{
  int texwid, texht;
  int texcomps;
  char fn[]="watertex.rgb";
 
  teximage = read_texture(fn, &texwid, &texht, &texcomps);
  if (!teximage)
  {
	printf("Sorry, can't read texture file...");
	exit(0);
  }
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

   glGenTextures(2, &texname);
   glBindTexture(GL_TEXTURE_2D, texname);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
				   GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
				   GL_NEAREST);

  	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texwid, texht, 0, GL_RGBA, GL_UNSIGNED_BYTE, teximage);

  	glEnable(GL_TEXTURE_2D);  
	glEnable(GL_DEPTH_TEST);
} 



void calcwater() {
	int x, y;
	float n;
	for(y = 1; y < WATERSIZE-1; y++) {
		for(x = 1; x < WATERSIZE-1; x++) {
			n = ( water[t][x-1][y] +
				  water[t][x+1][y] + 
				  water[t][x][y-1] + 
				  water[t][x][y+1]
				  ) /2;
			n -= water[f][x][y];
	  		n = n - (n / DAMP);
			water[f][x][y] = n;
		}
	}

	y = 0;
	for(x = 1; x < WATERSIZE-1; x++) {
			n = ( water[t][x-1][y] +
				  water[t][x+1][y] + 
				  water[t][x][y+1]
				  ) /2;
			n -= water[f][x][y];
	  		n = n - (n / DAMP);
			water[f][x][y] = n;
	}
	
	
	x = 0;
	for(y = 1; y < WATERSIZE-1; y++) {
			n = ( water[t][x+1][y] + 
				  water[t][x][y-1] + 
				  water[t][x][y+1]
				  ) /2;
			n -= water[f][x][y];
	  		n = n - (n / DAMP);
			water[f][x][y] = n;
	}

	x = WATERSIZE-1;
	for(y = 1; y < WATERSIZE-1; y++) {
			n = ( water[t][x-1][y] +
				  water[t][x][y-1] + 
				  water[t][x][y+1]
				  ) /2;
			n -= water[f][x][y];
	  		n = n - (n / DAMP);
			water[f][x][y] = n;
	}
	y = WATERSIZE-1;
	for(x = 1; x < WATERSIZE-1; x++) {
			n = ( water[t][x-1][y] +
				  water[t][x+1][y] + 
				  water[t][x][y-1] 
				  ) /2;
			n -= water[f][x][y];
	  		n = n - (n / DAMP);
			water[f][x][y] = n;
	}


}

void init();



void reshape(int width, int height) {
	w = width;
	h = height;

	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40.0, (GLfloat) w/(GLfloat) h, 1.0, 2000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity ();
}

void display(void) {
	int i, j, tmp;
	float tx, ty;
	float texd = (float)1/WATERSIZE;		/* for texture mapping */

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
	glTranslatef(0, 0, spin_z-110);
	glRotatef(spin_x, 0, 1, 0);
	glRotatef(spin_y-60, 1, 0, 0);

	calcwater();
	glBegin(GL_TRIANGLES);
	for(i = 0; i < WATERSIZE-1; i++) {
		for(j = 0; j < WATERSIZE-1; j++) {
			tx = (float)j/WATERSIZE;
			ty = (float)i/WATERSIZE;
			//texd = (float)1/WATERSIZE;

			glTexCoord2f(tx, ty); 
			glVertex3f(j-WATERSIZE/2, i-WATERSIZE/2, water[t][j][i]);
			glTexCoord2f(tx+texd, ty); 
			glVertex3f(j+1-WATERSIZE/2, i-WATERSIZE/2, water[t][j+1][i]);
			glTexCoord2f(tx+texd, ty+texd); 
			glVertex3f(j+1-WATERSIZE/2, i+1-WATERSIZE/2, water[t][j+1][i+1]);

			glTexCoord2f(tx, ty+texd); 
			glVertex3f(j-WATERSIZE/2, i+1-WATERSIZE/2, water[t][j][i+1]);
			glTexCoord2f(tx, ty); 
			glVertex3f(j-WATERSIZE/2, i-WATERSIZE/2, water[t][j][i]);
			glTexCoord2f(tx+texd, ty+texd); 
			glVertex3f(j+1-WATERSIZE/2, i+1-WATERSIZE/2, water[t][j+1][i+1]);

		}
	}
	glEnd();

	tmp = t; t = f; f = tmp;


	glPopMatrix();

	
	glutSwapBuffers();
}

int num  = 0;
int delay = 50;

void 
idle(void)
{
	
	if(!(++num %delay)) {
		water[f][rand()%WATERSIZE][rand()%WATERSIZE] = -rand()%50+10;
		delay = rand()%100 + 100;
	}
	glutPostRedisplay();
}

void
bail(int code)
{
	exit(code);
}

void
mouse(int button, int state, int x, int y)
{

	switch(button) {
		case 0:
			old_x = x - spin_x;
			old_y = y - spin_y;
			break;
		case 2:
			old_y = y - spin_z;
			move_z = (move_z ? 0 : 1);
	}
			

	glutPostRedisplay();

}

void 
motion(x, y) {

	if(!move_z) {
		spin_x = x - old_x;
		spin_y = y - old_y;
	} else {
		spin_z = y - old_y;
	}

	glutPostRedisplay();
}


void
keyboard(unsigned char key, int x, int y)
{
	static int old_x = 50;
	static int old_y = 50;
	static int old_width = 512;
	static int old_height = 512;

	switch (key) {
		case 27:
				bail(0);
			break;
		case 'w':
				glutPositionWindow(old_x, old_y);
				glutReshapeWindow(old_width, old_height);
			break;
		case 'f':
			if (glutGet(GLUT_WINDOW_WIDTH) < glutGet(GLUT_SCREEN_WIDTH)) {
				old_x = glutGet(GLUT_WINDOW_X);
				old_y = glutGet(GLUT_WINDOW_Y);
				old_width = glutGet(GLUT_WINDOW_WIDTH);
				old_height = glutGet(GLUT_WINDOW_HEIGHT);
				glutFullScreen();
			}
			break;
		case ' ':
			water[f][WATERSIZE/2][WATERSIZE/2] = -1000;
			break;

	}
}


void init(void) {
	int i, j;

	w = glutGet(GLUT_WINDOW_WIDTH);
	h = glutGet(GLUT_WINDOW_HEIGHT);

//	glEnable(GL_LIGHTING);
//	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);

	for( i = 0; i < WATERSIZE; i++) 
		for( j = 0; j < WATERSIZE; j++) {
			water[0][j][i] = 0;
			water[1][j][i] = 0;
		}
}

int
main(int argc, char** argv)
{
	int i;

	srand(time(NULL));

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(50, 50);
	glutInitWindowSize(512, 512);
	glutInit(&argc, argv);

	glutCreateWindow("Simulating Water");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);

	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	LoadTexture();

	glEnable (GL_DEPTH_TEST);

	if(argc == 2) {
		if (strcmp(argv[1], "-h") == 0) {
			fprintf(stderr, "%s [depth]\n", argv[0]);
			exit(0);
		}
		sscanf(argv[1], "%d", &depth);
	}


	init();

	glutIdleFunc(idle);
	glutMainLoop();
	return 0;
}


