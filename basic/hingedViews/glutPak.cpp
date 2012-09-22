
//3456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789_
/* (JT: I prefer to keep line lengths <=80 chars for easy printing/display).

	glutPak.cpp         **START READING HERE***

    Demo of GLUT API for operating system services and GUI callbacks for OpenGL.
    This file contains 'main()' and all the openGL callback functions we'll use.

2010.12.13-J. Tumblin: created for use in EECS351-1 at Northwestern University
2011.09.08-J. Tumblin: added openGL bitmap examples and image-file reading
2011.09.30-J. Tumblin: added 'PixelWorld' camera & viewport settings,

*=============================================================================*/

#include "glutPak.h"

//=====================
// GLOBAL VARIABLE(S): a bad idea, but forced by GLUT's fixed-arg callback fcns.
//                   Always use as FEW global vars as possible.  Best strategy
//                   I've found is to create a top-level class (e.g. CTheApp)
//                   that holds all the top-level data and functions for your
//                   program.  Then make just one object of this class to use as
//                   your program's one and only global variable (e.g. myApp).
//                   This 'top-level' class idea is sensible for ANY object-
//                   -oriented program, as it lets you make multiple instances
//                   of the entire application by just creating more objects of
//                   the top-level 'CTheApp' class.
//=====================
                            // the app's state variables that would otherwise
                            // have to be declared as global variables.

#ifndef M_PI                // be sure pi exists
#define M_PI acos(-1.0)     // ugly, slow, but exactly accurate M_PI replacement
#endif


CTheApp::CTheApp(void)
//------------------------------------------------------------------------------
// default constructor
{
    display_width = 640;	            // global variables that hold current
    display_height = 640;	            // display window position and size;
    display_xpos = 200;
    display_ypos = 100;
    strncpy(winTitle, "2011 NU EECS 351-1 GlutPak", 127); // (buffer size:128)
    doAnim  = 1;                      // ==1 to run animation, ==0 to pause.
    doClear = 1;
    animFrameTime = NU_PAUSE;           // default: wait between frames in mS
    // screen-clear color;
    clrColr4dv[0]= 0.7;
    clrColr4dv[1]= 0.7;
    clrColr4dv[2]= 0.7;
    clrColr4dv[3]= 0.7;
    // drawing (pen) color;
    penColr4dv[0]= 82.0/255.0;      // color; 0.0<=RGB<=1.0, GLdouble
    penColr4dv[1]= 0.0;             // Official NU purple; R== 82, G== 0, B== 99
    penColr4dv[2]= 99.0/255.0;
    penColr4dv[3]= 0.0;
    // text-writing color;
    txtColr4dv[0] = 0.1;
    txtColr4dv[1] = 0.4;
    txtColr4dv[2] = 0.1;
    txtColr4dv[3] = 0.5;
    // error-reporting color;
    errColr4dv[0] = 0.0;    // PURE BLACK
    errColr4dv[1] = 0.0;
    errColr4dv[2] = 0.0;
    errColr4dv[3] = 1.0;

    shapeNum = 0;                       // Shape-selector number.
    shape_xpos = shape_ypos = 0.0;      // initial position (arrow keys)
    shape_deg = 0.0;                    // initial orientation
    shape_degStep = 0.51;               // rotation (degrees) per time-step
    imgXpos[0]= 1;
    imgXpos[1] = -1;
    imgXpos[2] = 1;
    imgXpos[3] = -1;
    imgYpos[0]= 1;
    imgYpos[1] = 1;
    imgYpos[2] = -1;
    imgYpos[3] = -1;
    imgZpos[0] = 0;
    imgZpos[1] = 0;
    imgZpos[2] = 0;
    imgZpos[3] = 0;
    //  Start up the FreeImage library for image-file I/0:
#ifdef FREEIMAGE_LIB
	// call this ONLY when linking with FreeImage as a static library
		FreeImage_Initialise();
#endif
}

CTheApp::~CTheApp(void)
//------------------------------------------------------------------------------
// default destructor
{
	// call this ONLY when linking with FreeImage as a static library
	#ifdef FREEIMAGE_LIB
		FreeImage_DeInitialise();
	#endif
    return;
}

