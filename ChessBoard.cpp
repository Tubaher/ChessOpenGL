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

#define knightObject 1
#define pawnObject 2
#define rookObject 3
#define bishopObject 4
#define queenObject 5
#define kingObject 6
 
// Global variables
int windowWidth  = 1000;     // Initial Screen and viewport width 
int windowHeight = 1000;     // Initil Screen and viewport height

GLfloat zValue=80;
GLfloat xRotated, yRotated, zRotated;
static GLfloat angleTheta[3] = {0.0,0.0,0.0};
static GLfloat movingPiece[3] = {0.0,0.0,0.0};
static GLint axis = 2;
static GLint step = 2;
static GLfloat zRotationPawn = -50.0;
static GLfloat yRotationKnight = 0;
static GLfloat yRotationKing = 0;
static GLfloat zTanslationBishop = -30;
static GLfloat zShearQueen = 1.57079632679;
static GLfloat zScaleRook = 1;




GLfloat MVMatrix[16];
GLfloat MVMatrix2[16];
GLfloat windowXMax, windowXMin, windowYMax, windowYMin; // window bounds

// Projection window/clipping/work area
GLdouble windowAreaXLeft, windowAreaXRight, windowAreaYBottom, windowAreaYTop;
//Para dibujar el peon

GLfloat PawnCurrent[40][3] = {
{1.0011, 16.035, 0.0000},
{1.4773, 15.903, 0.0000},
{1.8476, 15.691, 0.0000},
{2.1651, 15.374, 0.0000},
{2.3238, 14.845, 0.0000},
{2.4825, 14.315, 0.0000},
{2.4561, 13.866, 0.0000},
{2.3767, 13.442, 0.0000},
{2.1651, 13.072, 0.0000},
{1.8212, 12.781, 0.0000},
{1.7153, 12.358, 0.0000},
{1.7153, 11.617, 0.0000},
{2.0593, 11.247, 0.0000},
{2.6942, 11.009, 0.0000},
{2.1651, 10.665, 0.0000},
{1.7418, 10.347, 0.0000},
{1.7947,  9.871, 0.0000},
{1.7947,  9.236, 0.0000},
{1.8476, 8.7074, 0.0000},
{2.1122, 8.1254, 0.0000},
{2.6942, 7.4111, 0.0000},
{2.0000, 7.0000, 0.0000},
{3.4349, 6.5910, 0.0000},
{3.9640, 6.1413, 0.0000},
{4.3873, 5.5857, 0.0000},
{4.5990, 5.7921, 0.0000},
{4.7048, 5.1836, 0.0000},
{4.7048, 4.4164, 0.0000},
{5.0000, 4.0000, 0.0000},
{5.6042, 3.8080, 0.0000},
{5.1863, 3.6492, 0.0000},
{5.7154, 3.3582, 0.0000},
{6.2974, 3.0937, 0.0000},
{6.6942, 2.8291, 0.0000},
{7.0910, 2.4058, 0.0000},
{7.2085, 1.9826, 0.0000},
{7.3259, 1.3741, 0.0000},
{7.4419, 0.8668, 0.0000},
{7.5000, 0.0000, 0.0000},
{0.0000, 0.0000, 0.0000}
};
GLfloat PawnNext[40][3] = {
{1.0011, 16.035, 0.0000},
{1.4773, 15.903, 0.0000},
{1.8476, 15.691, 0.0000},
{2.1651, 15.374, 0.0000},
{2.3238, 14.845, 0.0000},
{2.4825, 14.315, 0.0000},
{2.4561, 13.866, 0.0000},
{2.3767, 13.442, 0.0000},
{2.1651, 13.072, 0.0000},
{1.8212, 12.781, 0.0000},
{1.7153, 12.358, 0.0000},
{1.7153, 11.617, 0.0000},
{2.0593, 11.247, 0.0000},
{2.6942, 11.009, 0.0000},
{2.1651, 10.665, 0.0000},
{1.7418, 10.347, 0.0000},
{1.7947,  9.871, 0.0000},
{1.7947,  9.236, 0.0000},
{1.8476, 8.7074, 0.0000},
{2.1122, 8.1254, 0.0000},
{2.6942, 7.4111, 0.0000},
{2.0000, 7.0000, 0.0000},
{3.4349, 6.5910, 0.0000},
{3.9640, 6.1413, 0.0000},
{4.3873, 5.5857, 0.0000},
{4.5990, 5.7921, 0.0000},
{4.7048, 5.1836, 0.0000},
{4.7048, 4.4164, 0.0000},
{5.0000, 4.0000, 0.0000},
{5.6042, 3.8080, 0.0000},
{5.1863, 3.6492, 0.0000},
{5.7154, 3.3582, 0.0000},
{6.2974, 3.0937, 0.0000},
{6.6942, 2.8291, 0.0000},
{7.0910, 2.4058, 0.0000},
{7.2085, 1.9826, 0.0000},
{7.3259, 1.3741, 0.0000},
{7.4419, 0.8668, 0.0000},
{7.5000, 0.0000, 0.0000},
{0.0000, 0.0000, 0.0000}
};

