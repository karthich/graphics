
#include <unistd.h>     // defines symbolic constants, POSIX versions, etc.
#include <GL/glew.h>    // OpenGL 'extensions wrangler' (for 'mesa' lib usage)
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>
#include <iostream>
#include <math.h>

//Represents all the constants in the function
#define PI 3.14
#define BARREL_ROTATE_ANGLE_MAX -20.0
#define BARREL_ROTATE_ANGLE_MIN -90.0
#define rom10 GLUT_BITMAP_TIMES_ROMAN_10
#define HAND_ROTATE_ANGLE_MIN 0.50
#define HAND_ROTATE_ANGLE_MAX 20.0
#define ASPECT_RATIO 1.33
#define NU_PAUSE 2

void myDisplay();
void myMouseMove(int xw,int yw);
void myMouseClikMove(int xw,int yw);
void myMouse(int button, int state, int x, int y);
void myKeyboard(unsigned char key, int xw, int yw);
void drawCircle();
void drawAxes();
void drawCatapult();
void drawWheel();
void drawBarrel();
void drawHand();
void projectileMotion();
void myTimer (int value);
void runAnimTimer(int isOn);
void help();
void resetValues();
void myHidden (int isVisible);
void drawHelp();

//void myReshape( int width, int height );

//These are all the rotateAngles of the jointed objects
float handRotateAngle= 0;
float barrelRotateAngle = -90;

//projectile Motion parameters
double projectileX = 0.0;
double projectileY = 0.0;
float global_time = 0;
double projectileScale = 0.03;
double projectileVelX = 0;
double projectileVelY = 0;
double initProjVelX = 0;
double initProjVelY = 0;
const double INITIAL_VELOCITY = 0.001;
const double G_VALUE = 0.0001;

//color of the hand and the window sizes
int color = 0;
int windowDisplayHeight = 600;
int windowDisplayWidth = 800;
int boxTheta = 0;

//All the flags that the program uses toc control the program
bool barrelAngleSet = 0;
bool catapultShoot = 0;
static bool  nu_Anim_isOn = 1;
bool firstTime = 1;
bool notshot = 1;



// Intial positions of the all the parts of the catapult
float catapultTranslateX = -0.8;
float catapultTranslateY = -0.7;


float basketTranslateX = 0.5;
float basketTranslateY = -1;

float ballTranslateX = 0.5;
float ballTranslateY = 0;

float barrelLength = 0.56;
float barrelBreadth = 0.08;

