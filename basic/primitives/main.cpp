#include "properties.h"

using namespace std;



int main(int argc, char *argv[])
{
    glutInit(&argc, argv);  // initialize GLUT/OpenGL, relay any cmd-line args;
	glutInitDisplayMode(GLUT_DOUBLE| GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL);
									// double buffering, use full 32-bit color
									// (don't forget glSwap();show what we drew)

    glutInitWindowSize(windowDisplayWidth,windowDisplayHeight);
    glutInitWindowPosition(0,0);
	glutCreateWindow("EECS 351-1 Northwestern University");
									// Open a window (Microsoft Windows)

	glutDisplayFunc(myDisplay);		// Register the fcn. GLUT calls for drawing
	glutPassiveMotionFunc(myMouseMove); //Register func GLUT calls for passive mouse movement
	if(firstTime)
	{
        help();
	}

	glutKeyboardFunc(myKeyboard);  //Register func GLUT calls for button pressed movement
	//glutReshapeFunc( myReshape);
	glutVisibilityFunc(myHidden);
	glutMouseFunc(myMouse);
    runAnimTimer(1);
 	glutMainLoop();			        // start (endless) message-handling loop.
	return 0;                       // normal exit.
}


void drawSquareEdges(void)
//------------------------------------------------------------------------------
// Draw a 2D square at points +/-1, +/-1. Note counter-clockwise order.
//Code lifted from FANCY_PROGRAM sample code
{
	glBegin(GL_LINE_LOOP);
        glColor3d(1,1,1);
		glVertex2d( 1.0,  1.0);
		glVertex2d(-1.0,  1.0);
		glVertex2d(-1.0, -1.0);
		glVertex2d( 1.0, -1.0);
	glEnd();

}

void drawBasket()
//Simply draws the basket
//Never used in this program PLEASE IGNORE
{
   glBegin(GL_LINE_LOOP);
    glVertex2d(1,1);
    glVertex2d(-1,1);
    glVertex2d(-0.5,-1);
    glVertex2d(0.5,-1);
    glVertex2d(1,1);
    glEnd();
}

void myMouseMove(int xw,int yw)
/*This is the mouse callback function for passive motion of the mouse during the animation
  The function first calculates the mouse co-ordinates converts it to world co-ordinates on
  on the screen. It then generates a direction vector connecting the center of rotation of
  barrel to the mouse co-ordinates. The angle generated now serves as the angle by which the
  barrel of the catapult is rotated. Note that once the left mouse button is clicked this
  function no longer calculates this vector to set barrelRotateAngle and the flag that
  controls it is the boolean barrelAngleSet*/
{
    float worldCoordX = (float)(xw - windowDisplayWidth/2)/(windowDisplayWidth/2),
            worldCoordY = (float)(-1)*(yw - windowDisplayHeight/2)/(windowDisplayHeight/2);
        float slope = (worldCoordY-catapultTranslateY)/(worldCoordX-catapultTranslateX);

    if(!barrelAngleSet)
    {


        barrelRotateAngle = (float)(atanf(slope)*(180/PI))-65;

        if(barrelRotateAngle>BARREL_ROTATE_ANGLE_MAX)
        barrelRotateAngle = BARREL_ROTATE_ANGLE_MAX;

        else if(barrelRotateAngle<BARREL_ROTATE_ANGLE_MIN)
        barrelRotateAngle = BARREL_ROTATE_ANGLE_MIN;

       // cout<<worldCoordX<<","<<worldCoordY<<endl;
    }
    //cout<<worldCoordX<<","<<worldCoordY<<endl;
    //(catapultShoot)
    //projectileMotion();
    glutPostRedisplay();
}

void myMouseClikMove(int xw,int yw)
/* This is the callback function for mouse clicked and move (mouse dragging i.e)
    This program controls the movement of the hand's angle. The same method as mouseMove
    is used to calculate the handRotateAngle which controls the rotation angle of the
    catapult's hand. Note that once the catapultShoot flag has been activated, this
    function doesnt calculate the rotate angle*/
{

    if(!catapultShoot)
    {
        float worldCoordX = (float)(xw - windowDisplayWidth/2)/(windowDisplayWidth/2),
            worldCoordY = (float)(-1)*(yw - windowDisplayHeight/2)/(windowDisplayHeight/2);

        float slope = (worldCoordY-catapultTranslateY)/(worldCoordX-catapultTranslateX);
        handRotateAngle = (float)(atanf(slope)*(180/PI))-45;

        //cout<<handRotateAngle<<endl;

        if(handRotateAngle>HAND_ROTATE_ANGLE_MAX)
        handRotateAngle = HAND_ROTATE_ANGLE_MAX;

        else if(handRotateAngle<HAND_ROTATE_ANGLE_MIN)
        handRotateAngle = HAND_ROTATE_ANGLE_MIN;

        glutPostRedisplay();
        //cout<<gunRotateAngle<<endl;
    }

}

