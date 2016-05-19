#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "water-tex.h"

#define KEY_ESCAPE 27

 int displayFlag = 0;
 
using namespace std;

float norm[3];
GLfloat fa=0,g=0,ga=0,fb=0, g2=-5, g1=5;
float f1 = 0, f2 = 0;
bool* keyStates = new bool[256]; 
void fire_b1();
void fire_b2();
int posShip1 = 185;
int totalHits1 = 0, totalHits2 = 0, hits1 = 0, hits2 = 0, miss1=0, miss2= 0;
int posShip2 = 115;
int fullHealth1 = 75;
int fullHealth2 = 75;
char *player1, *player2;

typedef struct bullet
{
	bool spawn;
	float spx,spy;
};

bool is_spawn(bullet *b)
{
	if(b->spawn)
		return true;
	else
		return false;
}

bullet* create_bullet(float posx, float posy)
{
	bullet *bul;
	bul = new (bullet);
	bul->spx = posx;
	bul->spy = posy;
	bul->spawn = false;
	return bul;
}

bullet *b1 = NULL,*b2 = NULL;
/************************************************************************
  Window
 ************************************************************************/
 
typedef struct {
    int width;
	int height;
	char* title;
 
	float field_of_view_angle;
	float z_near;
	float z_far;
} glutWindow;
 
 
 
/*************************************************************************** 
  OBJ Loading 
 ***************************************************************************/
 
class Model_OBJ
{
  public: 
	Model_OBJ();			
    float* calculateNormal(float* coord1,float* coord2,float* coord3 );
    int Load(char *filename);	// Loads the model
	void Draw();					// Draws the model on the screen
	void Release();				// Release the model
 
	float* normals;							// Stores the normals
    float* Faces_Triangles;					// Stores the triangles
	float* vertexBuffer;					// Stores the points which make the object
	long TotalConnectedPoints;				// Stores the total number of connected verteces
	long TotalConnectedTriangles;			// Stores the total number of connected triangles
 
};
 
 
#define POINTS_PER_VERTEX 3
#define TOTAL_FLOATS_IN_TRIANGLE 9
using namespace std;
 
Model_OBJ::Model_OBJ()
{
	this->TotalConnectedTriangles = 0; 
	this->TotalConnectedPoints = 0;
}
 