void CTheApp::reportKeyModifiers(const char *pName)
//------------------------------------------------------------------------------
// Text and graphical display of any keyboard modifiers (SHIFT, CTRL, ALT).
// Used by keyboard & mouse callback fcns.
// Usage:  within myKeyboard() --   myApp.reportKeyModifiers("myKeyboard()");
{
    KBflags = glutGetModifiers();           // GLUT's keyboard-mod reader
    if(KBflags & GLUT_ACTIVE_SHIFT)
    {
        drawText3d(-0.9, 0.9, -0.9, "!!!SHIFT KEY!!!", 4);  // on-screen
        cout << pName << ": SHIFT-key" << endl;             // in text window
    }
    if(KBflags & GLUT_ACTIVE_CTRL )
    {
        drawText3d(-0.9, 0.8, -0.9, "!!! CTRL KEY!!!", 4);
        cout << pName << ": CTRL-key" << endl;
    }
    if(KBflags & GLUT_ACTIVE_ALT  )
    {
        drawText3d(-0.9, 0.7, -0.9, "!!! ALT  KEY!!!", 4);
        cout << pName << ": ALT-key" << endl;
    }
}

void CTheApp::drawAxes(void)
//------------------------------------------------------------------------------
// Draw small +x,+y,+z axes in red, green, blue:
{
	glColor3d(1.0, 0.0, 0.0);	// red
	glBegin(GL_LINES);
		glVertex3d(0.0,0.0,0.0);	// +x-axis
		glVertex3d(0.4,0.0,0.0);
	glEnd();
	drawText3d(0.4, 0.0, 0.0, "X", 2);       // label it.

	glColor3d(0.0, 1.0, 0.0);	// green
	glBegin(GL_LINES);
		glVertex3d(0.0,0.0,0.0);	// +y-axis
		glVertex3d(0.0,0.4,0.0);
	glEnd();
    drawText3d(0.0, 0.4, 0.0, "Y", 2);       // label it.

	glColor3d(0.0, 0.5, 1.0);	// bright blue
	glBegin(GL_LINES);
		glVertex3d(0.0,0.0,0.0);	// +z-axis
		glVertex3d(0.0,0.0,0.4);
	glEnd();
	drawText3d(0.0, 0.0, 0.4, "Z" ,2);       // label it.
}

void CTheApp::drawText3d( GLdouble x0, GLdouble y0, GLdouble z0,
                         const char *pString, int fontNum)
//-----------------------------------------------------------------------------
//  Use GLUT to write (bitmapped) text string that starts at pointer 'pString'.
//  --Use current pen color (CTheApp::penColrv4), but if the text string begins
//      with an exclamation point, it uses error color (CTheApp::errColrv4).
//  --Places the lower left corner of the first line of text at screen position
//      x0,y0,z0 in the current coordinate system.
//      Example:  DrawText3D(0.3, 0.4, 0.0,"Hello!", 4);
//  -- uses 'fontNum' to select among these GLUT pre-defined fonts:
// Font#
//  0   --  10pt HELVETICA
//  1   --  12pt HELVETICA
//  2   --  18pt HELVETICA
//  3   --  10pt TIMES_ROMAN
//  4   --  24pt TIMES_ROMAN
//-------------------------------
// NOTE: Unlike GLUT, the freeGLUT library offers support for stroke-font chars
// and functions for strings; you may want to use that instead
// SEE: glutBitmapString(), glutStrokeCharacter(), glutStrokeString() at this
//  site:  http://freeglut.sourceforge.net/docs/api.php#Introduction
{
int i, imax;					// character counters
void *pFont;                    // function ptr for fonts

    // IF string begins with '!' (exclamation point)
    if(pString[0]=='!' || pString[1]=='!')
         glColor4dv(errColr4dv);        // use the 'error' text color
    else glColor4dv(txtColr4dv);	    // otherwise, ordinary text color
    switch(fontNum)
    {
        case 0: pFont = GLUT_BITMAP_HELVETICA_10;   break;
        case 1: pFont = GLUT_BITMAP_HELVETICA_12;   break;
        case 2: pFont = GLUT_BITMAP_HELVETICA_18;   break;
        case 3: pFont = GLUT_BITMAP_TIMES_ROMAN_10; break;
        case 4:     // (the default choice)
        default:pFont = GLUT_BITMAP_TIMES_ROMAN_24; break;
    }
	glRasterPos3d(x0, y0, z0);	// set text's lower-left corner position
	imax = 1023;				// prevent overflow.
	for(i=0; pString[i] != '\0' && i<imax; i++)	// for each char,
	{
		glutBitmapCharacter(pFont, pString[i]);
	}
}

