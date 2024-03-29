//3456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789_
// (JT: I prefer to keep line lengths <=80 chars for easy printing/display).
//
// EECS 351-1 'Starter Code' for transition to GLSL programmable shading
//      3D-modified version of 'glutStartFANCY' given before
//        (animated jointed-arm robot)
//      EXCEPT:
//      --2D robot pieces replaced by 3D glut wire-frame and glut solid prims
//      --Mouse-drag rotates the entire robot around its base.
//      --Added 's' key to toggle between wireframe and solid 3D drawing.
//
//      --Adds 'CProgGLSL' class to load & use a very-basic set of programmable
//          shaders (vertex-shader,fragment-shader) for openGL rendering.
//      --Adds 'CShader' class for file I/0 for these GLSL programmable shaders,
//          it takes care of Windows/MacOS/Linux directory-path differences,
//
//      --Demonstrates how a very-basic vertex shader and very-basic fragment
//          shader can use old (deprecated) fixed-pipeline values for drawing.
//
//      --Changed CodeBlocks settings to put executable (.exe) in your project
//          directory rather than 2 levels down in subdirectory /Debug/bin.
//          (HOW? Project->Properties...-->Build Targets-->Output Filename;
//          change from 'bin\Debug\glutStart.exe' to just 'glutStart.exe'.
//          (NOTE! if you go back to 'bin\Debug\glutStart.exe', be sure you
//          change CShader::setPath() function to remove those 2 extra dir.
//          from the path to your shader files).
//      2012.02.20 J. Tumblin-- changed from 2D-->3D, added 3D ortho camera,
//          added solid/wire toggle to make a better test-bed for writing our
//          first shaders.
//
#include "../include/prog.h"
#include "../include/shapes.h"
#include "../include/lampMatl_01.h"
#include "../include/CTransRot.h"
#include "../include/textures.h"
using namespace std;


GLfloat xs, zs, progtime;
int material_changeflag=0;
GLint xs_loc,
    zs_loc,
    time_loc,
    light_pos_loc,
    light1_loc,
    light2_loc,
    light0_loc,
    material_change_loc,
    tex_loc,
    texIt_loc;

CProgGLSL* p_myGLSL;
CTransRot setModel;			// Mouse/Keyboard settings for model coord system.
CTransRot setCam;			// Mouse/Keyboard settings for camera coord system.
GLuint thisTexture;
int texdraw = 0;

//=====================
// GLOBAL VARIABLES: bad idea, but forced by GLUT's callback fcns.
//                   Do your best to keep the number of global vars low; e.g.
//                   Make a 'CTheApp' class to hold them all, and make just
//                   one global CTheApp object  (e.g. 'myApp').
//=====================
static int nu_display_width  = 640;	// global variables that hold current
static int nu_display_height = 640;	// display window position and size;
static int nu_display_xpos = 200;	// See getDisplayXXX() functions.
static int nu_display_ypos = 100;
static int nu_Anim_isOn = 1;        // ==1 to run animation, ==0 to pause.
                        // !DON'T MESS WITH nu_Anim_isOn; call runAnimTimer().
//========================
// Create global vars for our application:
GLdouble xclik=0.0, yclik=0.0;  // mouse button down position, in pixels
GLdouble xtheta=5.0, ytheta=5.0;// mouse-driven view-rotation angles in degrees.

int isCleared = 1;                  // toggle animation's screen-clear
int isSolid = 0;                    // toggle solid/wireframe drawing.

bool waveIt_loc = 1;
int light1 = 1,light2 = 1,light0 = 1;
GLdouble    theta1=0.0,             // animated rotation angles.
            theta2=0.0,
            theta3=0.0;
GLdouble    thetaStep1=0.4,         // animated rotational velocities
            thetaStep2=0.55,
            thetaStep3=2.1;
GLdouble    len1=1.0,               // animated arm lengths.
            len2=1.0;
CMatl matls[5];
CLight lights[6];



