/****************************************************************************/
/* Prac. 5A Polygonal mesh with triangles                                        */
/****************************************************************************/

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
// OSX systems need their own headers
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/glext.h>
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/freeglut.h>
#endif
// Use of degrees is deprecated. Use radians for GLM functions
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <time.h>

#define PI 3.14159265f

#define Object 1
 
// Global variables
int windowWidth  = 1000;     // Initial Screen and viewport width 
int windowHeight = 1000;     // Initil Screen and viewport height

GLfloat zValue=80;
GLfloat xRotated, yRotated, zRotated;
static GLfloat angleTheta[] = {0.0,0.0,0.0};
static GLint axis = 2;

GLfloat windowXMax, windowXMin, windowYMax, windowYMin; // window bounds

// Projection window/clipping/work area
GLdouble windowAreaXLeft, windowAreaXRight, windowAreaYBottom, windowAreaYTop;

 GLfloat knightCurrent[60][3]={
  {0.020,  0.00 , 0.0},
  {-7.00,  0.00 , 0.0},
  {-7.00,  1.00 , 0.0},
  {-6.69,  1.97 , 0.0},
  {-6.18,  2.60 , 0.0},
  {-5.68,  2.99 , 0.0},
  {-5.13,  3.42 , 0.0},
  {-4.51,  3.77 , 0.0},
  {-4.00,  4.00 , 0.0},
  {-4.70,  4.82 , 0.0},
  {-5.60,  5.41 , 0.0},
  {-6.26,  6.15 , 0.0},
  {-7.32,  6.62 , 0.0},
  {-6.69,  8.49 , 0.0},
  {-6.93,  9.78 , 0.0},
  {-7.82, 10.48 , 0.0},
  {-6.81, 12.19 , 0.0},
  {-7.55, 13.17 , 0.0},
  {-5.79, 14.96 , 0.0},
  {-6.26, 15.94 , 0.0},
  {-4.66, 16.72 , 0.0},
  {-4.55, 17.62 , 0.0},
  {-2.71, 18.12 , 0.0},
  {-1.23, 18.75 , 0.0},
  {0.170, 18.79 , 0.0},
  {1.110, 18.59 , 0.0},
  {2.050, 18.40 , 0.0},
  {2.240, 17.73 , 0.0},
  {2.320, 17.03 , 0.0},
  {2.940, 16.87 , 0.0},
  {3.610, 16.99 , 0.0},
  {4.270, 16.95 , 0.0},
  {4.580, 16.41 , 0.0},
  {4.540, 15.51 , 0.0},
  {5.170, 14.85 , 0.0},
  {5.830, 14.26 , 0.0},
  {6.260, 13.60 , 0.0},
  {6.410, 12.78 , 0.0},
  {5.000, 13.00 , 0.0},
  {5.870, 12.04 , 0.0},
  {4.500, 11.84 , 0.0},
  {3.450, 11.96 , 0.0},
  {2.590, 11.96 , 0.0},
  {2.000, 12.00 , 0.0},
  {1.770, 11.37 , 0.0},
  {1.810,  10.4 , 0.0},
  {1.770,  9.39 , 0.0},
  {2.200,  8.57 , 0.0},
  {2.670,  7.79 , 0.0},
  {3.220,  7.20 , 0.0},
  {3.680,  6.27 , 0.0},
  {3.960,  5.33 , 0.0},
  {4.000,  4.55 , 0.0},
  {4.000,  4.00 , 0.0},
  {4.850,  3.57 , 0.0},
  {5.830,  3.14 , 0.0},
  {6.730,  2.60 , 0.0},
  {7.350,  1.97 , 0.0},
  {7.940,  1.16 , 0.0},
  {8.000,  0.00 , 0.0}
};

