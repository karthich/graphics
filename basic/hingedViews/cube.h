//===============  OpenGL and GLUT on *ANY* CodeBlocks platform  ===============
//3456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789_
// (J. Tumblin: I like consistent 80-char column width)
//
// multi-platform includes for the GLUT/OpenGL libraries:
#if defined(_WIN32)
#include <GL/freeglut.h>// open-source GLUT functions for windows, keybd, mouse
/*---*OR*--- use:
// #include <GL/glut.h> // for 100% backwards-compatibility with Mark Kilgard's
                        // SGI-copyrighted GLUT library. Careful! this excludes
                        // freeglut's callbacks for mouse wheel, etc.
*/
/*
// freeglut.h already contains these other includes that were necessary for
// using Kilgard's original GLUT library:
#include <GL/GL.h>      // OpenGL 3D graphics rendering functions
#include <GL/Glu.h>     // OpenGL utilties functions
#include <GL/glext.h>   // #defines for all OpenGL features on your machine.
*/
#elif defined(__APPLE__)
#include <GLUT/glut.h>  // GLUT functions for windows, keybd, mouse
#include <OpenGL/GL.h>  // OpenGL 3D graphics rendering functions
#include <OpenGL/Glu.h> // OpenGL utilties functions
#include <OpenGL/GLext.h>   // #defines for all OpenGL features on your machine.
// UNIX, Linux of all flavors;
#else
#include <unistd.h>         // defines symbolic constants, POSIX versions, etc.
#include <GL/glew.h>        // OpenGL 'extensions wrangler' for 'mesa' lib usage
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>
#include <math.h>
#endif


//==============================================================================
#include <iostream>       // for cin, cout, etc.
#include <math.h>         // for sin, cos.
#include "CByteImg.h"




// font strings for drawText2D();
// (use as value for pFont argument)
//-------------------------------
#define rom10 GLUT_BITMAP_TIMES_ROMAN_10
#define rom24 GLUT_BITMAP_TIMES_ROMAN_24
#define helv10 GLUT_BITMAP_HELVETICA_10
#define helv12 GLUT_BITMAP_HELVETICA_12
#define helv18 GLUT_BITMAP_HELVETICA_18

#define NUM_CUBE_FACES 6
#define NUM_VERTICES_PER_FACE 4
#define NUM_VERTICES 8


static int nu_display_width = 640;	// global variables that hold current
static int nu_display_height = 640;	// display window position and size;
static int nu_display_xpos = 200;	// See getDisplayXXX() functions.
static int nu_display_ypos = 100;

//scene properties
int xposScene=0;
int yposScene=0;
int zposScene=0;
int xclik;
int yclik;

GLdouble xtheta=0, ytheta=0;
GLdouble znear = 1.0, zfar = 1000.0;
GLdouble zoom_factor_1=1,zoom_factor_2=1,zoom_factor_3=1,zoom_factor_4=1,zoom_step = 0.1;
GLdouble theta_1=0.0, theta_2=0.0, theta_3=0.0, theta_4=0.0, theta_step= 0.1;
GLdouble *pVerts, *pColrs;


int currentWindow=0;
bool projChange = 0, midbutclick=0;

int rotstep = 1;
int direction = 1;
int pyramidrot = 0;
int conerot = 0;
int cylrot = 0;
int directioncyl = 1;
static bool  nu_Anim_isOn = 1;

GLdouble cam_left=-0.25,cam_right=0.25,cam_top=-0.25,cam_bottom=0.25,cam_step = 0.1;


GLdouble aspect_ratio = 1;
GLdouble orthoLeft = -2;


int getWindowHeight();
int getWindowWidth();
int getDisplayYpos();
int getDisplayXpos();
void drawAxes(void);
void myDisplay();
void myMouse(int button, int state, int xw, int yw);
void myMouseMove(int xw,int yw);
void myMouseClikMove(int xw,int yw);
void drawScene();
void drawGndPlane(GLdouble siz, GLdouble xygap);
int whichWindow(int xw, int yw);
void zoomfunc(int window,int direction);
void rotateWindow(int currentWindow);
void myKeyboard(unsigned char key, int xw, int yw);
void makeTriangleBETTER(void);
void drawTriangleBETTER(void);
void runAnimTimer(int isOn);
void myTimer (int value);
void printHelp();
void myReshape(int height,int width);
double calculateAngle(double size, double distance);
void myKeySpecial(int key, int xw, int yw);

