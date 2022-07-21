/*
	krtvr3d.h

	Copyright 2022 G. Adam Stanislav
	All rights reserved

	Redistribution and use in source and binary forms,
	with or without modification, are permitted provided
	that the following conditions are met:

	1. Redistributions of source code must retain the
	above copyright notice, this list of conditions
	and the following disclaimer.

	2. Redistributions in binary form must reproduce the
	above copyright notice, this list of conditions and
	the following disclaimer in the documentation and/or
	other materials provided with the distribution.

	3. Neither the name of the copyright holder nor the
	names of its contributors may be used to endorse or
	promote products derived from this software without
	specific prior written permission.

	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS
	AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
	WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
	IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
	FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
	SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
	FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
	OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
	PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
	DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
	CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
	STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
	OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
	SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

#ifndef	_KRTVR3D_H_
#define	_KRTVR3D_H_

#include <stdio.h>
#include <stdbool.h>


#ifdef	NOKRTLIB	// Should you need a static library...
# define	KRTHID
# define	KRTDC
#elif defined _WIN32
# define	KRTHID
#ifdef	DLL
# define	KRTDC	__declspec(dllexport)
#else	// !DLL
# define	KRTDC	__declspec(dllimport)
#endif	// DLL
#elif defined __APPLE__ || defined linux || defined __FreeBSD__
#  define KRTDC __attribute__((visibility("default")))
# define	KRTHID __attribute__((visibility("hidden")))
#else	// !_WIN32 !__APPLE__ !linux !__FreeBSD__
# define	KRTDC
# define	KRTHID
#endif	// _WIN32

#ifdef __cplusplus
extern "C" {
#endif

// These are the possible values for KRUHOTVAR3D.mode.
#define	KRVTR_X	4
#define	KRVTR_Y	2
#define	KRVTR_Z	1
#define	KRVTR_XY	(KRVTR_X | KRVTR_Y)
#define	KRVTR_XZ	(KRVTR_X | KRVTR_Z)
#define	KRVTR_YZ	(KRVTR_Y | KRVTR_Z)
#define	KRVTR_XYZ	(KRVTR_X | KRVTR_Y | KRVTR_Z)
#define	KRVTR_CUSTOM	0

typedef struct KRTXYZ {
	double x;
	double y;
	double z;
} KRTXYZ;

typedef struct KRTXY {
	double x;
	double y;
	bool   cond;
} KRTXY;

typedef struct KRUHOTVAR3D {
	char const *fname;
	char const *modname;
	KRTXYZ scales;
	double base;
	double increment;
	unsigned int layers;
	unsigned int mode;
	unsigned int smooth;
	int nobase;
	int mirror;
	int minsc;			// Minimum layer to star scaling
	bool center;
	KRTXYZ scale;		// The default scale (any 0.0 is replaced by 1.0)
	KRTXYZ rotate;		// The default rotation
	KRTXYZ translate;	// The default translation
	KRTXYZ itrans;		// Translation of imported objects
	KRTXY  escale;		// Scaling of extrusion
	double twist;		// Twist the extrusion
	KRTXYZ angles;		// Only used if mode == KRVTR_CUSTOM
	unsigned int mooth;	// How smooth should minkowski be
} KRUHOTVAR3D;

typedef struct KRTLIST {
	KRUHOTVAR3D *thisone;
	struct KRTLIST *next;
	bool allocated;		// true if thisone was dynamicaly allocated, false otherwise
} KRTLIST;

KRTDC extern KRUHOTVAR3D const krtvr_defaults;

// Verify the validity of a KRTLIST. Return true if
// any error is found, false otherwise. List all
// errors on stderr.
KRTDC bool krtvr3d_errorcheck(KRTLIST * const krtvar);

KRTDC int krtvr3d_scad(KRTLIST * const krtvar, char const * const comment);
KRTDC KRTLIST *krtvr3d_arraytolist(KRUHOTVAR3D * const krtvar, unsigned int n);
KRTDC KRTLIST *krtvr3d_freelist(KRTLIST *list);

KRTDC KRUHOTVAR3D *krtvr3d_resetdata(KRUHOTVAR3D *krtvar);
KRTDC KRUHOTVAR3D *krtvr3d_newdata(void);
KRTDC KRTLIST *krtvr3d_newlisteddata(void);

#ifdef __cplusplus
}
#endif

#endif	// defined(_KRTVR3D_H_)