float* Model_OBJ::calculateNormal( float *coord1, float *coord2, float *coord3 )
{
   /* calculate Vector1 and Vector2 */
   float va[3], vb[3], vr[3], val;
   va[0] = coord1[0] - coord2[0];
   va[1] = coord1[1] - coord2[1];
   va[2] = coord1[2] - coord2[2];
 
   vb[0] = coord1[0] - coord3[0];
   vb[1] = coord1[1] - coord3[1];
   vb[2] = coord1[2] - coord3[2];
 
   /* cross product */
   vr[0] = va[1] * vb[2] - vb[1] * va[2];
   vr[1] = vb[0] * va[2] - va[0] * vb[2];
   vr[2] = va[0] * vb[1] - vb[0] * va[1];
 
   /* normalization factor */
   val = sqrt( vr[0]*vr[0] + vr[1]*vr[1] + vr[2]*vr[2] );
 
	
	norm[0] = vr[0]/val;
	norm[1] = vr[1]/val;
	norm[2] = vr[2]/val;
 
 
	return norm;
}
 
 
int Model_OBJ::Load(char* filename)
{
	string line;
	ifstream objFile (filename);	
	if (objFile.is_open())													// If obj file is open, continue
	{
		objFile.seekg (0, ios::end);										// Go to end of the file, 
		long fileSize = objFile.tellg();									// get file size
		objFile.seekg (0, ios::beg);										// we'll use this to register memory for our 3d model
 
		vertexBuffer = (float*) malloc (fileSize);							// Allocate memory for the verteces
		Faces_Triangles = (float*) malloc(fileSize*sizeof(float));			// Allocate memory for the triangles
		normals  = (float*) malloc(fileSize*sizeof(float));					// Allocate memory for the normals
 
		int triangle_index = 0;												// Set triangle index to zero
		int normal_index = 0;												// Set normal index to zero
 
		while (! objFile.eof() )											// Start reading file data
		{		
			getline (objFile,line);											// Get line from file
 
			if (line.c_str()[0] == 'v')										// The first character is a v: on this line is a vertex stored.
			{
				line[0] = ' ';												// Set first character to 0. This will allow us to use sscanf
 
				sscanf(line.c_str(),"%f %f %f ",							// Read floats from the line: v X Y Z
					&vertexBuffer[TotalConnectedPoints],
					&vertexBuffer[TotalConnectedPoints+1], 
					&vertexBuffer[TotalConnectedPoints+2]);
 
				TotalConnectedPoints += POINTS_PER_VERTEX;					// Add 3 to the total connected points
			}
			if (line.c_str()[0] == 'f')										// The first character is an 'f': on this line is a point stored
			{
		    	line[0] = ' ';												// Set first character to 0. This will allow us to use sscanf
 
				int vertexNumber[4] = { 0, 0, 0 };
                sscanf(line.c_str(),"%i%i%i",								// Read integers from the line:  f 1 2 3
					&vertexNumber[0],										// First point of our triangle. This is an 
					&vertexNumber[1],										// pointer to our vertexBuffer list
					&vertexNumber[2] );										// each point represents an X,Y,Z.
 
				vertexNumber[0] -= 1;										// OBJ file starts counting from 1
				vertexNumber[1] -= 1;										// OBJ file starts counting from 1
				vertexNumber[2] -= 1;										// OBJ file starts counting from 1
 
 
				/********************************************************************
				 * Create triangles (f 1 2 3) from points: (v X Y Z) (v X Y Z) (v X Y Z). 
				 * The vertexBuffer contains all verteces
				 * The triangles will be created using the verteces we read previously
				 */
 
				int tCounter = 0;
				for (int i = 0; i < POINTS_PER_VERTEX; i++)					
				{
					Faces_Triangles[triangle_index + tCounter   ] = vertexBuffer[3*vertexNumber[i] ];
					Faces_Triangles[triangle_index + tCounter +1 ] = vertexBuffer[3*vertexNumber[i]+1 ];
					Faces_Triangles[triangle_index + tCounter +2 ] = vertexBuffer[3*vertexNumber[i]+2 ];
					tCounter += POINTS_PER_VERTEX;
				}
 
				/*********************************************************************
				 * Calculate all normals, used for lighting
				 */ 
				float coord1[3] = { Faces_Triangles[triangle_index], Faces_Triangles[triangle_index+1],Faces_Triangles[triangle_index+2]};
				float coord2[3] = {Faces_Triangles[triangle_index+3],Faces_Triangles[triangle_index+4],Faces_Triangles[triangle_index+5]};
				float coord3[3] = {Faces_Triangles[triangle_index+6],Faces_Triangles[triangle_index+7],Faces_Triangles[triangle_index+8]};
				float *norm = this->calculateNormal( coord1, coord2, coord3 );
 
				tCounter = 0;
				for (int i = 0; i < POINTS_PER_VERTEX; i++)
				{
					normals[normal_index + tCounter ] = norm[0];
					normals[normal_index + tCounter +1] = norm[1];
					normals[normal_index + tCounter +2] = norm[2];
					tCounter += POINTS_PER_VERTEX;
				}
 
				triangle_index += TOTAL_FLOATS_IN_TRIANGLE;
				normal_index += TOTAL_FLOATS_IN_TRIANGLE;
				TotalConnectedTriangles += TOTAL_FLOATS_IN_TRIANGLE;			
			}	
		}
		objFile.close();														// Close OBJ file
	}
	else 
	{
		cout << "Unable to open file";								
	}
	return 0;
}
 
void Model_OBJ::Release()
{
	free(this->Faces_Triangles);
	free(this->normals);
	free(this->vertexBuffer);
}
 