void CTheApp::drawShape(void)
//------------------------------------------------------------------------------
// Use 'penColr[]' to draw the GLUT shape selected by 'shapeNum' at (xpos,ypos)
// using the current coordinate system.
// TRANSFORMATION SYNTAX HINTS:
//	glRotated(-123, 1,-2,1);	// rotate by -123 degrees around 3D axis vector;
//	glScaled(0.2, 0.3, 0.4);	// shrink along x,y,z axes
//  glTranslated(0.1,0.3,0.5);	// translate along x,y,z axes.
{
    glMatrixMode(GL_MODELVIEW);     // use this matrix to change coord systems
    glPushMatrix();                 // save its current value,

        glTranslated(shape_xpos, shape_ypos, shape_zpos);   // change coords.
        // Draw a selected shape from those GLUT supplies 'ready-made':
        glRotated(shape_deg, 0.0, 0.0, 1.0);       // rotate around Z axis.

        // Set drawing color:
        glColor4dv(penColr4dv);      // Set pen color,
        switch(shapeNum)            // Draw the (selected) GLUT shape
        {
            case 0: glutWireCube( 1.0 ); break; // length of cube's edges
            case 1: glutSolidCube ( 1.0 ); break;
            // How is glRectd() different?.... (HINT: it's 2D, not 3D)
            //      glRectd(-0.5, -0.5,     // Lower-left x,y
            //               0.5,  0.5);     // Upper-right x,y.
                             break;
            case 2:
                glPushMatrix();             // save current tranformation matrix
                    glRotated(80.0,0.0, 1.0, 0.0);  // turn 80 deg on y axis,
                    glutWireCone( 1.0, 1.0, 10, 6 );// draw 3D cone
                glPopMatrix();              // restore prev. transform. matrix
            break;
            case 3:
                glPushMatrix();             // save current tranformation matrix
                    glRotated(80.0,0.0, 1.0, 0.0);  // turn 80 deg on y axis,
                    glutSolidCone( 1.0, 1.0, 10, 6);// draw 3D cone
                glPopMatrix();              // restore prev. transform. matrix
            break;
            case 4:
                glPushMatrix();             // save current tranformation matrix
                    glRotated(40.0,0.0, 1.0, 0.0);  // turn 40 deg on y axis,
                    glutWireTorus(
                    0.3,            // inner radius, (smaller of the 2 radii;
                                    // radius of rings enclosing 'core-circle' )
                    0.7,            // outer radius, (larger of the 2 radii;
                                    // for 'core circle' enclosed within torus)
                    16,             // number of sides for each ring;
                    32);            // number of rings around center-circle.
                glPopMatrix();              // restore prev. transform. matrix
                break;
            case 5:
                glutSolidTorus(
                0.3,            // inner radius, (smaller of the 2 radii;
                                // radius of rings enclosing 'core-circle' )
                0.7,            // outer radius, (larger of the 2 radii;
                                // for 'core circle' enclosed within torus)
                16,             // number of sides for each ring;
                32);            // number of rings around center-circle.
                break;
/*
Other GLUT shapes for you to try... found in GLUT.h (or freeglut_std.h)
case XX:glutWireSphere( GLdouble radius, GLint slices, GLint stacks );
case XX:glutSolidSphere( GLdouble radius, GLint slices, GLint stacks );
case XX:glutWireDodecahedron( void );
case XX:glutSolidDodecahedron( void );
case XX:glutWireOctahedron( void );
case XX:glutSolidOctahedron( void );
case XX:lutWireTetrahedron( void );
case XX:glutSolidTetrahedron( void );
case XX:glutWireIcosahedron( void );
case XX:glutSolidIcosahedron( void );
*/
            case 6:
                glPushMatrix();             // save current tranformation matrix
                glRotated(11.0, 1.0, 0.0, 0.0);  // turn 80 deg on x axis,
                    glutWireTeapot( 0.5 );
                glPopMatrix();              // restore prev. transform. matrix
                    break;      // size.
            case 7:    // (default value)
            default:    glutSolidTeapot( 0.5 );         // size.
                break;
        }
    glPopMatrix();                  // restore its previous coord. system.
    return;
}