GLfloat KingCurrent[40][3] = {
{0,28.4614		 ,0.0},
{0.9112, 28.3638 ,0.0},
{1.6017, 27.6734 ,0.0},
{1.6426, 26.8286 ,0.0},
{1.0681, 25.8935 ,0.0},
{2.0674, 25.9025 ,0.0},
{3.0561, 25.0041 ,0.0},
{4.032, 25.9022	 ,0.0},
{5.4708, 24.8098 ,0.0},
{6.8123, 25.6941 ,0.0},
{8, 25			 ,0.0},
{7.0433, 23.3405 ,0.0},
{6.6699, 21.6364 ,0.0},
{5.6625, 20.4928 ,0.0},
{4.607, 19.1918	 ,0.0},
{5.9044, 18.949	 ,0.0},
{4.8478, 17.6315 ,0.0},
{3.6979, 17.3194 ,0.0},
{3.2173, 15.6544 ,0.0},
{3.0748, 14.1988 ,0.0},
{2.9785, 12.7412 ,0.0},
{2.8822, 11.2846 ,0.0},
{3.9662, 10.6243 ,0.0},
{3.3009, 8.9325	 ,0.0},
{3.387, 7.78271	 ,0.0},
{3.5437, 6.6894	 ,0.0},
{3.7041, 5.1836	 ,0.0},
{3.7041, 4.4164	 ,0.0},
{4, 4			 ,0.0},
{4.6046, 3.8080	 ,0.0},
{5.1867, 3.6492	 ,0.0},
{5.7153, 3.3582	 ,0.0},
{6.2974, 3.0937	 ,0.0},
{6.7449, 2.8438	 ,0.0},
{7.060, 2.47349	 ,0.0},
{7.4081, 1.9826	 ,0.0},
{6.9776, 1.3620	 ,0.0},
{7.4429, 0.9357	 ,0.0},
{7.4893, 0.0702	 ,0.0},
{0, 0			 ,0.0},
};

GLfloat KingNext[40][3] = {
{0,28.4614		 ,0.0},
{0.9112, 28.3638 ,0.0},
{1.6017, 27.6734 ,0.0},
{1.6426, 26.8286 ,0.0},
{1.0681, 25.8935 ,0.0},
{2.0674, 25.9025 ,0.0},
{3.0561, 25.0041 ,0.0},
{4.032, 25.9022	 ,0.0},
{5.4708, 24.8098 ,0.0},
{6.8123, 25.6941 ,0.0},
{8, 25			 ,0.0},
{7.0433, 23.3405 ,0.0},
{6.6699, 21.6364 ,0.0},
{5.6625, 20.4928 ,0.0},
{4.607, 19.1918	 ,0.0},
{5.9044, 18.949	 ,0.0},
{4.8478, 17.6315 ,0.0},
{3.6979, 17.3194 ,0.0},
{3.2173, 15.6544 ,0.0},
{3.0748, 14.1988 ,0.0},
{2.9785, 12.7412 ,0.0},
{2.8822, 11.2846 ,0.0},
{3.9662, 10.6243 ,0.0},
{3.3009, 8.9325	 ,0.0},
{3.387, 7.78271	 ,0.0},
{3.5437, 6.6894	 ,0.0},
{3.7041, 5.1836	 ,0.0},
{3.7041, 4.4164	 ,0.0},
{4, 4			 ,0.0},
{4.6046, 3.8080	 ,0.0},
{5.1867, 3.6492	 ,0.0},
{5.7153, 3.3582	 ,0.0},
{6.2974, 3.0937	 ,0.0},
{6.7449, 2.8438	 ,0.0},
{7.060, 2.47349	 ,0.0},
{7.4081, 1.9826	 ,0.0},
{6.9776, 1.3620	 ,0.0},
{7.4429, 0.9357	 ,0.0},
{7.4893, 0.0702	 ,0.0},
{0, 0			 ,0.0},

};



GLfloat KingCrownCurrent[12][3] = {

{-1, 28 ,-2},
{-1, 31 ,-2},
{-3,30.5,-2},
{-3,33.5,-2},
{-1, 33	,-2},
{-1.2,35,-2},
{1.2, 35,-2},
{1, 33	,-2},
{3, 33.5,-2},
{3, 30.5,-2},
{1, 31	,-2},
{1, 28	,-2}

};

GLfloat KingCrownNext[12][3] = {

{-1, 28 ,0},
{-1, 31 ,0},
{-3,30.5,0},
{-3,33.5,0},
{-1, 33	,0},
{-1.2,35,0},
{1.2, 35,0},
{1, 33	,0},
{3, 33.5,0},
{3, 30.5,0},
{1, 31	,0},
{1, 28	,0}

};

//Para dibujar la torre