GLfloat knightNext[60][3]={
  {0.020,  0.00 , 0.0},
  {-7.00,  0.00 , 0.0},
  {-7.00,  1.00 , 0.0},
  {-6.69,  1.97 , 0.0},
  {-6.18,  2.60 , 0.0},
  {-5.68,  2.99 , 0.0},
  {-5.13,  3.42 , 0.0},
  {-4.51,  3.77 , 0.0},
  {-4.00,  4.00 , 0.0},
  {-4.70,  4.82 , 0.0},
  {-5.60,  5.41 , 0.0},
  {-6.26,  6.15 , 0.0},
  {-7.32,  6.62 , 0.0},
  {-6.69,  8.49 , 0.0},
  {-6.93,  9.78 , 0.0},
  {-7.82, 10.48 , 0.0},
  {-6.81, 12.19 , 0.0},
  {-7.55, 13.17 , 0.0},
  {-5.79, 14.96 , 0.0},
  {-6.26, 15.94 , 0.0},
  {-4.66, 16.72 , 0.0},
  {-4.55, 17.62 , 0.0},
  {-2.71, 18.12 , 0.0},
  {-1.23, 18.75 , 0.0},
  {0.170, 18.79 , 0.0},
  {1.110, 18.59 , 0.0},
  {2.050, 18.40 , 0.0},
  {2.240, 17.73 , 0.0},
  {2.320, 17.03 , 0.0},
  {2.940, 16.87 , 0.0},
  {3.610, 16.99 , 0.0},
  {4.270, 16.95 , 0.0},
  {4.580, 16.41 , 0.0},
  {4.540, 15.51 , 0.0},
  {5.170, 14.85 , 0.0},
  {5.830, 14.26 , 0.0},
  {6.260, 13.60 , 0.0},
  {6.410, 12.78 , 0.0},
  {5.000, 13.00 , 0.0},
  {5.870, 12.04 , 0.0},
  {4.500, 11.84 , 0.0},
  {3.450, 11.96 , 0.0},
  {2.590, 11.96 , 0.0},
  {2.000, 12.00 , 0.0},
  {1.770, 11.37 , 0.0},
  {1.810,  10.4 , 0.0},
  {1.770,  9.39 , 0.0},
  {2.200,  8.57 , 0.0},
  {2.670,  7.79 , 0.0},
  {3.220,  7.20 , 0.0},
  {3.680,  6.27 , 0.0},
  {3.960,  5.33 , 0.0},
  {4.000,  4.55 , 0.0},
  {4.000,  4.00 , 0.0},
  {4.850,  3.57 , 0.0},
  {5.830,  3.14 , 0.0},
  {6.730,  2.60 , 0.0},
  {7.350,  1.97 , 0.0},
  {7.940,  1.16 , 0.0},
  {8.000,  0.00 , 0.0}
};

void DrawAxis(void){
  glBegin(GL_LINE_STRIP);
  glColor3f(1.0f, 0.0f, 0.0f);
  glVertex3f(0.0, 0.0, 0.0);
  glVertex3f(250.0, 0.0, 0.0); 
  glColor3f(0.3f, 0.0f, 0.0f);
  glVertex3f(-250.0, 0.0, 0.0);
  glVertex3f(0.0, 0.0, 0.0);


  glColor3f(0.0f, 1.0f, 0.0f);
  glVertex3f(0.0, 250.0, 0.0);
  glVertex3f(0.0, 0.0, 0.0);
  glColor3f(0.0f, 0.3f, 0.0f);
  glVertex3f(0.0, -250.0, 0.0);
  glVertex3f(0.0, 0.0, 0.0);


  glColor3f(0.0f, 0.0f, 1.0f);
  glVertex3f(0.0, 0.0, 250.0);
  glVertex3f(0.0, 0.0, 0.0);
  glColor3f(0.0f, 0.0f, 0.3f);
  glVertex3f(0.0, 0.0, -250.0);
  glVertex3f(0.0, 0.0, 0.0);
  glEnd();
}
 
/*normalize a vector*/
void normalize(float v[3]) {    
   GLfloat d = sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]); 
   if (d == 0.0) {
      printf("\nErrot: zero length vector");    
      return;
   }
   v[0] /= d; v[1] /= d; v[2] /= d; 
}

//v1[], v2[] are two vectors
//out[] holds the crossproduct v1 x v2 
void normcrossprod(float v1[3], float v2[3], float out[3]) 
{ 
   GLint i, j; 
   GLfloat length;

   out[0] = v1[1]*v2[2] - v1[2]*v2[1]; 
   out[1] = v1[2]*v2[0] - v1[0]*v2[2]; 
   out[2] = v1[0]*v2[1] - v1[1]*v2[0]; 
   normalize(out); 
}
void board(void) {
	//glClear(GL_COLOR_BUFFER_BIT);
	int counter = 0;
	for (GLint x = -80; x < 80; x += 20)
	{
		for (GLint z = -80; z < 80; z += 20)
		{
			glBegin(GL_POLYGON);
      if (((x + z) / 20) & 1) {
				glColor3f(.6, .6, .6);
			}
			else {
				glColor3f(.2, .2, .2);
			}
      glVertex3f(x,0,z);
      glVertex3f(x+20,0,z);
      glVertex3f(x+20,0,z+20);
      glVertex3f(x,0,z+20);
      glEnd();
		}//end of y		
	}//end of x   

}
void Knight(void)
{  
	int nSlices=4;
	double dz=PI*0.35;
	int nPoints=60;
	for (int i=0; i<nSlices;i++)
	{
		
    		//glVertex3f(x,y,z);
		glColor3f(0.5f,0.5f,0.5f);
		
		glBegin(GL_LINE_STRIP); 
		for (int j=0; j<nPoints; j++)
		{
			double x=knightCurrent[j][0];
			double y=knightCurrent[j][1];
			double z=knightCurrent[j][2];

			glVertex3f(x,y,z);
			
			double zNext=z+dz;
			//glVertex3f(xNext,yNext,zNext);

			knightNext[j][2]=zNext;
			
		}
		glEnd(); 
		for (int j=1; j<nPoints; j++)
		{
		   glColor3f(0.5f,0.5f,0.5f);
		   glBegin(GL_POLYGON); 
					
			
			glVertex3f(knightNext[j][0],knightNext[j][1],knightNext[j][2]);	
			glVertex3f(knightCurrent[j][0],knightCurrent[j][1],knightCurrent[j][2]);		
			glVertex3f(knightCurrent[j-1][0],knightCurrent[j-1][1],knightCurrent[j-1][2]);				
			glVertex3f(knightNext[j-1][0],knightNext[j-1][1],knightNext[j-1][2]);
			
			
		   glEnd();
		}
	
		for (int j=0; j<nPoints; j++)
		{
			knightCurrent[j][0]=knightNext[j][0];
			knightCurrent[j][1]=knightNext[j][1];
			knightCurrent[j][2]=knightNext[j][2];
		}
		//glVertex3f(x,y,z);
	}


        //print matrix
	/* glGetFloatv(GL_MODELVIEW_MATRIX,MVMatrix);
	for (int i=0; i<16;i++)
	{
		printf("%f", MVMatrix[i]);
	} */
	
}