/*===============================================================================
 OpenGL Pixel-Block Operations: --"BLITTING"--
 =============================
OpenGL gives you exquisitely flexible tools for manipulating blocks of pixels
held in your computers memory and in its openGL graphics hardware.  However,
the default settings are not always correct for all uses on all machines; if
not, use these formatting functions first:

FORMATTING FUNCTIONS
=====================
--glPixelStore() lets openGL read & write pixel blocks in the
                correct format for YOUR computer's memory arrays.
  **CAREFUL! Don't assume default values for glPixelStore() are what you want!
  **DETAILS: glPixelStore() lets you change the settings openGL uses to pack or
    unpack blocks of pixels stored in your computer's memory (in 'client side'
    buffers).  It can accomodate almost any client memory requirements;
    for example, it lets you specify that that each scanline read/write begins
    on 8, 16, or 32 bit boundaries (nice for MS Windows); it lets you specify
    byte-order swaps (nice for Apple OS-X or any little-endian vs. big-endian
    mismatches:     http://en.wikipedia.org/wiki/Endianness.

--glPixelTransfer() changes colors, indices, maps, scales & biases.
  **CAREFUL! Pgms using only true-color images rarely need glPixelTransfer().
  **DETAILS: glPixelTransfer() lets you change openGL settings that enable it
    to modify or rearrange pixel values during pixel-block transfers between
    buffers.  For example, it could rearrange BGR-format pixels to RBG format
    as it transfers them from a client-side buffer (your computer's memory) to
    a texture buffer.  It can use color index tables, gamma correction lookup
    tables,change stencil bits and more.  All specified changes apply to all
    transfers, including those by glDrawPixels(), glReadPixels(),
    glCopyPixels(), and glTexImage() etc.

--'glPixelMap() stores maps and tables used by glPixelTransfer().
  **NOTE: If you don't need glPixelTransfer(), you don't need glPixelMap().
  **DETAILS:glPixelMap() lets to create the lookup tables used by
    glPixelTransfer(). These lookup tables change pixel values, such as
    gamma-correction tables applied to R,G,B,A values,
    or tables that set stencil values.

BUFFER SELECTION FUNCTIONS
==========================
The openGL 'Pixel-Block' manipulating functions give us, by default, access to
the openGL color buffer(s) used for drawing on-screen.
For single-buffered display, they access the GL_FRONT buffer by default;
for double-buffered display, they access the GL_BACK  buffer by default.
To change buffers, call
    --glReadBuffer() to set the source buffer for pixel-block reading,
    --glDrawBuffer() to set the destination buffer for pixel-block writing.

Available Color Buffer Choices:
--In addition to GL_FRONT and GL_BACK, you may find the GL_FRONT_AND_BACK
    choice useful for some interactive, double-buffered applications. After the
    function call glDrawBuffer(GL_FRONT_AND_BACK), pixel-block functions will
    write to BOTH the front and back buffers simultaneously.
--Double-buffered stereo display implemented in openGL gives you four usable
    color buffers: GL_RIGHT_FRONT, GL_LEFT_FRONT,GL_RIGHT_BACK,GL_LEFT_BACK.
--Better graphics hardware may provide 'auxiliary' color buffers (GL_AUX0,
    GL_AUX1, GL_AUX2, ... GL_AUXn) to hold partial renderings for tasks such as
    picking,shadows, transparency, and anti-aliasing.
--Texture Buffers: the long-awaited 'render-to-texture' functions that let you
    use your on-screen display image as a texture map.

PIXEL-MOVING FUNCTIONS
=======================
--glBitmap() unpacks a rectangle of ***1-bit*** values from your computer's
    memory (a client-side buffer) to the openGL display. The default 'unpacking'
    method usually works on all machines if you store sequential bits in
    sequential bytes in sequential rows ('row-major' order), starting with the
    bottom-most row and working upwards (e.g. origin at lower left).  Otherwise,
    use the glPixelStore() function to select other non-default formatting
    options for reading from your computer's memory.
  **DETAILS: glBitmap() draws the lower-left corner of the bitmap rectangle at
    the (transformed) GL_CURRENT_RASTER_POSITION vector value set by
    glRasterPos*() functions. (Note this position gets transformed bu
    GL_MODELVIEW and GL_PROJECTION before use).
    It draws the rectangles' contents on-screen (1 bit per pixel), and then
    changes the GL_CURRENT_RASTER_POSITION value by an amount you specify
    (often it's just (0,0)).
        OpenGL offers 16 different ways to 'draw' a bitmap on-screen, but the
    default 'overlay' method is very simple. Where the bitmap==0, leave pixel
    values unchanged; where the bitmap==1, replace the existing display color
    with openGL's current bitmap-drawing color (usually the glColor() value).
    SEE: http://www.opengl.org/sdk/docs/man/xhtml/glBitmap.xml

    -----Bitmap-Drawing Color:-----
        When you call 'glRasterPos*()' it saves the most recent glColor() value
    as the bitmap-drawing color; when you call glBitmap(), it uses this color
    and clears the bitmap-drawing color.  If you call you call glBitmap() while
    the bitmap-drawing color is cleared, it uses the current glColor() instead.

    -----Other Bitmap Drawing Ops-----
    To use any non-default drawing method, you must:
     a) enable it: call glEnable(GL_COLOR_LOGIC_OP);  (disabled by default)
     b) specify it: call glLogicOp() with the desired drawing method as argument.
        Besides the default GL_OVERLAY arg, try GL_COPY, GL_OR, or GL_XOR.
        See:   http://www.opengl.org/sdk/docs/man/xhtml/glLogicOp.xml

--glDrawPixels() 'unpacks' a rectangular block of pixels from your computer's
    memory (a client-side buffer) and copies them to the openGL display at the
    (transformed) CURRENT_RASTER_POSITION value set by glRasterPos().
    The default 'unpacking' method usually works on all machines if you store
    sequential pixel-components (RGB, RGBA, etc) in sequential pixels in
    sequential rows ('row-major' order), e.g. GLubyte buf[HEIGHT][WIDTH][COLOR];
    If not, see glPixelStore() to specify your machine's formatting.
    glDrawPixels() places the lower-left corner of the user's block of pixels
    on-screen at the (transformed) glRasterPos*() position (the position gets
    transformed by GL_MODELVIEW and GL_PROJECTION before use).
        SEE:   http://www.opengl.org/sdk/docs/man/xhtml/glDrawPixels.xml

--glReadPixels() selects a rectangular block of pixels from openGL's display
    buffer from absolute window position (x,y) and 'packs' them into a user-
    supplied memory buffer (a client-side buffer).  The default 'packing' method
    usually works for all machines if you read the result using sequential
    pixel-components (RGB, RGBA, etc), in sequential pixels in sequential rows
    ('row-major' order), e.g. GLubyte buf[HEIGHT][WIDTH][COLOR];
    If not, use the glPixelStore() fcn. to specify your machine's formatting.
    CAREFUL!  glReadPixels() does NOT use glRasterPos() position (screen position

--glCopyPixels() copies a rectangular block of pixels at the absolute window
    position (x,y) and PASTES (writes) them to the (transformed) glRasterPos()
    position in the same openGL buffer.  It needs no client-side memory buffer,
    and thus is MUCH faster than calling glReadPixels() then glDrawPixels().
*/