void myMouse(int button, int state, int x, int y)
/* This program is the callback function for the click of a mouse button.
    This is run in the following situations and does the following actions:
    * When the LEFT MOUSE BUTTON is pressed down, it registers the click and drag
    callback function
    * When the LEFT MOUSE BUTTON is released, it registers the barrelAngleSet flag.
    *When the RIGHT MOUSE BUTTON is released, it starts the shooting of the projectile
    */
{
    if(button == GLUT_LEFT_BUTTON&&state == GLUT_DOWN)
    {
        glutMotionFunc(myMouseClikMove);
    }
    else if(button == GLUT_LEFT_BUTTON&&state == GLUT_UP)
    {
        if(!barrelAngleSet)
            barrelAngleSet = 1;

    }

    if(button == GLUT_RIGHT_BUTTON&&state == GLUT_DOWN)
    {
        //shoot the ball
        catapultShoot = 1;
        initProjVelX = INITIAL_VELOCITY*cos(-barrelRotateAngle*PI/180);
        initProjVelY = INITIAL_VELOCITY * sin(-barrelRotateAngle*PI/180);

        cout<<"Vx = "<<initProjVelX<<endl;
        cout<<"Vy = "<<initProjVelY<<endl;
        cout<<"projection angle= "<<-barrelRotateAngle<<endl;



    }
    glutPostRedisplay();
}


void myDisplay()
/* This is the display callback function registered in the main Loop*/
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //Draws the catapult primitive at the position defined by catapultTranslateX,catapultTranslateY
    // and scales it by 0.5
    glPushMatrix();
        glScaled(0.5,0.5,1);
        glTranslated(catapultTranslateX,catapultTranslateY,0);
        drawCatapult();
    glPopMatrix();
    //prints the animation help on the screen
    drawHelp();
    //if the catapult motion has been put in motion
    if(catapultShoot)
    {
        // This entire push pop set moves the axes to the top of the
        //hand of the catapult. Then it calls projectile motion function to run the update
        // the new coordinate of the projectile
        glPushMatrix();
            glColor3d(1,1,1);
            glTranslated(-0.4,-0.7,0);
            glRotated(barrelRotateAngle,0,0,1);
            glTranslated(-(barrelLength+projectileScale),0,0);
            glTranslated(-barrelBreadth/2,barrelBreadth/2,0);
            glRotated(360-barrelRotateAngle,0,0,1);
            glTranslated(projectileX,projectileY,0);
            //drawAxes();
            glScaled(projectileScale,projectileScale,1);
            drawCircle();
        glPopMatrix();
        projectileMotion();
       // cout<<blah <<" "<<blah1<<endl;
        //cout<<"barrelRotateAngle = "<<barrelRotateAngle<<"handRotateAngle ="<<handRotateAngle<<endl;
    }
    //draws the animated box at the top of the screen
    glPushMatrix();
        glTranslated(0.5,0.5,0);
        glRotated(boxTheta,0,0,1);
        glScaled(0.05,0.05,0);
        drawSquareEdges();
        boxTheta = (boxTheta+1)%360;
    glPopMatrix();

    glFlush();
    glutSwapBuffers();
}

//This draws the barrel part of the catapult
void drawBarrel()
{
    glPushMatrix();
        glScaled(0.7,0.1,1);
        drawSquareEdges();
        glTranslated(-1,-0.95,0);
        glScaled(1,4,0);
        //drawAxes();
        glRotated(handRotateAngle,0,0,1);
        //drawAxes();
        drawHand();
    glPopMatrix();


}

//draws the two wheels of the catapult
void drawWheel()
{
    glPushMatrix();
    glScaled(0.15,0.15,1);
    drawCircle();
    glPopMatrix();
}
//Draws the hand of the catapult
void drawHand()
{
    glBegin(GL_TRIANGLE_FAN);
    glColor3f((double)(color%255)/255,42.0/255.0,42.0/255.0);
    glVertex2f(0,0);
    glVertex2f(0,0.5);
    glVertex2f(-1,0);
    glEnd();
    color++;
}