CCube cube;
CPyramid pyr;



         // pointer to our one 'GLSL program' object;
                                    //holds two programmable shaders--a vertex
                                    // shader 'vSh', and a fragment shader 'fSh'
                                    // plus all other openGL state info we need
                                    // to use them for openGL rendering.
                                    // Currently, these shaders just 'pass-thru'
                                    // (deprecated) fixed-pipeline data.

//==============================================================================
int main( int argc, char *argv[] )
//==============================================================================
{
    xs = zs = 1.0f;
    //--GLUT/openGL Setup-------------------
	glutInit( &argc, argv );	    // init GLUT user-interface library;
	glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH  );
                    // set rendering mode to RGB (not RGBA) color,
					// single-buffering GLUT_SINGLE, or
					// double-buffering GLUT_DOUBLE, where you'd need to call
					// glutSwapBuffers() after drawing operations to show
					// on-screen what you draw)
					//  enable depth testing for 3D drawing ('Z-buffering')
	glutInitWindowSize(1024,768 );
    glutInitWindowPosition(getDisplayXpos(),getDisplayYpos() );
                    // set display window size & position from global vars
	glutCreateWindow( "EECS351-1 Project C/D Tests" ); // set window title-bar name
    // And finally (must be AFTER the glutCreateWindow() call):
	glEnable( GL_DEPTH_TEST );			// enable hidden surface removal
// STUDENTS: what happens if you disable GL_DEPTH_TEST?
//	And what happens if display() callback clears the color buffer,
// 	but not the depth buffer? (e.g. what happens if you change
//  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); to
//  glClear( GL_COLOR_BUFFER_BIT)'; ?)
    glEnable(GL_LIGHTING);
	glClearColor( 0, 0, 0, 1 );	    // 'cleared-screen' color: black (0,0,0)
	// Register our callback functions in GLUT:
	glutDisplayFunc( myDisplay );	// callback for display window redraw
	glutReshapeFunc( myReshape);    // callback to create or re-size window
	glutVisibilityFunc(myHidden);   // callback for display window cover/uncover
	glutKeyboardFunc( myKeyboard);  // callback for alpha-numeric keystroke
	glutSpecialFunc ( myKeySpecial);// callback for all others: arrow keys,etc.
	glutMouseFunc( myMouseClik );   // callback for mouse button events
	glutMotionFunc( myMouseMove );  // callback for mouse dragging events

    // The other thing to keep in mind, is we need an openGl context
    // available before we try to make the program.  So we put off making
    // an object of pass through shader till now.  We'll initialize it with
    // the right path, and file names.
    //==============Create GLSL programmable shaders============================
    // Always AFTER 'glutCreateWindow()' because some GLSL commands rely on the
    // openGL 'rendering context' (all the state variables that tie openGL to
    // your particular OS and graphics card). The glutCreateWindow() call
    // forces creation of that 'rendering context' we need.

#if !defined(__APPLE__)
    glewInit();                                 // if we use GLEW (Apple won't).
                                                // we must start its library;
#endif
    // Create one GLSL-program object that will hold our programmable shaders;

    srand ( time(NULL) );
    shaderInit(argv);
    shapeInit();
    matlInit();
    lightInit();
    texInit();

    runAnimTimer(1);                // start our animation loop.
	glutMainLoop();	                // enter GLUT's event-handler; NEVER EXITS.

	delete p_myGLSL;                // orderly exit; delete the object we made.
}