void Model_OBJ::Draw()
{
	
glEnableClientState(GL_VERTEX_ARRAY);						// Enable vertex arrays
 	glEnableClientState(GL_NORMAL_ARRAY);						// Enable normal arrays
	glVertexPointer(3,GL_FLOAT,	0,Faces_Triangles);				// Vertex Pointer to triangle array
	glNormalPointer(GL_FLOAT, 0, normals);						// Normal pointer to normal array
	glDrawArrays(GL_TRIANGLES, 0, TotalConnectedTriangles);		// Draw the triangles
	glDisableClientState(GL_VERTEX_ARRAY);						// Disable vertex arrays
	glDisableClientState(GL_NORMAL_ARRAY);						// Disable normal arrays
}
 
/***************************************************************************
 * Program code
 ***************************************************************************/
 
Model_OBJ obj,obj1;
float g_rotation;
glutWindow win;

void keyOperations()
{
	if(keyStates['a'])
	{
		fa-=0.43;
		fb-=0.4773;
		g1-=0.475;
		posShip1--;
	}
	if(keyStates['d'])
	{
		fa+=0.43;
		fb+=0.4773;
		g1+=0.5;	
		posShip1++;
	}	
	if(keyStates['j'])
	{
		g-=0.2;
		ga-=0.275;
		g2-=0.5;
		posShip2--;
	}
	if(keyStates['l'])
	{
		g+=0.2;
		ga+=0.275;
		g2+=0.5;
		posShip2++;
	}
	if(keyStates['w'])
	{
		totalHits2++;
		b1 = create_bullet(g1,-5);
		fire_b1();
		f2 += 0.3;
	}
	if(keyStates['i'])
	{
		totalHits1++;
		b2 = create_bullet(g2,-5);
		fire_b2();
		f1+=0.3;
	}
}

void loadBackgroundImage(char *image) {
	char *imageData;
	FILE *file;
	int width = 1920;
	int height = 1080;
	file = fopen(image, "r");
	imageData = (char*) malloc(width * height * 24);
	int imageSize = width * height * 24;
	fread(imageData, width * height * 4, 1, file);
	fclose(file);
	 /*
	 * TGA is stored in BGR (Blue-Green-Red) format,
	 * we need to convert this to Red-Green-Blue (RGB).
	 * The following section does BGR to RGB conversion
	 */
	for (int i = 0; i < imageSize; i+=3) {
		// 24 bits per pixel   =  3 byte per pixel
		char c = imageData[i];
		imageData[i] = imageData[i+2];
		imageData[i+2] = c;
	}
	glRasterPos2i(0,0);
	glPixelStorei (GL_UNPACK_ROW_LENGTH, width);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glDrawPixels(width, height, GL_RGB, GL_UNSIGNED_BYTE, imageData);
}
 
void display() 
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
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
	keyOperations();
	glColor3f(0.0, 1.0, 0.0);
	glPushMatrix();
		gluLookAt(0, 0, 100, 0, 0, 0, 0, 1, 0);
		glTranslatef(0, 67, -10);
		glBegin(GL_QUADS);
			glVertex2f(0, 0);
			glVertex2f(fullHealth1, 0);
			glVertex2f(fullHealth1, 10);
			glVertex2f(0, 10);
		glEnd();
	glPopMatrix();
	glPushMatrix();
		gluLookAt(0, 0, 100, 0, 0, 0, 0, 1, 0);
		glTranslatef(-90,60, 0);
		glBegin(GL_QUADS);
			glVertex2f(0, 0);
			glVertex2f(fullHealth2, 0);
			glVertex2f(fullHealth2, 10);
			glVertex2f(0, 10);
		glEnd();
	glPopMatrix();
	glColor3f(0.0,1.0,0.0);
	glPushMatrix();
		gluLookAt(0,0,50,0,0,0,0,1,0);
		glTranslatef(-24+ga,-11,-5);
		glRotatef(90,0,1,0);
		glBegin(GL_LINE_LOOP);
			glVertex3f(0,0, 0);
			glVertex3f(0,8, 0);
			glVertex3f(0,8, 30);
			glVertex3f(0,0, 30);
		glEnd();
	glPopMatrix();	
	glColor3f(1, 0, 0);
	glPushMatrix();
		gluLookAt( 0,0,100, 0,0,0, 0,1,0);
		glRotatef(90,0,1,0);
		// glRotatef(90,0,1,0);
		glTranslatef(0,-5,-5);
		glScalef(2, 2, 2);
		glTranslatef(0,0,g);
	//	g_rotation++;
		obj.Draw();
		
	glPopMatrix();
	glColor3f(1,0,0);
	glPushMatrix();
		gluLookAt(0,0,100,0,0,0,0,1,0);
		glTranslatef(-10+fb,5,0);
		glRotatef(90,0,1,0);
		glBegin(GL_LINE_LOOP);
			glVertex3f(0,0, 0);
			glVertex3f(0,7, 0);
			glVertex3f(0,7, 25);
			glVertex3f(0,0, 25);
		glEnd();
	glPopMatrix();	
		
	glColor3f(0.0, 1.0, 1.0);
	glPushMatrix();
		gluLookAt( 0,0,100, 0,0,0, 0,1,0);
		glRotatef(90,0,1,0);
		glTranslatef(0,10,6);
		glScalef(1,1,1);
		glTranslatef(0,0,fa);
		obj1.Draw();		
	glPopMatrix();
	glutSwapBuffers();
	glFlush();

}