void CTheApp::printPixelStoreReport(void)
//------------------------------------------------------------------------------
// OpenGL gives you exquisitely flexible tools for manipulating blocks of pixels
// in any arrangement in memory on any machine, but it's default settings aren't
// always correct for all uses on all machines.  This function reports the
// current settings openGL will use to pack or unpack blocks of pixels you store
// in your computer's memory ( in 'client side' buffers, not in the GPU).
// To modify these settings, call glPixelStore(), explained here:
//          http://www.opengl.org/sdk/docs/man/xhtml/glPixelStore.xml
{
GLboolean isTrueA, isTrueB;
GLint  countA, countB;

    cout << "===================printPixelStoreTest()==================" <<endl;
    glGetBooleanv(  GL_PACK_SWAP_BYTES, &isTrueA);
    glGetBooleanv(GL_UNPACK_SWAP_BYTES, &isTrueB);
    cout << "      GL_PACK_SWAP_BYTES\t";
    if(isTrueA == true) cout << "TRUE\t";
                else    cout << " no \t";
    cout << "    GL_UNPACK_SWAP_BYTES\t";
    if(isTrueB == true) cout << "TRUE" << endl;
                else    cout << " no  " << endl;
    //------------
    glGetBooleanv(  GL_PACK_LSB_FIRST, &isTrueA);
    glGetBooleanv(GL_UNPACK_LSB_FIRST, &isTrueB);
    cout << "      GL_PACK_LSB_FIRST  \t";
    if(isTrueA == true) cout << "TRUE \t";
                else    cout << " no  \t";
    cout << "    GL_UNPACK_LSB_FIRST  \t";
    if(isTrueB == true) cout << "TRUE" << endl;
                else    cout << " no  " << endl;
    //------------
    glGetIntegerv(  GL_PACK_ROW_LENGTH, &countA);
    glGetIntegerv(GL_UNPACK_ROW_LENGTH, &countB);
    cout << "      GL_PACK_ROW_LENGTH \t";
    if(countA != 0) cout << countA <<"\t";
                else    cout << " no  \t";
    cout << "    GL_UNPACK_ROW_LENGTH\t";
    if(countB != 0) cout << countB << endl;
                else    cout << " no  " << endl;
    //------------
    // THIS NEXT VALUE ISN'T AVAILABLE.
//    glGetIntegerv(  GL_PACK_IMAGE_HEIGHT, &countA);
//    glGetIntegerv(GL_UNPACK_IMAGE_HEIGHT, &countB);
//    cout << "      GL_PACK_IMAGE_HEIGHT\t";
//    if(countA != 0) cout << countA << "\t";
//                else    cout << " no  \t";
//    cout << "    GL_UNPACK_IMAGE_HEIGHT\t";
//    if(countB != 0) cout << countB << endl;
//                else    cout << " no  " << endl;
   //------------
    glGetIntegerv(  GL_PACK_SKIP_PIXELS, &countA);
    glGetIntegerv(GL_UNPACK_SKIP_PIXELS, &countB);
    cout << "      GL_PACK_SKIP_PIXELS \t";
    if(countA != 0) cout << countA << "\t";
                else    cout << " no  \t";
    cout << "    GL_UNPACK_SKIP_PIXELS\t";
    if(countB != 0) cout << countB << endl;
                else    cout << " no  " << endl;
    //------------
    glGetIntegerv(  GL_PACK_SKIP_ROWS, &countA);
    glGetIntegerv(GL_UNPACK_SKIP_ROWS, &countB);
    cout << "      GL_PACK_SKIP_ROWS  \t";
    if(countA != 0) cout << countA << "\t";
                else    cout << " no  \t";
    cout << "    GL_UNPACK_SKIP_ROWS  \t";
    if(countB != 0) cout << countB << endl;
                else    cout << " no  " << endl;
    //------------
//    glGetIntegerv(  GL_PACK_SKIP_IMAGES, &countA);
//    cout << "      GL_PACK_SKIP_IMAGES\t";
//    if(countA != 0) cout << countA << endl;
//                else    cout << " no " << endl;;
    //------------
    glGetIntegerv(  GL_PACK_ALIGNMENT, &countA);
    glGetIntegerv(GL_UNPACK_ALIGNMENT, &countB);
    cout << "      GL_PACK_ALIGNMENT  \t";
    if(countA != 0) cout << countA << "\t";
                else    cout << " no  \t";
    cout << "    GL_UNPACK_ALIGNMENT   \t";
    if(countB != 0) cout << countB << endl;
                else    cout << " no  " << endl;
}