//draws the catapult using the above three parts
void drawCatapult()
{
    glPushMatrix();
        glTranslated(0,-0.7,0);
        glScaled(0.8,0.8,1);
        //base of the catapult
        glPushMatrix();
        glScaled(0.7,0.1,0);
        drawSquareEdges();
        glPopMatrix();


        //wheel one
        glPushMatrix();
        glTranslated(0.5,-0.1,0);
        drawWheel();
        glPopMatrix();

        //wheel two
        glPushMatrix();
        glTranslated(-0.5,-0.1,0);
        drawWheel();
        glPopMatrix();

        //draw the barrel
        glPushMatrix();

        glRotated(barrelRotateAngle,0,0,1);
        //drawAxes();
        glTranslated(-0.7,0,0);
        drawBarrel();

        glTranslated(0.7,0,0);
        //drawAxes();
        glPopMatrix();

    glPopMatrix();
}

//This is the GLUT registered callback function for keyboard keys pressed during the animation
  // We have discussed this keyboard controls in the user guide of the report

void myKeyboard(unsigned char key, int xw, int yw)
{
    switch(key)
    {
        case 'q': exit(0);
                    break;
        case 'r': resetValues();
                    break;
        case 'p': runAnimTimer(!nu_Anim_isOn);
                    break;
        case 'h': help();
                  break;
        default:cout<<"invalid Key"<<endl;
                break;
    }
    //cout<<"projectileX= "<<projectileX<<",projectileY= "<<projectileY<<endl;
    //cout<<"projection angle= "<<-barrelRotateAngle<<endl;
    glutPostRedisplay();

}

void drawAxes(void)
//------------------------------------------------------------------------------
// Draw small +x,+y,+z axes in red, green, blue:
{
	glColor3d(1.0, 0.0, 0.0);	// red
	glBegin(GL_LINES);
		glVertex3d(0.0,0.0,0.0);	// +x-axis
		glVertex3d(0.4,0.0,0.0);
	glEnd();
	glColor3d(0.0, 1.0, 0.0);	// green
	glBegin(GL_LINES);
		glVertex3d(0.0,0.0,0.0);	// +y-axis
		glVertex3d(0.0,0.4,0.0);
	glEnd();
	glColor3d(0.0, 0.5, 1.0);	// bright blue
	glBegin(GL_LINES);
		glVertex3d(0.0,0.0,0.0);	// +z-axis
		glVertex3d(0.0,0.0,0.4);
	glEnd();
}

//draws the circle used as the wheels of the catapult and projectile
// draws the circle as a set of animated triangle fans all rotated
//over 2*PI
void drawCircle()
{
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(0,0);
    for(int i= 0;i<360;i+=1)
    {

        glVertex2f((double) 0+ sin(i) * 1, (double) 0 + cos(i) * 1);
    }

    glEnd();
}

// This is the part of the program that actively generates new co-ordinates for the projectile,
// the initial velocity has been set in this function, it calculates the instanteous velocity,
// adds it to the displacement, in both of the x and y  axes and then updates it as the new position of the projectile.
void projectileMotion()
{
    if(projectileY>=-0.49)
    {
        float theta = -barrelRotateAngle;
        projectileVelY = initProjVelY - G_VALUE*global_time;
        projectileVelX = initProjVelX;
        //cout<<projectileVelX<<endl;
        projectileX +=projectileVelX;
        projectileY += projectileVelY;
        global_time+=0.01;
        //cout<<projectileX<<","<<projectileY<<endl;
    }
    //glutPostRedisplay();

}

void runAnimTimer(int isOn)
//-----------------------------------------------------------------------------
// Call runAnimTimer(0) to STOP animation;
//      runAnimTimer(1) to START or CONTINUE it.
{
    nu_Anim_isOn = isOn;   // Update global var to the most-recent state;
    if(isOn == 1)          // Start or continue animation?
    {   //YES. ANIMATE!   register a GLUT timer callback:
        glutTimerFunc(NU_PAUSE, myTimer, 1);
        // This registers 'myTimer()' as a timer callback function.
        // GLUT will now pause NU_PAUSE milliseconds (longer if busy drawing),
        // call myTimer(1), and then 'un-register' the callback; it won't call
        // myTimer() again unless we register it again.
        // myTimer(1) checks to be SURE no other 'stop animation' requests
        // arrived (e.g. keyboard input, mouse input, visibility change, etc.)
        // while we waited.  If global variable nu_Anim_isOn is STILL 1,
        // it calls runAnimTimer(1) to continue animation.
    }
    // NO.  STOP animation-- don't register a GLUT timer for myTimer().
}