void reduceHealthBar1() {
	fullHealth1 -= 10;
	if (fullHealth1 < 0)
	{
		int score2 = (hits1*100)-(miss1*10);
		printf("player - 2 won!!with a score %f\n", (float)score2);
		FILE *fp = fopen("highscores.txt", "a+");
		fprintf(fp, "%s:\t%f\n", player2, (float)score2);
		fclose(fp);
		exit(0);
	}
	glutPostRedisplay();
}


void reduceHealthBar2() {
	fullHealth2 -= 10;
	if (fullHealth2 < 0)
	{
		int score2 = (hits2*100)-(miss2*10);
		printf("player - 1 won!! with a score %f\n",(float)score2);
		FILE *fp = fopen("highscores.txt", "a+");
		fprintf(fp, "%s:\t%f\n", player1, (float)score2);
		fclose(fp);
		exit(0);
	}
	glutPostRedisplay();
}

void isHit1() {
	printf("abs values: %d and %d\n", posShip1, posShip2);
	//int diff = posShip2 - 115;
	if(posShip1 > posShip2) {
		if (posShip2 + 58 < (posShip1 + 110) && posShip2 + 58 > posShip1 ) {
			printf("Hit!!!\n");
			hits1++;
			reduceHealthBar1();
		} else {
			miss1++;
		}
	}
}

void isHit2() {
	printf("abs values: %d and %d\n", posShip1, posShip2);
	float diff = posShip1 - posShip2;
	//int diff = posShip2 - 115;
	//if(posShip1 < posShip2)
		if (diff < 110 && posShip1 - 60 < (posShip2 + 115) && posShip1 - 60 > posShip2 ) {
			printf("Hit!!!\n");
			reduceHealthBar2();
		}
		else if(diff < 60 && diff > -22 && posShip1 - 60 < (posShip2 + 115))
		{
			printf("Hit!!!\n");
			hits2++;
			reduceHealthBar2();
		} else {
			miss2++;
		}
}


void fire_b1()
{
		
	if(b1->spawn)
	{
		// printf("f2 value: %f\n",f2 );
		if(f2 > 50)
		{
			keyStates['w'] = false;
			f2 = 0;
			b1->spawn = false;
		}
		glPushMatrix();	
	 		gluLookAt(0,0,100,0,0,0,0,1,0);
	 		
	 		glRotatef(90,0,1,0);
	 		

		glPopMatrix();
		b1->spawn = false;
		// else
		// {
		// 	f2 += 0.3;
		// }
	}
	else
	{
		// printf("%f\n",f2 );
		b1->spawn = true;
		glDisable(GL_LIGHTING);
	 	glDisable(GL_LIGHT0);
	 	glPushMatrix();	
	 		gluLookAt(0,0,100,0,0,0,0,1,0);
	 		glTranslatef(-5,7-f2,0);
	 		glRotatef(90,0,1,0);
			glColor3f(1.0,0.0,0.0);
	 		glBegin(GL_POINTS);
				glVertex3f(0,b1->spy,b1->spx);
			glEnd();
			glPointSize(5.0);
		glPopMatrix();
		if(f2 > 15)
		{
			isHit2();
			f2 = 0;
			keyStates['w'] = false;
		}
	}
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
}