GLfloat RookCurrent[40][3] = {
{0,   22			,0.0},
{0.5, 22			,0.0},
{0.5, 21			,0.0},
{0.5, 20			,0.0},
{1.5, 20			,0.0},
{1.5, 22			,0.0},
{2.862, 22.020		,0.0},
{2.856, 20.016		,0.0},
{4.285, 19.987		,0.0},
{4.298, 22.007		,0.0},
{6, 22				,0.0},
{6, 21				,0.0},
{6, 20				,0.0},
{6, 19				,0.0},
{5, 18				,0.0},
{4.028, 16.467 		,0.0},
{3, 15				,0.0},
{2.5, 14			,0.0},
{4, 13.5			,0.0},
{2.585439	, 12.32,0.0},
{2.371168	, 10.637,0.0},
{2.585439	, 9.409,0.0},
{3, 8.5				,0.0},
{3.720691	, 7.733,0.0},
{3.5, 6.5			,0.0},
{3.599005	,5.79213,0.0},
{3.704825	, 5.183,0.0},
{3.704825	, 4.416,0.0},
{4, 4				,0.0},
{4.604298	, 3.808,0.0},
{5.186310	, 3.649,0.0},
{5.715412	, 3.358,0.0},
{6.297424	, 3.093,0.0},
{6.744496	, 2.843,0.0},
{7.060298	 ,2.4734,0.0},
{7.408538	, 1.982,0.0},
{6.977994	, 1.362,0.0},
{7.442248	, 0.935,0.0},
{7.489864	, 0.070,0.0},
{0		,0.0	,0.0},
};
GLfloat RookNext[40][3] = {
{0,   22			,0.0},
{0.5, 22			,0.0},
{0.5, 21			,0.0},
{0.5, 20			,0.0},
{1.5, 20			,0.0},
{1.5, 22			,0.0},
{2.862, 22.020		,0.0},
{2.856, 20.016		,0.0},
{4.285, 19.987		,0.0},
{4.298, 22.007		,0.0},
{6, 22				,0.0},
{6, 21				,0.0},
{6, 20				,0.0},
{6, 19				,0.0},
{5, 18				,0.0},
{4.028, 16.467 		,0.0},
{3, 15				,0.0},
{2.5, 14			,0.0},
{4, 13.5			,0.0},
{2.585439	, 12.32,0.0},
{2.371168	, 10.637,0.0},
{2.585439	, 9.409,0.0},
{3, 8.5				,0.0},
{3.720691	, 7.733,0.0},
{3.5, 6.5			,0.0},
{3.599005	,5.79213,0.0},
{3.704825	, 5.183,0.0},
{3.704825	, 4.416,0.0},
{4, 4				,0.0},
{4.604298	, 3.808,0.0},
{5.186310	, 3.649,0.0},
{5.715412	, 3.358,0.0},
{6.297424	, 3.093,0.0},
{6.744496	, 2.843,0.0},
{7.060298	 ,2.4734,0.0},
{7.408538	, 1.982,0.0},
{6.977994	, 1.362,0.0},
{7.442248	, 0.935,0.0},
{7.489864	, 0.070,0.0},
{0 		,0.0	,0.0}
};


//Para dibujar el alfil


GLfloat BishopCurrent[40][3] = {
{0.0,       20.0, 0.0},
{1.3578, 19.7019, 0.0},
{1.4932, 19.2611, 0.0},
{1.2901, 18.8203, 0.0},
{1.9671, 18.3060, 0.0},
{2.5087, 17.5346, 0.0},
{2.9149, 16.6897, 0.0},
{2.8766, 15.6560, 0.0},
{2.4410, 14.8530, 0.0},
{2.0687, 14.1917, 0.0},
{1.8994, 13.4570, 0.0},
{1.9671, 12.8326, 0.0},
{2.2379, 12.4652, 0.0},
{2.6942, 12.0090, 0.0},
{2.1651, 11.6651, 0.0},
{2.0348, 11.2530, 0.0},
{2.0348, 10.8122, 0.0},
{2.1702, 10.2979, 0.0},
{2.2718, 9.78364, 0.0},
{2.4072, 9.19589, 0.0},
{2.6942, 8.41119, 0.0},
{3.0, 8.0        ,0.0},
{3.4349, 7.59108, 0.0},
{3.9640, 7.14134, 0.0},
{4.3873, 6.58579, 0.0},
{4.5990,.7921378, 0.0},
{4.7048, 5.18367, 0.0},
{4.7048, 4.41647, 0.0},
{5.0, 4.0		, 0.0},
{5.6042, 3.80800, 0.0},
{6.1863, 3.64927, 0.0},
{6.7154, 3.358260,0.0},
{7.0974, 3.09371, 0.0},
{7.1942, 2.82916, 0.0},
{7.0602, 2.47349, 0.0},
{7.2085, 1.98260, 0.0},
{7.2988, 1.40819, 0.0},
{7.2649, 0.71023, 0.0},
{7.9326, 0.08575, 0.0},
{0.0, 0.0		, 0.0},
};
GLfloat BishopNext[40][3] = {
{0.0,       20.0, 0.0},
{1.3578, 19.7019, 0.0},
{1.4932, 19.2611, 0.0},
{1.2901, 18.8203, 0.0},
{1.9671, 18.3060, 0.0},
{2.5087, 17.5346, 0.0},
{2.9149, 16.6897, 0.0},
{2.8766, 15.6560, 0.0},
{2.4410, 14.8530, 0.0},
{2.0687, 14.1917, 0.0},
{1.8994, 13.4570, 0.0},
{1.9671, 12.8326, 0.0},
{2.2379, 12.4652, 0.0},
{2.6942, 12.0090, 0.0},
{2.1651, 11.6651, 0.0},
{2.0348, 11.2530, 0.0},
{2.0348, 10.8122, 0.0},
{2.1702, 10.2979, 0.0},
{2.2718, 9.78364, 0.0},
{2.4072, 9.19589, 0.0},
{2.6942, 8.41119, 0.0},
{3.0, 8.0        ,0.0},
{3.4349, 7.59108, 0.0},
{3.9640, 7.14134, 0.0},
{4.3873, 6.58579, 0.0},
{4.5990,.7921378, 0.0},
{4.7048, 5.18367, 0.0},
{4.7048, 4.41647, 0.0},
{5.0, 4.0		, 0.0},
{5.6042, 3.80800, 0.0},
{6.1863, 3.64927, 0.0},
{6.7154, 3.358260,0.0},
{7.0974, 3.09371, 0.0},
{7.1942, 2.82916, 0.0},
{7.0602, 2.47349, 0.0},
{7.2085, 1.98260, 0.0},
{7.2988, 1.40819, 0.0},
{7.2649, 0.71023, 0.0},
{7.9326, 0.08575, 0.0},
{0.0, 0.0		, 0.0},
};


