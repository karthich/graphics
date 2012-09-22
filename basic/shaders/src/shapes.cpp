#include <iostream>
#include "../include/shapes.h"
using namespace std;
//------------------------------CCube Class Specification-----------------------------------------
//------------------------------------------------------------------------------------------------
const GLsizeiptr CCube::PositionSize = 8 * 3 * sizeof(GLfloat); //number of vertices*dimensions of vertex*sizeof(vertex)
const GLsizeiptr CCube::ColorSize = 8 * 3 * sizeof(GLfloat); //number of colors*dimensions of colors*sizeof(vertex)
const GLsizeiptr CCube::IndexSize = 36 * sizeof(GLuint);
const GLsizei CCube::VertexCount = 36; //number of vertices

const GLfloat CCube::PositionData[]=
    {
        -1.0f,-1.0f,-1.0f,
        -1.0f,1.0f,-1.0f,
        1.0f,1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f,1.0f,1.0f,
        -1.0f,1.0f,1.0f,
        -1.0f,-1.0f,1.0f,
        1.0f,-1.0f,1.0f
    };
const GLfloat CCube::ColorData[]=
    {
        1.0f,0.0f,0.0f,
        1.0f,0.0f,0.0f,
        1.0f,0.0f,0.0f,
        1.0f,0.0f,0.0f,
        1.0f,0.0f,0.0f,
        1.0f,0.0f,0.0f,
        1.0f,0.0f,0.0f
    };
const GLuint CCube::IndexData[]=
    {
        2,3,0, 0,1,2, // -z face
        5,6,7, 7,4,5, // +z face
        3,2,4, 4,7,3, // +x face
        1,5,4, 4,2,1, // +y face
        6,5,1, 1,0,6, // -x face
        3,0,6, 6,7,3 // -y face;
    };
void CCube::make()
{
    glGenBuffers(BufferSize, BufferName);

    glBindBuffer(GL_ARRAY_BUFFER, BufferName[COLOR_OBJECT]);
	glBufferData(GL_ARRAY_BUFFER, ColorSize, ColorData, GL_STREAM_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, BufferName[POSITION_OBJECT]);
    glBufferData(GL_ARRAY_BUFFER, PositionSize, PositionData, GL_STREAM_DRAW);

   	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BufferName[INDEX_OBJECT]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndexSize, IndexData, GL_STREAM_DRAW);
}

void CCube::draw()
{
    glBindBuffer(GL_ARRAY_BUFFER, BufferName[COLOR_OBJECT]);
	glColorPointer(3, GL_FLOAT, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, BufferName[POSITION_OBJECT]);
	glVertexPointer(3, GL_FLOAT, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BufferName[INDEX_OBJECT]);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

	glDrawElements(GL_TRIANGLES,VertexCount , GL_UNSIGNED_INT, 0);

    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);

}

void CCube::end()
{
    glDeleteBuffers(BufferSize,BufferName);
}
CCube::CCube()
{

}
CCube::~CCube()
{

}
//------------------------------CPyramid Class Specification--------------------------------------
//------------------------------------------------------------------------------------------------
const GLsizeiptr CPyramid::PositionSize = 5 * 3 * sizeof(GLfloat); //number of vertices*dimensions of vertex*sizeof(vertex)
const GLsizeiptr CPyramid::ColorSize = 5* 3 * sizeof(GLfloat); //number of colors*dimensions of colors*sizeof(vertex)
const GLsizeiptr CPyramid::IndexSize = 18 * sizeof(GLuint);
const GLsizeiptr CPyramid::NormalSize = 18 * sizeof(GLfloat);
const GLsizei CPyramid::VertexCount = 18;

const GLfloat CPyramid::PositionData[]=
    {
        1.0f,0.0f,-1.0f,
        1.0f,0.0f,1.0f,
        -1.0f,0.0f,1.0f,
        -1.0f,0.0f,-1.0f,
        0.0f,1.0f,0.0f
    };
const GLfloat CPyramid::ColorData[]=
    {
        1.0,  1.0,  0.0,
        1.0,  1.0,  0.0,
        1.0,  1.0,  0.0,
        1.0,  1.0,  0.0,
        1.0,  1.0,  0.0
    };

const GLfloat CPyramid::NormalData[] =
{
    0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, -1.0f,

    0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, -1.0f,


    0.707f, 0.707f, 0.0f,
    0.707f, 0.707f, 0.0f,
    0.707f, 0.707f, 0.0f,

    0.0f, 0.707f, -0.707f,
    0.0f, 0.707f, -0.707f,
    0.0f, 0.707f, -0.707f,

    0.707f, 0.707f, 0.0f,
    0.707f, 0.707f, 0.0f,
    0.707f, 0.707f, 0.0f,

    0.0f, 0.707f, 0.707f,
    0.0f, 0.707f, 0.707f,
    0.0f, 0.707f, 0.707f,

};
const GLuint CPyramid::IndexData[] =
    {
        3,0,2,
        2,0,1,

        0,1,4,

        3,0,4,

        2,3,4,

        4,1,2
    };

void CPyramid::make()
{
    glGenBuffers(BufferSize, BufferName);

    glBindBuffer(GL_ARRAY_BUFFER, BufferName[COLOR_OBJECT]);
	glBufferData(GL_ARRAY_BUFFER, ColorSize, ColorData, GL_STREAM_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, BufferName[NORMAL_OBJECT]);
    glBufferData(GL_ARRAY_BUFFER,NormalSize,NormalData,GL_STREAM_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, BufferName[POSITION_OBJECT]);
    glBufferData(GL_ARRAY_BUFFER, PositionSize, PositionData, GL_STREAM_DRAW);

   	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BufferName[INDEX_OBJECT]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndexSize, IndexData, GL_STREAM_DRAW);
}