void fire_b2()
{
	
	if(b2->spawn)
	{
		// printf("f1 value: %f\n",f1 );
		if(f1 > 50)
		{
			keyStates['i'] = false;
			f1 = 0;
			b2->spawn = false;
		}
		glPushMatrix();	
	 		gluLookAt(0,0,100,0,0,0,0,1,0);
	 		
	 		glRotatef(90,0,1,0);
	 		

		glPopMatrix();
		b2->spawn = false;
	}
	else
	{
		// printf("%f\n",f1 );
		b2->spawn = true;
		glDisable(GL_LIGHTING);
	 	glDisable(GL_LIGHT0);
	 	glPushMatrix();	
	 		gluLookAt(0,0,100,0,0,0,0,1,0);
	 		glTranslatef(-10,-5+f1,0);
	 		glRotatef(90,0,1,0);
			glColor3f(1.0,0.0,0.0);
	 		glBegin(GL_POINTS);
				glVertex3f(0,b2->spy,b2->spx);
			glEnd();
			glPointSize(5.0);
		glPopMatrix();
		if(f1 > 15)
		{
			isHit1();
			f1 = 0;
			keyStates['i'] = false;
		}
	}
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
}


 
void initialize () 
{
    glMatrixMode(GL_PROJECTION);
	glViewport(0, 0, win.width, win.height);
	GLfloat aspect = (GLfloat) win.width / win.height;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	glFrustum(-1.0,1.0,-1.0,1.0,1.0,500.0);
	//gluPerspective(win.field_of_view_angle, aspect, win.z_near, win.z_far);
    glMatrixMode(GL_MODELVIEW);
    glShadeModel( GL_SMOOTH );
    glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
    glClearDepth( 1.0f );
    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_LEQUAL );
    glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
 
    GLfloat amb_light[] = { 0.1, 0.1, 0.1, 1.0 };
    GLfloat diffuse[] = { 0.6, 0.6, 0.6, 1 };
    GLfloat specular[] = { 0.7, 0.7, 0.3, 1 };
    glLightModelfv( GL_LIGHT_MODEL_AMBIENT, amb_light );
    glLightfv( GL_LIGHT0, GL_DIFFUSE, diffuse );
    glLightfv( GL_LIGHT0, GL_SPECULAR, specular );
    glEnable( GL_LIGHT0 );
    glEnable( GL_COLOR_MATERIAL );
    glShadeModel( GL_SMOOTH );
    glLightModeli( GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE );
    glDepthFunc( GL_LEQUAL );
    glEnable( GL_DEPTH_TEST );
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0); 
}
 
 
void keyboard ( unsigned char key, int x, int y ) 
{
  if(key == KEY_ESCAPE)
	exit(0);
  else
  {
	keyStates[key] = true;
	if(keyStates['i'])
	{
		keyStates['j'] = false;
		keyStates['l'] = false;
	}
	if(keyStates['w'])
	{
		keyStates['a'] = false;
		keyStates['d'] = false;
	}
  }	
}

void keyboardup ( unsigned char key, int x, int y )
{
	if(key == 'w' || key == 'i')
		keyStates[key] = true;
	else
		keyStates[key] = false;
}

int main(int argc, char **argv) 
{
	player1 = argv[1];
	player2 = argv[2];
	// set window values
	win.width = 512;
	win.height = 512;
	win.title = "Battle Of Ships";
	win.field_of_view_angle = 45;
	win.z_near = 1.0f;
	win.z_far = 500.0f;
 
	// initialize and run program
	glutInit(&argc, argv);                                      // GLUT initialization
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );  // Display Mode
	glutInitWindowSize(win.width,win.height);					// set window size
	glutCreateWindow(win.title);								// create Window
	glutDisplayFunc(display);									// register Display Function
	glutIdleFunc(idle);									// register Idle Function
        glutKeyboardFunc( keyboard );
	glutKeyboardUpFunc( keyboardup );								// register Keyboard Handler
	LoadTexture();
	initialize();
	obj.Load("absship.obj");
	obj1.Load("absship.obj");
	glutMainLoop();												// run GLUT mainloop
	return 0;
}