void myTimer (int value)
//------------------------------------------------------------------------------
// Don't call this function yourself; use runAnimTimer() instead!
// runAnimTimer(1) causes GLUT to wait NU_PAUSE milliseconds (longer if busy)
// and then call this function.
// TRICKY: We allow users to choose very long NU_PAUSE times to see what happens
// in animation.  During GLUT's wait time, user events (keyboard, mouse, or
// window cover/uncover) may make runAnimTimer(0) calls intended to STOP
// animation.  GLUT can't cancel a previously-issued GLUT timer callback, so
// in the TimerCallback() function we check global variable nu_Anim_isON.
{
                                // GLUT finished waiting.
    if(value != nu_Anim_isOn)
        cout << "\n(Anim cancelled during GLUT wait)\n";
    if(nu_Anim_isOn==1)         // Go on with animation?
    {

            glutPostRedisplay();    // tell GLUT to re-draw the screen, and
            runAnimTimer(1);

                // continue with the next time interval.
    }
                                // NO. Do nothing. No further animation.
}

void help()
{

    cout<<"Use mouse to rotate the barrel, Click Left Mouse to rotate Hand. \nNOTE: You cannot change the barrel rotation once u have clicked left mouse button";
    cout<<"\n When ready to shoot press the right mouse button"<<endl;
    cout<<"Press r to restart, p to pause, q to quit"<<endl;

}

//When r is pressed, the function resets all the global variables to their initial values
void resetValues()
{
    barrelAngleSet = 0;

    catapultShoot = 0;
    nu_Anim_isOn = 1;
    firstTime = 1;
    notshot = 1;

    projectileX = 0.0;
    projectileY = 0.0;
    projectileVelX = 0;
    projectileVelY = 0;
    initProjVelX = 0;
    initProjVelY = 0;

    handRotateAngle= 0;
    barrelRotateAngle = -90;
    global_time= 0;
    //glutMainLoop();
    runAnimTimer(1);
    glutPostRedisplay();

}


void myHidden (int isVisible)
//------------------------------------------------------------------------------
// Called by GLUT when our on-screen window gets covered or un-covered. When
// called, it turns off animation and re-display when our display window is
// covered or stowed (not visible). SEE ALSO: runAnimTimer() and myTimer()
{
    if(isVisible == GLUT_VISIBLE)  runAnimTimer(1); // Visible! YES! Animate!
    else runAnimTimer(0);                           // No. Stop animating.
}


void drawText2D(void *pFont, double x0, double y0, const char *pString)
//-----------------------------------------------------------------------------
//  Using the current color and specified font, write the text in 'pString' to
// the display window.  Place the lower left corner of the first line of text
//  at screen position x0,y0 in the current coordinate system.
//      Example:  DrawText2D(helv18, 0.3,0.4 ,"Hello!");
//
// Available fonts:
// helv10 (HELVETICA size 10)   rom10 (TIMES_ROMAN size 10)
// helv12 (HELVETICA size 12)   rom24 (TIMES_ROMAN size 24)
// helv18 (HELVETICA size 18)
{
int i, imax;					// character counters

	glRasterPos2d(x0, y0);		// set text's lower-left corner position
	imax = 1023;				// limit the number of chars we print.
	for(i=0; pString[i] != '\0' && i<imax; i++)	// for each char,
	{
		glutBitmapCharacter(pFont, pString[i]);
	}
}
void drawHelp()
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glPushMatrix();
    glColor3d(1,1,1);
    drawText2D(rom10, 0.6,0.9,"Mouse Move - Control Barrel");
    drawText2D(rom10, 0.6,0.85, "Left Mouse Button - control Hand");
    drawText2D(rom10,0.6,0.80,"Right Mouse Button - Shoot");
    drawText2D(rom10,0.2,0.75," h- help, p-pause, r-reset,q-quit");
    glPopMatrix();

}