void shaderInit(char **argv) //to get the shader started
{
    p_myGLSL = new CProgGLSL(argv[0],  "shaders/myVertexShader.vsh",
                                        "shaders/myFragShader.fsh");
    p_myGLSL->loadShaders();    // read in the shader files' contents
    p_myGLSL->compileProgram(); // compile and link the program for the GPU,
    p_myGLSL->useProgram();     // tell openGL/GPU to use it!

    time_loc = glGetUniformLocation(p_myGLSL->getProgramID(), "time");
    xs_loc = glGetUniformLocation(p_myGLSL->getProgramID(), "xs");
    zs_loc = glGetUniformLocation(p_myGLSL->getProgramID(), "zs");
    waveIt_loc = glGetUniformLocation(p_myGLSL->getProgramID(),"waveIt");
    light_pos_loc = glGetUniformLocation(p_myGLSL->getProgramID(),"light_pos");
    light1_loc = glGetUniformLocation(p_myGLSL->getProgramID(),"light1");
    light2_loc = glGetUniformLocation(p_myGLSL->getProgramID(),"light2");
    light0_loc = glGetUniformLocation(p_myGLSL->getProgramID(),"light0");
    material_change_loc = glGetUniformLocation(p_myGLSL->getProgramID(),"material_change");
    tex_loc = glGetUniformLocation(p_myGLSL->getProgramID(), "texture_color");
    texIt_loc = glGetUniformLocation(p_myGLSL->getProgramID(),"texIt");

    glUniform1f(xs_loc, 1.0f);
    glUniform1f(zs_loc, 1.0f);
    glUniform1f(time_loc, 0.0f);
    printHelp();
}

void shapeInit()
{
    cube.make();
    pyr.make();

}

void matlInit()
{
    srand(time(NULL));
    for(int i=0;i<5;i++)
    {
        matls[i].createMatl(randRangeInt(1,22));
        matls[i].isFaceted = (bool) randRangeInt(0,1);
        matls[i].isTwoSided = (bool) randRangeInt(0,1);
    }

}

void texInit()
{
    texture = LoadTexture("texture.raw", 256, 256);

}
void lightInit()
{
   //***Create our light sources.          // make pre-defined light sources:
    lights[0].createLamp(LAMP_POINT_KEY,     GL_LIGHT0);
    lights[1].createLamp(LAMP_POINT_L_FILL,  GL_LIGHT1);
    lights[2].createLamp(LAMP_POINT_R_FILL,GL_LIGHT2);


    lights[1].I_pos.row[0] = 2.0f;   // set position of lamp 1; at origin
    lights[1].I_pos.row[1] = 0.0f;
    lights[1].I_pos.row[2] = 0.0f;
    lights[1].I_pos.row[3] = 1.0f; // IMPORTANT! zero-valued 'w' means lamp is
                                    // infinitely far away. w=1.0 for local lights.
    lights[0].I_pos.row[0] = -2.0f;// position our first lamp (already created in
    lights[0].I_pos.row[1] = 0.0f;// myGlutSetup() fcn as LAMP_WHITE_KEY), and
    lights[0].I_pos.row[2] = 0.0f;
    lights[0].I_pos.row[3] = 1.0f;

    lights[2].I_pos.row[0] = 0.0f;   // set position of lamp 1; at origin
    lights[2].I_pos.row[1] = 5.0f;
    lights[2].I_pos.row[2] = 0.0f;
    lights[2].I_pos.row[3] = 1.0f; // IMPORTANT! zero-valued 'w' means lamp is


    lights[3].I_pos.row[0] = 0.0f;   // set position of lamp 1; at origin
    lights[3].I_pos.row[1] = -5.0f;
    lights[3].I_pos.row[2] = 0.0f;
    lights[3].I_pos.row[3] = 1.0f; // IMPORTANT! zero-valued 'w' means lamp is


    lights[4].I_pos.row[0] = 0.0f;   // set position of lamp 1; at origin
    lights[4].I_pos.row[1] = 0.0f;
    lights[4].I_pos.row[2] = 3.0f;
    lights[4].I_pos.row[3] = 1.0f; // IMPORTANT! zero-valued 'w' means lamp is


    lights[5].I_pos.row[0] = 0.0f;   // set position of lamp 1; at origin
    lights[5].I_pos.row[1] = 0.0f;
    lights[5].I_pos.row[2] = -3.0f;
    lights[5].I_pos.row[3] = 1.0f; // IMPORTANT! zero-valued 'w' means lamp is




}



