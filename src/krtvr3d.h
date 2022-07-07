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

#include <stdio.h>
#include <stdbool.h>

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
	bool center;
	KRTXYZ scale;		// The default scale (any 0.0 is replaced by 1.0)
	KRTXYZ rotate;		// The default rotation
	KRTXYZ translate;	// The default translation
	KRTXYZ itrans;		// Translation of imported objects
	KRTXY  escale;		// Scaling of extrusion
	double twist;		// Twist the extrusion
	KRTXYZ angles;		// Only used if mode == KRVTR_CUSTOM
} KRUHOTVAR3D;

typedef struct KRTLIST {
	KRUHOTVAR3D *thisone;
	struct KRTLIST *next;
} KRTLIST;

extern KRUHOTVAR3D const krtvr_defaults;

int krtvr3d_scad(KRTLIST * const krtvar);
KRTLIST *krtvr3d_arraytolist(KRUHOTVAR3D * const krtvar, unsigned int n);
KRTLIST *krtvr3d_freelist(KRTLIST *list);

KRUHOTVAR3D *krtvr3d_resetdata(KRUHOTVAR3D *krtvar);
KRUHOTVAR3D *krtvr3d_newdata(void);
KRTLIST *krtvr3d_newlisteddata(void);
