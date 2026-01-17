//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
// $NoKeywords: $
//=============================================================================//

#ifndef QLIMITS_H
#define QLIMITS_H

// DATA STRUCTURE INFO

#define MAX_NUM_ARGVS	50

// SYSTEM INFO
#define	MAX_QPATH		96			// max length of a game pathname
#define	MAX_OSPATH		260			// max length of a filesystem pathname

#define	ON_EPSILON		0.1			// point on plane side epsilon

// Base width and height for display.
constexpr inline int BASE_WIDTH{640};
constexpr inline int BASE_HEIGHT{480};

#endif // QLIMITS_H