void drawScene()
{
    //glScaled(0.5,0.5,0.5);
    glRotated(xtheta,1,0,0);
    glRotated(ytheta,0,1,0);

    progtime+=0.05;                 // advance the timestep
    glUniform1f(time_loc, progtime);    // send it to the shader as a uniform.
    glUniform1i(light1_loc,light1);
    glUniform1i(light2_loc,light2);
    glUniform1i(light0_loc,light0);

    lights[3].applyLamp();
    lights[4].applyLamp();
    lights[5].applyLamp();
    //glUniform1f(light_number_loc,1);
    glPushMatrix();
        glUniform1i(waveIt_loc,0);
        glTranslated(2,0,0);
        lights[0].applyLamp();
        lights[1].applyLamp();
        matls[0].applyMatl();
        //matls[0].showName();
        glUniform1i(material_change_loc,0);
        glUniform1i(texIt_loc,1);
        glutSolidTeapot(0.6);
    glPopMatrix();


//    glUniform1f(light_number_loc,0);
    glPushMatrix();
        glTranslated(1,1,1);
        setModel.apply_RT_Matrix();
        lights[2].applyLamp();
    glPopMatrix();

    glPushMatrix();
        glUniform1i(waveIt_loc,0);
        glTranslated(0,-1,0);
        //matls[1].applyMatl();
        matls[1].showName();
        glUniform1i(material_change_loc,0);
        glUniform1i(texIt_loc,0);
        pyr.draw();
    glPopMatrix();

    glPushMatrix();

        glUniform1i(waveIt_loc,1);
        matls[2].applyMatl();
        //matls[2].showName();
        glUniform1i(material_change_loc,material_changeflag);
        glUniform1i(texIt_loc,0);
        glutSolidTorus(0.2,0.5,100,300);

    glPopMatrix();

    glPushMatrix();
        glUniform1i(waveIt_loc,0);
        glTranslated(-3,0,0);
        matls[3].applyMatl();
        //matls[3].showName();
        glUniform1i(material_change_loc,0);
        glUniform1i(texIt_loc,0);
        glutSolidCone(0.5,1,100,200);
    glPopMatrix();



}
void myDisplay( void )
//------------------------------------------------------------------------------
// Registered as our GLUT display callback function. GLUT calls us whenever we
// need to re-draw the screen.
// OPENGL HINTS:
//	glRotated(123, 1,-2,1);	// rotate by 170 degrees around a 3D axis vector;
//	glScaled(0.2, 0.3, 0.4);	// shrink along x,y,z axes
//  glTranslated(0.1,0.3,0.5);	// translate along x,y,z axes.
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    //------------------------------SHADER ANIMATION:

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();


   // lights[0].applyLamp();        // use it for lighting.
   glTranslated(0,0,-6);
    setCam.apply_TR_Matrix();	// further translate the coord system by




       // send it to the shader as a uniform.

    glPushMatrix();
    if(!texdraw)
      drawScene();
    else
        cube1();



    //cube1();
    glPopMatrix();

	glFlush();	                // do any and all pending openGL rendering.
	glutSwapBuffers();			// For double-buffering: show what we drew.
}

void myReshape( int width, int height )
//------------------------------------------------------------------------------
{
// Called by GLUT on initial creation of the window, using the width,height
// arguments given in glutInitWindowSize(); see main() fcn. ALSO called by GLUT
// whenever you re-size the display window to a different width, height.
    nu_display_width    = width;    // save the current display size
    nu_display_height   = height;   // in these global vars.


    if(width > height)
    {	// WIDE window:  height sets viewport size, and
        glViewport((width-height)/2,0, height, height);
    }	// move origin rightwards to keep image centered in window.
    else
    {	// TALL window:  width sets viewport size, and
        glViewport(0,(height-width)/2, width, width);
        // need to origin upwards to keep image centered in window
    }		// viewport stays SQUARE, but extends to left and right of window

// or invent your own; Perhaps make square CVV fit around window?
// Or could you draw TWO viewports on-screen, side-by-side in the same window?
//
//--------------------------------
// Create a simple 3D orthographic camera that shows contents of CVV only:
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-2.0, 2.0,              // left, right
                    -2.0, 2.0,              // bottom, top
                    1.0, 1000.0);


	glutPostRedisplay();			// request redraw--we changed window size!
}



