#include "../include/prog.h"
#include "../include/CTransRot.h"
//==============================================================================
//
//  CTransRot function bodies:
//
//==============================================================================

CTransRot::~CTransRot(void)
//------------------------------------------------------------------------------
// Default destructor
{
}

CTransRot::CTransRot(void)
//------------------------------------------------------------------------------
// Default constructor
{
	reset();						// set all values to zero.
}

void CTransRot::reset(void)
//------------------------------------------------------------------------------
// Set all values to zero.
{
	x_pos = 0.0; y_pos = 0.0; z_pos = 0.0;
	x_rot = 0.0; y_rot = 0.0; z_rot = 0.0;
}

void CTransRot::apply_TR_Matrix(void)
//------------------------------------------------------------------------------
// Apply translations, then rotations to current coordinate axes.
{
	glTranslated(x_pos, y_pos, z_pos);	// trans. current coord system axes and
										// origin to make new coord system.
								// d suffix to specify 'doubles' as arguments.
	glRotated(z_rot, 0.0, 0.0, 1.0);	// rotate new coord system axes about
	glRotated(x_rot, 0.0, 1.0, 0.0);	// the current z axis, then x, then y.
	glRotated(y_rot, 1.0, 0.0, 0.0);
}

void CTransRot::apply_RT_Matrix(void)
//------------------------------------------------------------------------------
// Apply rotations, then translations to current coordinate axes.
{
	glRotated(z_rot, 0.0, 0.0, 1.0);	// rotate new coord system axes about
	glRotated(x_rot, 0.0, 1.0, 0.0);	// the current z axis, then x, then y.
	glRotated(y_rot, 1.0, 0.0, 0.0);
								// d suffix to specify 'doubles' as arguments.
	glTranslated(x_pos, y_pos, z_pos);	// trans. current coord system axes and
										// origin to make new coord system.
}