//Para dibujar la reina


GLfloat QueenCurrent[40][3] = {
{0,28.4614		 ,0.0},
{0.9112, 28.3638 ,0.0},
{1.6017, 27.6734 ,0.0},
{1.6426, 26.8286 ,0.0},
{1.0681, 25.8935 ,0.0},
{2.0674, 25.9025 ,0.0},
{3.0561, 25.0041 ,0.0},
{4.032, 25.9022	 ,0.0},
{5.4708, 24.8098 ,0.0},
{6.0123, 25.6941 ,0.0},
{5.5, 25			 ,0.0},
{5.2433, 23.3405 ,0.0},
{5.1699, 21.6364 ,0.0},
{5.0625, 20.4928 ,0.0},
{4.207, 19.1918	 ,0.0},
{5.4044, 18.949	 ,0.0},
{4.7478, 17.6315 ,0.0},
{3.4979, 17.3194 ,0.0},
{3.2173, 15.6544 ,0.0},
{3.0748, 14.1988 ,0.0},
{2.9785, 12.7412 ,0.0},
{2.8822, 11.2846 ,0.0},
{3.9662, 10.6243 ,0.0},
{3.3009, 8.9325	 ,0.0},
{3.387, 7.78271	 ,0.0},
{3.5437, 6.6894	 ,0.0},
{3.7041, 5.1836	 ,0.0},
{3.7041, 4.4164	 ,0.0},
{4, 4			 ,0.0},
{4.6046, 3.8080	 ,0.0},
{5.1867, 3.6492	 ,0.0},
{5.7153, 3.3582	 ,0.0},
{6.2974, 3.0937	 ,0.0},
{6.7449, 2.8438	 ,0.0},
{7.060, 2.47349	 ,0.0},
{7.8081, 1.9826	 ,0.0},
{6.9776, 1.3620	 ,0.0},
{7.9429, 0.9357	 ,0.0},
{7.9893, 0.0702	 ,0.0},
{0, 0			 ,0.0},
};

GLfloat QueenNext[40][3] = {
{0,28.4614		 ,0.0},
{0.9112, 28.3638 ,0.0},
{1.6017, 27.6734 ,0.0},
{1.6426, 26.8286 ,0.0},
{1.0681, 25.8935 ,0.0},
{2.0674, 25.9025 ,0.0},
{3.0561, 25.0041 ,0.0},
{4.032, 25.9022	 ,0.0},
{5.4708, 24.8098 ,0.0},
{6.8123, 25.6941 ,0.0},
{8, 25			 ,0.0},
{7.0433, 23.3405 ,0.0},
{6.6699, 21.6364 ,0.0},
{5.6625, 20.4928 ,0.0},
{4.607, 19.1918	 ,0.0},
{5.9044, 18.949	 ,0.0},
{4.8478, 17.6315 ,0.0},
{3.6979, 17.3194 ,0.0},
{3.2173, 15.6544 ,0.0},
{3.0748, 14.1988 ,0.0},
{2.9785, 12.7412 ,0.0},
{2.8822, 11.2846 ,0.0},
{3.9662, 10.6243 ,0.0},
{3.3009, 8.9325	 ,0.0},
{3.387, 7.78271	 ,0.0},
{3.5437, 6.6894	 ,0.0},
{3.7041, 5.1836	 ,0.0},
{3.7041, 4.4164	 ,0.0},
{4, 4			 ,0.0},
{4.6046, 3.8080	 ,0.0},
{5.1867, 3.6492	 ,0.0},
{5.7153, 3.3582	 ,0.0},
{6.2974, 3.0937	 ,0.0},
{6.7449, 2.8438	 ,0.0},
{7.060, 2.47349	 ,0.0},
{7.4081, 1.9826	 ,0.0},
{6.9776, 1.3620	 ,0.0},
{7.4429, 0.9357	 ,0.0},
{7.4893, 0.0702	 ,0.0},
{0, 0			 ,0.0},
};


 GLfloat knightCurrent[61][3]={
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
  {8.000,  0.00 , 0.0},
  {0.020,  0.00 , 0.0}
};