void myKeyboard(unsigned char key, int x, int y)
//------------------------------------------------------------------------------
// GLUT CALLBACK: Don't call this function in your program--GLUT does it every
// time user presses an alphanumeric key on the keyboard.
//	xw,yw == mouse position in window-system pixels (origin at UPPER left)

{
int curMatl;
int junk;                   // to stop compiler warnings

    junk = x; junk = y;     // stops warnings of 'unused parameters' x,y
	switch(key) {
		case 27: // Esc
		case 'Q':
		case 'q':
		case ' ':
			exit(0);		// Quit application
			break;
		case 'r':
			setCam.reset();	// reset camera coord system,
			break;
		case 'R':
			setModel.reset();	// reset model coord system.
			break;
        case 'm':
        case 'M':           // Cycle through all materials for stuff[0];

            break;
		case '<':			// -Z Translation for model coord system
			setModel.z_pos -= 0.3;
			break;
		case '>':			// +Z Translation for model coord system
			setModel.z_pos += 0.3;
			 break;
        case 's':
            if(material_changeflag==0) material_changeflag=1;
            else material_changeflag  = 0;
        case '+':           // PhotoShop-like move in/out with +/- keys
        case '=':
            setCam.z_pos += 0.1;
            break;
        case 'h':
            printHelp();
        case '-':
            setCam.z_pos -= 0.1;
			break;
        case '1':
            if(light1==0) light1=1;
            else light1 = 0;
            cout<<light1<<endl;
            break;
        case 't':       //apply textures
            if(texdraw==0) texdraw = 1;
            else texdraw = 0;
            break;
        case '2' :
            if(light2==0) light2=1;
            else light2  = 0;
            break;
        case '0':
            if(light0==0) light0=1;
            else light0 = 0;
		default:
			printf("unknown key %c:  Try arrow keys, r, R, s, S, <, >, or q",key);
			break;
	}
}

void myKeySpecial(int key, int xw, int yw)
//------------------------------------------------------------------------------
// Called by GLUT when users press 'arrow' keys, 'Fcn' keys, or any other
// non-alphanumeric key.  Search glut.h to find other #define literals for
// special keys, such as GLUT_KEY_F1, GLUT_KEY_UP, etc.
//	xw,yw == mouse position in window-system pixels (origin at UPPER left!)/
{

	// We might have changed something. Force a re-display
	glutPostRedisplay();
}

void myMouseClik(int buttonID, int upDown, int xpos, int ypos)
//-----------------------------------------------------------------------------
// Called by GLUT on mouse button click or un-click
// When user clicks a mouse button,  buttonID== 0 for left mouse button,
//                              		    (== 1 for middle mouse button?)
//	                                 		 == 2 for right mouse button;
//	                        upDown  == 0 if mouse button was pressed down,
//	                                == 1 if mouse button released.
//	xw,yw == mouse position in window-system pixels (origin at UPPER left!)
{
    if(buttonID==0)				// if left mouse button,
	{
		if(upDown==0)			// on mouse press,
		{
			setCam.isDragging = 1;	// ready to record cam coord system changes.
			setCam.m_x = xpos;		// Dragging begins here.
			setCam.m_y = ypos;
		}
		else setCam.isDragging = 0;
	}
	else if(buttonID==2)		// if right mouse button,
	{
		if(upDown==0)
		{
			setModel.isDragging = 1;// ready to record model coord system changes.
			setModel.m_x = xpos;	// Dragging begins here.
			setModel.m_y = ypos;
		}
		else setModel.isDragging = 0;
	}
	else						// something else.
	{
		setCam.isDragging  = 0;	// default; DON'T change cam,
		setModel.isDragging = 0;	//					or  model coord systems.
	}

	printf("clik: buttonID=%d, upDown=%d, xpos=%d, ypos%d\n",
										buttonID,upDown,xpos,ypos);
}

