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
#define NUM_CUBE_FACES 6
#define NUM_VERTICES_PER_FACE 4
#define NUM_VERTICES 8

GLuint pyrVertBuffer,pyrColrBuffer;

class Ccube
{
    float facecolor[NUM_CUBE_FACES][3];					// RGB values
    int face[NUM_CUBE_FACES][NUM_VERTICES_PER_FACE];
    float vertex[NUM_VERTICES][3];
    public:
        Ccube()
        {

        }
        ~Ccube()
        {

        }

    void makecube( void )
    {
        // assign face colors
        facecolor[0][0] = 1.0; facecolor[0][1] = 0.0; facecolor[0][2] = 0.0; //RED
        facecolor[1][0] = 0.0; facecolor[1][1] = 1.0; facecolor[1][2] = 0.0; //GREEN
        facecolor[2][0] = 0.0; facecolor[2][1] = 0.0; facecolor[2][2] = 1.0; //BLUE
        facecolor[3][0] = 0.0; facecolor[3][1] = 1.0; facecolor[3][2] = 1.0; //CYAN


        // assign vertices
        vertex[0][0] = -1; vertex[0][1] = -1; vertex[0][2] = -1; // lower left back
        vertex[1][0] = -1; vertex[1][1] = -1; vertex[1][2] = 1;  // lower left front
        vertex[2][0] = 1;  vertex[2][1] = -1; vertex[2][2] = 1;  // lower right front
        vertex[3][0] = 1;  vertex[3][1] = -1; vertex[3][2] = -1; // lower right back
        vertex[4][0] = -1; vertex[4][1] = 1;  vertex[4][2] = -1; // upper left back
        vertex[5][0] = -1; vertex[5][1] = 1;  vertex[5][2] = 1;  // upper left front
        vertex[6][0] = 1;  vertex[6][1] = 1;  vertex[6][2] = 1;  // upper right front
        vertex[7][0] = 1;  vertex[7][1] = 1;  vertex[7][2] = -1; // upper right back

        // assign faces
        face[0][0] = 0; face[0][1] = 1; face[0][2] = 2; face[0][3] = 3; // base
        face[1][0] = 4; face[1][1] = 5; face[1][2] = 6; face[1][3] = 7; // top
        face[2][0] = 0; face[2][1] = 1; face[2][2] = 5; face[2][3] = 4; // left
        face[3][0] = 2; face[3][1] = 3; face[3][2] = 7; face[3][3] = 6; // right
        face[4][0] = 0; face[4][1] = 4; face[4][2] = 7; face[4][3] = 3; // back
        face[5][0] = 1; face[5][1] = 2; face[5][2] = 6; face[5][3] = 5; // front
    }

     void drawCube( void )
    //------------------------------------------------------------------------------
    // Use openGL calls to draw the cube as a solid object:
    {
        int i, j;

        glBegin( GL_QUADS );	// start rendering the quads (faces of cube)
        // (? can you make a version that uses triangle-drawing GL_TRIANGLES?)
        for ( i = 0; i < NUM_CUBE_FACES; i++ ) { // for each face
            	// set the face color
            for ( j = 0; j < NUM_VERTICES_PER_FACE; j++ )
            {
                glColor3fv( facecolor[j] );
                glVertex3fv( vertex[face[i][j]] );	// make quad
            }
        }
        glEnd();
    }


};


class Ctriangle
{
    /* The four vericies of a tetrahedron */

    GLuint vao,vbo[2];

    public:
    Ctriangle()
    {

    }

    ~Ctriangle()
    {

    }

    void make()
    {
          /* The four vericies of a tetrahedron */
        const GLfloat triangle[3][3] = {{1,0,-1},{1,0,1},{-1,0,1}};

        /* Color information for each vertex */
        const GLfloat colors[3][3] = {
        {  1.0,  1.0,  0.0  },   /* red */
        {  0.0,  1.0,  0.0  },   /* green */
        {  0.0,  0.0,  1.0  }
        }; /* white */


        glGenBuffers(2, vbo);

        glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
        glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), triangle, GL_STATIC_DRAW);


        glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
        glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), colors, GL_STATIC_DRAW);

    }

    void draw()
    {
        unsigned int indices[3] = {0,1,2};

        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, &indices);
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_COLOR_ARRAY);


    }


};



class Cpyramid
{
    GLdouble *pVerts,*pColrs;
    public:
    Cpyramid()
    {

    }

    ~Cpyramid()
    {
    }
    void makePyramid()
    {
        GLdouble pyrVerts[5][3] = {{1,0,-1},{1,0,1},{-1,0,1},{-1,0,-1},{0,1,0}};
        //GLdouble pyrVerts[4][3] = {{-1,-1,-1},{-1,1,-1},{1,1,-1},{1,-1,-1}};
        GLdouble pyrColrs[5][3] = {{0,1,1},{1,1,0},{1,0,1},{1,0,0},{0,1,0}};
//        int vertsOrder[] = {3,0,4, 0,1,4, 1,2,4, 2,3,4,
//                           2,3,0, 0,1,2};

        int vertsOrder[] = {3,0,2, 2,0,1, 0,1,4, 3,0,4, 2,3,4, 4,1,2};
        pVerts = (GLdouble*) calloc(18*3,sizeof(GLdouble));
        pColrs = (GLdouble*) calloc(18*3,sizeof(GLdouble));

        for(int i=0;i<18;i++)
        {
            pVerts[3*i] = pyrVerts[vertsOrder[i]][0];
            pVerts[3*i+1] = pyrVerts[vertsOrder[i]][1];
            pVerts[3*i+2] = pyrVerts[vertsOrder[i]][2];
            //cout<<"Storing ("<<pVerts[i]<<","<<pVerts[i+1]<<","<<pVerts[i+2]<<")"<<endl;

            pColrs[3*i] = pyrColrs[vertsOrder[i]][0];
            pColrs[3*i+1] = pyrColrs[vertsOrder[i]][1];
            pColrs[3*i+2] = pyrColrs[vertsOrder[i]][2];


        }

        //doing the pyramid with vertex buffer objects
        //glGenBuffers(1,&pyrVerts);
        //glBinder


        //glGenBuffers(1,&pyrColrBuffer);
        //glBindBuffer(GL_PIXEL_UNPACK_BUFFER,pyrColrBuffer);
        //glBufferData(GL_PIXEL_UNPACK_BUFFER,sizeof(pColrs),pColrs,GL_STREAM_DRAW);
        //glColorPointer(3,GL_DOUBLE,0,&pyrColrBuffer);

        //doing the pyramid with vertex array pointers LIKE A BAUSS
        glVertexPointer(3,GL_DOUBLE,0,pVerts);
        glColorPointer(3,GL_DOUBLE,0,pColrs);
    }

    void drawPyramid()
    {


        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);

        //vertex array method drawing
        glGenBuffers(1,&pyrVertBuffer);
        glBindBuffer(GL_ARRAY_BUFFER,pyrVertBuffer);
        glBufferData(GL_ARRAY_BUFFER,sizeof(GLdouble)*18*3,pVerts,GL_STATIC_DRAW);
        glVertexPointer(3,GL_DOUBLE,0,0);
        glDrawArrays(GL_TRIANGLES,0,18);

        //vertex buffer pyramid drawing

        //glDrawArrays(GL_TRIANGLES,0,18);
        //glDrawElements(GL_TRIANGLES,18,GL_UNSIGNED_INT,0);
    }
};