GLfloat knightNext[61][3]={
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
  {8.000,  0.00 , 0.0},
  {0.020,  0.00 , 0.0}
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
	int nSlices=6;
	double dz=1;
	int nPoints=61;
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
			glVertex3f(knightNext[j][0],knightNext[j][1],knightNext[j][2]);
			
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

  GLfloat rectScannigCurrent[5][3]={
    {-8,00.00,0.00},
    {8.000,0.00,0.00},
    {8.000,0.00,6.00},
    {-8.00,0.00,6.00},
    {-8.00,0.00,0.00},
  };
  GLfloat rectScannigNext[5][3]={
    {-8,00.00,0.00},
    {8.000,0.00,0.00},
    {8.000,0.00,6.00},
    {-8.00,0.00,6.00},
    {-8.00,0.00,0.00},
  };

  nSlices = 5;
  double dy = 1;
  nPoints = 5;

  	for (int i=0; i<nSlices;i++)
	{
		
    		//glVertex3f(x,y,z);
		glColor3f(0.5f,0.5f,0.5f);
		
		glBegin(GL_LINE_STRIP); 
		for (int j=0; j<nPoints; j++)
		{
			double x=rectScannigCurrent[j][0];
			double y=rectScannigCurrent[j][1];
			double z=rectScannigCurrent[j][2];

			glVertex3f(x,y,z);
			
			double yNext=y+dy;
			//glVertex3f(xNext,yNext,zNext);

			rectScannigNext[j][1]=yNext;
			
		}
		glEnd(); 
		for (int j=1; j<nPoints; j++)
		{
		   glColor3f(0.5f,0.5f,0.5f);
		   glBegin(GL_POLYGON); 
					
			
			glVertex3f(rectScannigNext[j][0],rectScannigNext[j][1],rectScannigNext[j][2]);	
			glVertex3f(rectScannigCurrent[j][0],rectScannigCurrent[j][1],rectScannigCurrent[j][2]);		
			glVertex3f(rectScannigCurrent[j-1][0],rectScannigCurrent[j-1][1],rectScannigCurrent[j-1][2]);				
			glVertex3f(rectScannigNext[j-1][0],rectScannigNext[j-1][1],rectScannigNext[j-1][2]);
			glVertex3f(rectScannigNext[j][0],rectScannigNext[j][1],rectScannigNext[j][2]);
			
		   glEnd();
		}
	
		for (int j=0; j<nPoints; j++)
		{
			rectScannigCurrent[j][0]=rectScannigNext[j][0];
			rectScannigCurrent[j][1]=rectScannigNext[j][1];
			rectScannigCurrent[j][2]=rectScannigNext[j][2];
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

//*** Funciones para dibujar ***

void DrawQueen(void)
{

	int nSlices = 1000;
	double dTheta = 2.0*PI / nSlices;
	int nPoints = 40;
	
	for (int i = 0; i < nSlices; i++)
	{
		double theta = i * dTheta;
		double theta_next = (i + 1)*dTheta;


		//glVertex3f(x,y,z);
		glColor3f(0.0f, 1.0f, 0.0f);

		glBegin(GL_LINE_STRIP);
		for (int j = 0; j < nPoints; j++)
		{
			double x = QueenCurrent[j][0];
			double y = QueenCurrent[j][1];
			double z = QueenCurrent[j][2];
			//glVertex3f(Points[j][0]*cos(theta)+Points[j][2]*sin(theta),Points[j][1],-Points[j][0]*sin(theta)+Points[j][2]*cos(theta));
			glVertex3f(x, y, z);

			double xNext = QueenCurrent[j][0] * cos(theta_next) + QueenCurrent[j][2] * sin(theta_next);
			double yNext = QueenCurrent[j][1];
			double zNext = -QueenCurrent[j][0] * sin(theta_next) + QueenCurrent[j][2] * cos(theta_next);
			//glVertex3f(xNext,yNext,zNext);

			QueenNext[j][0] = xNext;
			QueenNext[j][1] = yNext;
			QueenNext[j][2] = zNext;


		}
		glEnd();

		for (int j = 0; j < nPoints; j++)
		{
			QueenCurrent[j][0] = QueenNext[j][0];
			QueenCurrent[j][1] = QueenNext[j][1];
			QueenCurrent[j][2] = QueenNext[j][2];
		}
	}
	glFlush();
}
	


void DrawBishop(void)
{

	int nSlices = 1000;
	double dTheta = 2.0*PI / nSlices;
	int nPoints = 40;
	
	for (int i = 0; i < nSlices; i++)
	{
		double theta = i * dTheta;
		double theta_next = (i + 1)*dTheta;


		//glVertex3f(x,y,z);
		glColor3f(0.0f, 1.0f, 0.0f);

		glBegin(GL_LINE_STRIP);
		for (int j = 0; j < nPoints; j++)
		{
			double x = BishopCurrent[j][0];
			double y = BishopCurrent[j][1];
			double z = BishopCurrent[j][2];
			//glVertex3f(Points[j][0]*cos(theta)+Points[j][2]*sin(theta),Points[j][1],-Points[j][0]*sin(theta)+Points[j][2]*cos(theta));
			glVertex3f(x, y, z);

			double xNext = BishopCurrent[j][0] * cos(theta_next) + BishopCurrent[j][2] * sin(theta_next);
			double yNext = BishopCurrent[j][1];
			double zNext = -BishopCurrent[j][0] * sin(theta_next) + BishopCurrent[j][2] * cos(theta_next);
			//glVertex3f(xNext,yNext,zNext);

			BishopNext[j][0] = xNext;
			BishopNext[j][1] = yNext;
			BishopNext[j][2] = zNext;


		}
		glEnd();

		for (int j = 0; j < nPoints; j++)
		{
			BishopCurrent[j][0] = BishopNext[j][0];
			BishopCurrent[j][1] = BishopNext[j][1];
			BishopCurrent[j][2] = BishopNext[j][2];
		}
	}
	glFlush();
}



void DrawRook(void)
{

	int nSlices = 1000;
	double dTheta = 2.0*PI / nSlices;
	int nPoints = 40;
	
	for (int i = 0; i < nSlices; i++)
	{
		double theta = i * dTheta;
		double theta_next = (i + 1)*dTheta;


		//glVertex3f(x,y,z);
		glColor3f(0.0f, 1.0f, 0.0f);

		glBegin(GL_LINE_STRIP);
		for (int j = 0; j < nPoints; j++)
		{
			double x = RookCurrent[j][0];
			double y = RookCurrent[j][1];
			double z = RookCurrent[j][2];
			//glVertex3f(Points[j][0]*cos(theta)+Points[j][2]*sin(theta),Points[j][1],-Points[j][0]*sin(theta)+Points[j][2]*cos(theta));
			glVertex3f(x, y, z);

			double xNext = RookCurrent[j][0] * cos(theta_next) + RookCurrent[j][2] * sin(theta_next);
			double yNext = RookCurrent[j][1];
			double zNext = -RookCurrent[j][0] * sin(theta_next) + RookCurrent[j][2] * cos(theta_next);
			//glVertex3f(xNext,yNext,zNext);

			RookNext[j][0] = xNext;
			RookNext[j][1] = yNext;
			RookNext[j][2] = zNext;


		}
		glEnd();

		for (int j = 0; j < nPoints; j++)
		{
			RookCurrent[j][0] = RookNext[j][0];
			RookCurrent[j][1] = RookNext[j][1];
			RookCurrent[j][2] = RookNext[j][2];
		}
	}
	glFlush();
}



void DrawPawn(void)
{

	int nSlices = 1000;
	double dTheta = 2.0*PI / nSlices;
	int nPoints = 40;
	for (int i = 0; i < nSlices; i++)
	{
		double theta = i * dTheta;
		double theta_next = (i + 1)*dTheta;


		//glVertex3f(x,y,z);
		glColor3f(0.0f, 1.0f, 0.0f);

		glBegin(GL_LINE_STRIP);
		for (int j = 0; j < nPoints; j++)
		{
			double x = PawnCurrent[j][0];
			double y = PawnCurrent[j][1];
			double z = PawnCurrent[j][2];
			//glVertex3f(Points[j][0]*cos(theta)+Points[j][2]*sin(theta),Points[j][1],-Points[j][0]*sin(theta)+Points[j][2]*cos(theta));
			glVertex3f(x, y, z);

			double xNext = PawnCurrent[j][0] * cos(theta_next) + PawnCurrent[j][2] * sin(theta_next);
			double yNext = PawnCurrent[j][1];
			double zNext = -PawnCurrent[j][0] * sin(theta_next) + PawnCurrent[j][2] * cos(theta_next);
			//glVertex3f(xNext,yNext,zNext);

			PawnNext[j][0] = xNext;
			PawnNext[j][1] = yNext;
			PawnNext[j][2] = zNext;


		}
		glEnd();

		for (int j = 0; j < nPoints; j++)
		{
			PawnCurrent[j][0] = PawnNext[j][0];
			PawnCurrent[j][1] = PawnNext[j][1];
			PawnCurrent[j][2] = PawnNext[j][2];
		}
	}
	glFlush();
}



void DrawKingCrown(void){
  int nSlices=4;
	double dz=PI*0.35;
	int nPoints=12;
	for (int i=0; i<nSlices;i++)
	{
		
    		//glVertex3f(x,y,z);
		glColor3f(0.0f, 1.0f, 0.0f);
		
		glBegin(GL_LINE_STRIP); 
		for (int j=0; j<nPoints; j++)
		{
			double x=KingCrownCurrent[j][0];
			double y=KingCrownCurrent[j][1];
			double z=KingCrownCurrent[j][2];

			glVertex3f(x,y,z);
			
			double zNext=z+dz;
			//glVertex3f(xNext,yNext,zNext);

			KingCrownNext[j][2]=zNext;
			
		}
		glEnd(); 
		for (int j=1; j<nPoints; j++)
		{
		   glColor3f(0.0f, 1.0f, 0.0f);
		   glBegin(GL_POLYGON); 
					
			
			glVertex3f(KingCrownNext[j][0],KingCrownNext[j][1],KingCrownNext[j][2]);	
			glVertex3f(KingCrownCurrent[j][0],KingCrownCurrent[j][1],KingCrownCurrent[j][2]);		
			glVertex3f(KingCrownCurrent[j-1][0],KingCrownCurrent[j-1][1],KingCrownCurrent[j-1][2]);				
			glVertex3f(KingCrownNext[j-1][0],KingCrownNext[j-1][1],KingCrownNext[j-1][2]);
			
			
		   glEnd();
		}
	
		for (int j=0; j<nPoints; j++)
		{
			KingCrownCurrent[j][0]=KingCrownNext[j][0];
			KingCrownCurrent[j][1]=KingCrownNext[j][1];
			KingCrownCurrent[j][2]=KingCrownNext[j][2];
		}
		//glVertex3f(x,y,z);
}
}

void DrawKingBody(void)
{

	int nSlices = 1000;
	double dTheta = 2.0*PI / nSlices;
	int nPoints = 40;
	for (int i = 0; i < nSlices; i++)
	{
		double theta = i * dTheta;
		double theta_next = (i + 1)*dTheta;


		//glVertex3f(x,y,z);
		glColor3f(0.0f, 1.0f, 0.0f);

		glBegin(GL_LINE_STRIP);
		for (int j = 0; j < nPoints; j++)
		{
			double x = KingCurrent[j][0];
			double y = KingCurrent[j][1];
			double z = KingCurrent[j][2];
			//glVertex3f(Points[j][0]*cos(theta)+Points[j][2]*sin(theta),Points[j][1],-Points[j][0]*sin(theta)+Points[j][2]*cos(theta));
			glVertex3f(x, y, z);

			double xNext = KingCurrent[j][0] * cos(theta_next) + KingCurrent[j][2] * sin(theta_next);
			double yNext = KingCurrent[j][1];
			double zNext = -KingCurrent[j][0] * sin(theta_next) + KingCurrent[j][2] * cos(theta_next);
			//glVertex3f(xNext,yNext,zNext);

			KingNext[j][0] = xNext;
			KingNext[j][1] = yNext;
			KingNext[j][2] = zNext;


		}
		glEnd();

		for (int j = 0; j < nPoints; j++)
		{
			KingCurrent[j][0] = KingNext[j][0];
			KingCurrent[j][1] = KingNext[j][1];
			KingCurrent[j][2] = KingNext[j][2];
		}
	}
	glFlush();
}

void DrawKing(void){
  DrawKingCrown();
  DrawKingBody();
}

void initGL() 
{
   glClearColor(0.0, 0.0, 0.0, 1.0); //   Set background (clear) color to black

   //glEnable(GL_LIGHTING);   //TODO: UNCOMMENT
   //glEnable(GL_LIGHT0);     //TODO: UNCOMMENT
  glNewList(knightObject, GL_COMPILE);	
    Knight();
  glEndList();
  glNewList(pawnObject, GL_COMPILE);	
    DrawPawn();
  glEndList();
   glNewList(rookObject, GL_COMPILE);	
    DrawRook();
  glEndList();
  glNewList(bishopObject, GL_COMPILE);	
    DrawBishop();
  glEndList();
  glNewList(queenObject, GL_COMPILE);	
    DrawQueen();
  glEndList();
  glNewList(kingObject, GL_COMPILE);	
    DrawKing();
  glEndList();
}

/* Callback handler for window re-paint event */
void display() 
{
   
   glClear (GL_COLOR_BUFFER_BIT); /*   clear screen */
   glMatrixMode(GL_MODELVIEW);    //   To operate on the model-view matrix
   glLoadIdentity();              //   Reset model-view matrix
   
   
   gluLookAt(0.0,300.0,zValue,  0.0,0.0,0.0,   0.0,1.0,0.0);      //  Define camera settings
   /* glPushMatrix();
   glGetFloatv(GL_MODELVIEW_MATRIX,MVMatrix); */
   glGetFloatv(GL_MODELVIEW_MATRIX,MVMatrix2);
   glRotatef(angleTheta[0], 1.0, 0.0, 0.0);
   glRotatef(angleTheta[1], 0.0, 1.0, 0.0);
   board();
   /* glTranslatef(movingPiece[0], 0.0, 0.0); 
   glTranslatef(0.0, 0.0, movingPiece[2]); */
  
    

  //visualizar valores a trasladar
  /* for (int i=0; i<16;i++)
	{
		printf("%f\n", movingPiece[0]);
    printf("%f\n", movingPiece[2]);
	}
  printf("Hasta aquí un valor\n"); */
  //print matrix
	/* glGetFloatv(GL_MODELVIEW_MATRIX,MVMatrix);
	for (int i=0; i<16;i++)
	{
		printf("%f\n", MVMatrix[i]);
	}
  printf("Hasta aquí una matriz\n");
  //glPushMatrix(); */
  glPushMatrix();
  glGetFloatv(GL_MODELVIEW_MATRIX,MVMatrix);
  glPopMatrix();
  GLfloat transPawn[16]={
    1.00000000,
    0.00000000,
    0.00000000,
    0.00000000,
    0.00000000,
    1.00000000,
    0.00000000,
    0.00000000,
    0.00000000,
    0.00000000,
    1.00000000,
    0.00000000,
  -70.00000000,
    0.00000000,
 zRotationPawn,
    1.00000000
  };
  glMultMatrixf(transPawn);
  glCallList(pawnObject);        //   Draw obj

  glLoadIdentity();               //Cargo de nuevo la matriz identidad
  glMultMatrixf(MVMatrix);        //Regreso al punto que guardé
  GLfloat transKnight[16]={
    1.00000000,
    0.00000000,
    0.00000000,
    0.00000000,
    0.00000000,
    1.00000000,
    0.00000000,
    0.00000000,
    0.00000000,
    0.00000000,
    1.00000000,
    0.00000000,
  -50.00000000,
    0.00000000,
  -70.00000000,
    1.00000000
  };
  glMultMatrixf(transKnight);

  GLfloat transRotationKnight[16]={
    cos(yRotationKnight),
    0.00000000,
    -sin(yRotationKnight),
    0.00000000,
    0.00000000,
    1.00000000,
    0.00000000,
    0.00000000,
    sin(yRotationKnight),
    0.00000000,
    cos(yRotationKnight),
    0.00000000,
    0.00000000,
    0.00000000,
    0.00000000,
    1.00000000
  };
  glMultMatrixf(transRotationKnight);
  glCallList(knightObject);        //   Draw obj

  glLoadIdentity();               //Cargo de nuevo la matriz identidad
  glMultMatrixf(MVMatrix);        //Regreso al punto que guardé
  GLfloat transKing[16]={
    1.00000000,
    0.00000000,
    0.00000000,
    0.00000000,
    0.00000000,
    1.00000000,
    0.00000000,
    0.00000000,
    0.00000000,
    0.00000000,
    1.00000000,
    0.00000000,
   10.00000000,
    0.00000000,
  -70.00000000,
    1.00000000
  };
  glMultMatrixf(transKing);
  
  GLfloat transRotationKing[16]={
	cos(yRotationKing),
    0.00000000,
    -sin(yRotationKing),
    0.00000000,
    0.00000000,
    1.00000000,
    0.00000000,
    0.00000000,
	sin(yRotationKing),
    0.00000000,
	cos(yRotationKing),
    0.00000000,
    0.00000000,
    0.00000000,
	  0.00000000,
    1.00000000
  };
  glMultMatrixf(transRotationKing);

  glCallList(kingObject);        //   Draw obj

  glLoadIdentity();               //Cargo de nuevo la matriz identidad
  glMultMatrixf(MVMatrix);        //Regreso al punto que guardé
  GLfloat transQueen[16]={
    1.00000000,
    0.00000000,
    0.00000000,
    0.00000000,
    0.00000000,
    1.00000000,
    0.00000000,
    0.00000000,
    0.00000000,
    0.00000000,
    1.00000000,
    0.00000000,
  -10.00000000,
    0.00000000,
  -70.00000000,
    1.00000000
  };
  glMultMatrixf(transQueen);

  GLfloat ShearQueen[16]={
    1.00000000,
    0.00000000,
    0.00000000,
    0.00000000,
    1.0/tan(zShearQueen),
    1.00000000,
    0.00000000,
    0.00000000,
    0.00000000,
    0.00000000,
    1.00000000,
    0.00000000,
    0.00000000,
    0.00000000,
    0.00000000,
    1.00000000
  };
  glMultMatrixf(ShearQueen);
  glCallList(queenObject);        //   Draw obj

  glLoadIdentity();               //Cargo de nuevo la matriz identidad
  glMultMatrixf(MVMatrix);        //Regreso al punto que guardé
  GLfloat transBishop[16]={
    1.00000000,
    0.00000000,
    0.00000000,
    0.00000000,
    0.00000000,
    1.00000000,
    0.00000000,
    0.00000000,
    0.00000000,
    0.00000000,
    1.00000000,
    0.00000000,
    zTanslationBishop,
    0.00000000,
    zTanslationBishop -40,
    1.00000000
  };
  glMultMatrixf(transBishop);
  glCallList(bishopObject);        //   Draw obj

  glLoadIdentity();               //Cargo de nuevo la matriz identidad
  glMultMatrixf(MVMatrix);        //Regreso al punto que guardé
  GLfloat transRook[16]={
    1.00000000,
    0.00000000,
    0.00000000,
    0.00000000,
    0.00000000,
    1.00000000,
    0.00000000,
    0.00000000,
    0.00000000,
    0.00000000,
    1.00000000,
    0.00000000,
   70.00000000,
    0.00000000,
  -70.00000000,
    1.00000000
  };
  glMultMatrixf(transRook);
   GLfloat ScaleRook[16]={
    zScaleRook,
    0.00000000,
    0.00000000,
    0.00000000,
    0.00000000,
	 zScaleRook,
    0.00000000,
    0.00000000,
    0.00000000,
    0.00000000,
	 zScaleRook,
    0.00000000,
    0.00000000,
    0.00000000,
    0.00000000,
    1.00000000
  };
  glMultMatrixf(ScaleRook);
  glCallList(rookObject);        //   Draw obj

  glLoadIdentity();
  glMultMatrixf(MVMatrix2);  //regreso de nuevo al punto

  
  //glPopMatrix();
  
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

/* GLUT_KEY_LEFT
Left directional key.
GLUT_KEY_UP
Up directional key.
GLUT_KEY_RIGHT
Right directional key.
GLUT_KEY_DOWN
Down directional key. */

void keyboard(int key, int x, int y){
  if(key==GLUT_KEY_RIGHT || key==GLUT_KEY_LEFT){
    step = 0;
    if(key==GLUT_KEY_RIGHT) movingPiece[step] += 2.0;
    if(key==GLUT_KEY_LEFT) movingPiece[step] -= 2.0;
  }
  if(key==GLUT_KEY_UP || key==GLUT_KEY_DOWN){
    step = 2;
    if(key==GLUT_KEY_DOWN) movingPiece[step] += 2.0;
    if(key==GLUT_KEY_UP) movingPiece[step] -= 2.0;
  }

	display();

}
void keyboard_2(unsigned char key, int x, int y){
  switch (key){
		case 'p':
			zRotationPawn += 5.0;
      if (zRotationPawn == 80)
				zRotationPawn = -50.0;
			break;

    case 'c':
			yRotationKnight += PI * 0.35;
			break;

    case 'r':
			zScaleRook += 0.5;
			if (zScaleRook == 3.0)
			zScaleRook = 1;

		break;


    case 'k':
			yRotationKing += 10.0;
			break;

    case 'b':
			zTanslationBishop += 20;
		if (zTanslationBishop == 90)
			zTanslationBishop = -30;
			
		break;
    
    case 'q':
			zShearQueen += PI*0.35;
		break;

		default:
         break;
	}

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
   //glutSpecialFunc(keyboard);
   glutKeyboardFunc(keyboard_2);
   initGL();                     // Our own OpenGL initialization
   glutMainLoop();               // Enter event-processing loop
   return 1;
}