void myMouseMove(int xpos, int ypos)
//-----------------------------------------------------------------------------
// Called by GLUT when user moves mouse while pressing any mouse button.
//	xw,yw == mouse position in window-system pixels (origin at UPPER left)
{
#define JT_INCR 1.0					// Degrees rotation per pixel of mouse move

	if(setModel.isDragging==1)			// if we're dragging the left mouse,
	{								// increment the x,y rotation amounts.
		setModel.x_rot += JT_INCR*(xpos - setModel.m_x);
		setModel.y_rot += JT_INCR*(ypos - setModel.m_y);
		setModel.m_x = xpos;		// and update current mouse position.
		setModel.m_y = ypos;
	}
	if(setCam.isDragging==1)		// if we're dragging theright mouse,
	{								// increment the x,y rotation amounts.
		setCam.x_rot += JT_INCR*(xpos - setCam.m_x);
		setCam.y_rot += JT_INCR*(ypos - setCam.m_y);
		setCam.m_x = xpos;
		setCam.m_y = ypos;
	}
	printf("move %d, %d\n", xpos,ypos);	// print what we did.

	// We might have changed something. Force a re-display
	glutPostRedisplay();

#undef JT_INCR
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
    {                           // YES.
        glutPostRedisplay();    // tell GLUT to re-draw the screen, and
        runAnimTimer(1);        // continue with the next time interval.
    }
                                // NO. Do nothing. No further animation.
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

// Read the current size and position of the graphics display window
//------------------------------------------------------------------------------
int getDisplayHeight(void) { return nu_display_height; }
int getDisplayWidth(void)  { return nu_display_width; }
int getDisplayXpos(void)   { return nu_display_xpos; }
int getDisplayYpos(void)   { return nu_display_ypos; }


void drawSquarePoints(void)
//------------------------------------------------------------------------------
// Draw a 2D square at points +/-1, +/-1. Note counter-clockwise order:
{
	glPointSize(3.0);
	glBegin(GL_POINTS);
		glVertex2d( 1.0,  1.0);
		glVertex2d(-1.0,  1.0);
		glVertex2d(-1.0, -1.0);
		glVertex2d( 1.0, -1.0);
	glEnd();

}

void drawSquareEdges(void)
//------------------------------------------------------------------------------
// Draw a 2D square at points +/-1, +/-1. Note counter-clockwise order:
{
	glBegin(GL_LINE_LOOP);
		glVertex2d( 1.0,  1.0);
		glVertex2d(-1.0,  1.0);
		glVertex2d(-1.0, -1.0);
		glVertex2d( 1.0, -1.0);
	glEnd();

}


void drawSquareFace(void)
//------------------------------------------------------------------------------
// Draw a 2D square at point +/-1, +/-1. Note counter-clockwise order:
{
	glBegin(GL_QUADS);
		glVertex2d( 1.0,  1.0);
		glVertex2d(-1.0,  1.0);
		glVertex2d(-1.0, -1.0);
		glVertex2d( 1.0, -1.0);
	glEnd();
}


int randRangeInt(int floor,int ceil)
{
    int range = (ceil - floor);
    int output = rand() % (ceil - floor + 1) + floor;
    //cout<<output<<endl;
    return output;
}

float randRangeFloat(float floor,float ceil)
{
    float range = ceil-floor;
    return floor + (range*rand())/(RAND_MAX+1.0);
}
void printHelp()
{
    cout<<"Enter 0,1, 2 to switch off the lights"<<endl;
    cout<<"use left button on mouse to turn the scene \n and the right button to change the light"<<endl;
}
void cube1 (void)
{




    glutSolidTeapot(2.0);
}