void initGL() 
{
   glClearColor(0.0, 0.0, 0.0, 1.0); //   Set background (clear) color to black

   //glEnable(GL_LIGHTING);   //TODO: UNCOMMENT
   //glEnable(GL_LIGHT0);     //TODO: UNCOMMENT
   glNewList(Object, GL_COMPILE);	
      //DrawIcosahedron();        //    Draw a object,
      board();
      Knight();
   glEndList();
}

/* Callback handler for window re-paint event */
void display() 
{
   
   glClear (GL_COLOR_BUFFER_BIT); /*   clear screen */
   glMatrixMode(GL_MODELVIEW);    //   To operate on the model-view matrix
   glLoadIdentity();              //   Reset model-view matrix
   
   
   gluLookAt(.0,0.0,zValue,  0.0,0.0,0.0,   0.0,1.0,0.0);      //  Define camera settings
   glPushMatrix();
    glRotatef(angleTheta[0], 1.0, 0.0, 0.0);
    glRotatef(angleTheta[1], 0.0, 1.0, 0.0);
   
   
   /* Draw obj */
   
   glCallList(Object);        //   Draw obj   
   glPopMatrix();

   DrawAxis();
   glFlush ();                   //  Render Object
}
 
/* Call back when the windows is re-sized */
void reshape(GLsizei width, GLsizei height) {
   // Compute aspectRatio ratio of the new window
   GLfloat aspectRatio = (GLfloat)width / (GLfloat)height; //Compute aspect ratio
 
   
   glViewport(0, 0, width, height);  // Set the viewport to cover the new screen size
 
   // Set the aspectRatio ratio of the clipping area to match the viewport
   glMatrixMode(GL_PROJECTION);  //   To operate on the Projection matrix
   glLoadIdentity();             //   Reset the projection matrix
   if (width >= height) {
      windowAreaXLeft   = -windowWidth * aspectRatio;
      windowAreaXRight  = windowWidth * aspectRatio;
      windowAreaYBottom = -windowHeight;
      windowAreaYTop    = windowHeight;
   } else {
      windowAreaXLeft   = -windowWidth;
      windowAreaXRight  = windowWidth;
      windowAreaYBottom = -windowHeight / aspectRatio;
      windowAreaYTop    = windowHeight / aspectRatio;
   }
   
   //Adjust vision cone
   //gluOrtho2D(windowAreaXLeft, windowAreaXRight, windowAreaYBottom, windowAreaYTop);
   
   gluPerspective(60.0, GLfloat(width) / GLfloat(height), 0.5, 800.0); //   Define the shape of your viewing volume using glFrustum function
 
   
  
   //Save new windowXMin, windowXMax, windowYMin, windowYMax (or called left, right, bottom, top respectively)
   windowXMin = windowAreaXLeft;
   windowXMax = windowAreaXRight;
   windowYMin = windowAreaYBottom;
   windowYMax = windowAreaYTop;


}

 void mouse(int btn, int state, int x, int y)
{
	if(btn==GLUT_LEFT_BUTTON && state == GLUT_DOWN) axis = 0;
        if(btn==GLUT_RIGHT_BUTTON && state == GLUT_DOWN) axis = 1;
	if(btn==GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) axis = 2;	
	angleTheta[axis] += 2.0;
	if( angleTheta[axis] > 360.0 ) angleTheta[axis] -= 360.0;
	display();
}


/* Main function: GLUT runs as a console application starting at main() */
int main(int argc, char** argv) {
   glutInit(&argc, argv);            // Initialize GLUT
   glutInitWindowSize(windowWidth, windowHeight);  // Initial window width and height
   glutCreateWindow("Chess");      // Create window with given title
   glutDisplayFunc(display);     // Register callback handler for window re-paint
   glutReshapeFunc(reshape); 
   glutMouseFunc(mouse);
   initGL();                     // Our own OpenGL initialization
   glutMainLoop();               // Enter event-processing loop
   return 1;
}
