//===============  OpenGL and GLUT on *ANY* CodeBlocks platform  ===============
//3456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789_
// (J. Tumblin: I like consistent 80-char column width)
//
// multi-platform includes for the GLUT/OpenGL libraries:
#if defined(_WIN32)

//#include <unistd.h>         // defines symbolic constants, POSIX versions, etc.
#include <GL/glew.h>        // OpenGL 'extensions wrangler' for 'mesa' lib usage
#include <GL/freeglut.h>// open-source GLUT functions for windows, keybd, mouse
#include <GL/freeglut.h>
//#include <GL/gl.h>
#include <GL/glu.h>
//#include <GL/glext.h>
#include <math.h>
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
//#include <GL/glext.h>
#include <math.h>

#endif

class CCube
{
    static const int BufferSize = 3;
    GLuint BufferName[BufferSize];

    static const GLsizei VertexCount;
    static const GLsizeiptr IndexSize;
    static const GLsizeiptr PositionSize;
    static const GLsizeiptr ColorSize;


    static const GLfloat PositionData[];
    static const GLfloat ColorData[];
    static const GLuint IndexData[];
    enum EVertexBufferTag
    {
        INDEX_OBJECT = 0,
        POSITION_OBJECT = 1,
        COLOR_OBJECT = 2
    };
    public:
    CCube();
    ~CCube();
    void make();
    void draw();
    void end();

};

class CPyramid
{
    static const int BufferSize = 4;
    GLuint BufferName[BufferSize];

    static const GLsizei VertexCount;
    static const GLsizeiptr IndexSize;
    static const GLsizeiptr PositionSize;
    static const GLsizeiptr ColorSize;
    static const GLsizeiptr NormalSize;

    static const GLfloat PositionData[];
    static const GLfloat ColorData[];
    static const GLuint IndexData[];
    static const GLfloat NormalData[];
    enum EVertexBufferTag
    {
        INDEX_OBJECT = 0,
        POSITION_OBJECT = 1,
        COLOR_OBJECT = 2,
        NORMAL_OBJECT = 3
    };
    public:
    CPyramid();
    ~CPyramid();
    void make();
    void draw();
    void end();
};

class CRectangle
{

    static const int BufferSize = 3;
    GLuint BufferName[BufferSize];

    static const GLsizei VertexCount;
    static const GLsizeiptr IndexSize;
    static const GLsizeiptr PositionSize;
    static const GLsizeiptr ColorSize;


    static const GLfloat PositionData[];
    static const GLfloat ColorData[];
    static const GLuint IndexData[];
    enum EVertexBufferTag
    {
        INDEX_OBJECT = 0,
        POSITION_OBJECT = 1,
        COLOR_OBJECT = 2
    };

    public:
    CRectangle(void);
    ~CRectangle(void);
    void make(void);
    void draw(void);
    void end(void);
};

class CTriangle
{
    static const int BufferSize = 3;
    GLuint BufferName[BufferSize];

    static const GLsizei VertexCount;
    static const GLsizeiptr IndexSize;
    static const GLsizeiptr PositionSize;
    static const GLsizeiptr ColorSize;


    static const GLfloat PositionData[];
    static const GLfloat ColorData[];
    static const GLuint IndexData[];
    enum EVertexBufferTag
    {
        INDEX_OBJECT = 0,
        POSITION_OBJECT = 1,
        COLOR_OBJECT = 2
    };

    public:
    CTriangle(void);
     ~CTriangle(void);

    void make(void);
    void draw(void);
    void end(void);


};