void CPyramid::draw()
{
    glBindBuffer(GL_ARRAY_BUFFER, BufferName[COLOR_OBJECT]);
	glColorPointer(3, GL_FLOAT, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, BufferName[POSITION_OBJECT]);
	glVertexPointer(3, GL_FLOAT, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER,BufferName[NORMAL_OBJECT]);
	glNormalPointer(GL_FLOAT, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BufferName[INDEX_OBJECT]);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);

	glDrawElements(GL_TRIANGLES,VertexCount , GL_UNSIGNED_INT, 0);

    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);

}

void CPyramid::end()
{
    glDeleteBuffers(BufferSize,BufferName);
}
CPyramid::CPyramid()
{

}
CPyramid::~CPyramid()
{

}
//------------------------------CTriangle Class Specification-------------------------------------
//------------------------------------------------------------------------------------------------

const GLsizeiptr CTriangle::PositionSize = 3 * 3 * sizeof(GLfloat); //number of vertices*dimensions of vertex*sizeof(vertex)
const GLsizeiptr CTriangle::ColorSize = 3* 3 * sizeof(GLfloat); //number of colors*dimensions of colors*sizeof(vertex)
const GLsizeiptr CTriangle::IndexSize = 3 * sizeof(GLuint);
const GLsizei CTriangle::VertexCount = 3;

const GLfloat CTriangle::PositionData[]=
    {
        1.0f,0.0f,0.0f,
        0.0f,1.0f,0.0f,
        -1.0f,0.0f,0.0f
    };

const GLfloat CTriangle::ColorData[]=
    {
        1.0,  1.0,  0.0,
        0.0,  1.0,  0.0,
        0.0,  0.0,  1.0
    };
const GLuint CTriangle::IndexData[]=
    {
        0,1,2
    };
CTriangle::CTriangle()
{

}
CTriangle::~CTriangle()
{

}
void CTriangle::make()
{
    glGenBuffers(BufferSize, BufferName);

    glBindBuffer(GL_ARRAY_BUFFER, BufferName[COLOR_OBJECT]);
	glBufferData(GL_ARRAY_BUFFER, ColorSize, ColorData, GL_STREAM_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, BufferName[POSITION_OBJECT]);
    glBufferData(GL_ARRAY_BUFFER, PositionSize, PositionData, GL_STREAM_DRAW);

   	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BufferName[INDEX_OBJECT]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndexSize, IndexData, GL_STREAM_DRAW);
}

void CTriangle::draw()
{
    glBindBuffer(GL_ARRAY_BUFFER, BufferName[COLOR_OBJECT]);
	glColorPointer(3, GL_FLOAT, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, BufferName[POSITION_OBJECT]);
	glVertexPointer(3, GL_FLOAT, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BufferName[INDEX_OBJECT]);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

	glDrawElements(GL_TRIANGLES, VertexCount, GL_UNSIGNED_INT, 0);

    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);

}

void CTriangle::end()
{
    glDeleteBuffers(BufferSize,BufferName);
}

//------------------------------CRectangle Class Specification------------------------------------
//------------------------------------------------------------------------------------------------
const GLsizeiptr CRectangle::PositionSize = 4 * 3 * sizeof(GLfloat); //number of vertices*dimensions of vertex*sizeof(vertex)
const GLsizeiptr CRectangle::ColorSize = 4 * 3 * sizeof(GLfloat); //number of colors*dimensions of colors*sizeof(vertex)
const GLsizeiptr CRectangle::IndexSize = 6 * sizeof(GLuint); //number of colo
const GLsizei CRectangle::VertexCount = 6;

const GLfloat CRectangle::PositionData[] =
    {
        -1.0f,-1.0f,0.0f,
         1.0f,-1.0f,0.0f,
         1.0f, 1.0f,0.0f,
        -1.0f, 1.0f,0.0f,
    };
const GLfloat CRectangle::ColorData[] =
    {
        255,   0,   0,
        255, 255,   0,
          0, 255,   0,
          0,   0, 255
    };

const GLuint CRectangle::IndexData[] =
{       0, 1, 2, 3, 0, 2
};


void CRectangle::make()
{
    glGenBuffers(BufferSize, BufferName);

    glBindBuffer(GL_ARRAY_BUFFER, BufferName[COLOR_OBJECT]);
	glBufferData(GL_ARRAY_BUFFER, ColorSize, ColorData, GL_STREAM_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, BufferName[POSITION_OBJECT]);
    glBufferData(GL_ARRAY_BUFFER, PositionSize, PositionData, GL_STREAM_DRAW);

   	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BufferName[INDEX_OBJECT]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndexSize, IndexData, GL_STREAM_DRAW);


}

void CRectangle::end()
{
    glDeleteBuffers(BufferSize, BufferName);
}

void CRectangle::draw()
{


	glBindBuffer(GL_ARRAY_BUFFER, BufferName[COLOR_OBJECT]);
	glColorPointer(3, GL_FLOAT, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, BufferName[POSITION_OBJECT]);
	glVertexPointer(3, GL_FLOAT, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BufferName[INDEX_OBJECT]);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

	glDrawElements(GL_TRIANGLES, VertexCount, GL_UNSIGNED_INT, 0);

    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
}
CRectangle::CRectangle()
{

}

CRectangle::~CRectangle()
{

}


