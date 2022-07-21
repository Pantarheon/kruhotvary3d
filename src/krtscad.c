/*
	krtscad.c

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

#include <string.h>
#include <ctype.h>
#include "krtvr3d.h"

// Produce a SCAD file, to be processed by OpenSCAD.

static char const mdl[] = "module %s__(tw = %g, es = [%g, %g], mink = 0, n, f, layers) {\n\t";

KRTDC int krtvr3d_scad(KRTLIST * const krtvar, char const * const comment) {
	KRTLIST *next;
	KRUHOTVAR3D const *krt;
	KRTXYZ uhly;
	char const *k;
	double ex, ey;
	unsigned int i, n;
	unsigned int maxlayer;

	if (krtvar == NULL) {
		fprintf(stderr, "krtvr3d_scad: Nothing to do.\n");
		return 1;
	}

	if (krtvr3d_errorcheck(krtvar)) return 2;

	// If we get here, we have found no errors in any of the
	// KRUHOTVAR3D structures passed to us as a list of inputs.
	//
	// So this is a good place to output an optional comment.
	// We *assume* the comment contains no asterisk immediately
	// followed by a slash. That would result in an invalid
	// comment.
	//
	// We leave it up to the caller to assure no such error
	// is present in the comment string passed to us.
	//
	// The purpose of this comment is to allow to include a
	// license, or a copyright notice, or both, or whatever
	// else the caller might want and wish, or perhaps even need.
	if (comment != NULL) fprintf(stdout, "/*\n%s\n*/\n\n", comment);

	// Next (or first if no comment was passed to us), we print
	// a very brief notice informing a prospective reader of our
	// engine.
	fprintf(stdout,
		"// Script created by the kruhotvary 3d engine,\n"
		"// https://github.com/Pantarheon/kruhotvary3d.git\n\n"
	);

	// Call the first main module and comment out the rest of them.
	for (n = 0, next = krtvar; next; n++, next = next->next) {
		krt = next->thisone;
		fprintf(stdout,
			"// %s();\n" + (n == 0) * 3,
			krt->modname
		);
	}

	fprintf(stdout, "\n");

	// Compile all KRUHOTVAR3D structures in the list,
	// into individual OpenSCAD modules within a single
	// output stream.
	for (next = krtvar; next; next = next->next) {
		krt = next->thisone;
		k = krt->modname;
		maxlayer = (krt->layers > 0) ? krt->layers : 1;

		if ((i = krt->mode & 7) == 0) {
			uhly.x = krt->angles.x;
			uhly.y = krt->angles.y;
			uhly.z = krt->angles.z;
		}
		else {
			uhly.x = (i & 4) ? 360.0 : 0.0;
			uhly.y = (i & 2) ? 360.0 : 0.0;
			uhly.z = (i & 1) ? 360.0 : 0.0;
		}

		if (krt->escale.cond) {
			ex = krt->escale.x;
			ey = krt->escale.x;
		}
		else {
			ex = 1.0;
			ey = 1.0;
		}

		fprintf(stdout,
			"%s_Base = %g;\n"
			"%s_Incr = %g;\n"
			"%s_Scal = [%g, %g, %g];\n"
			"%s_Minsc  = %u;\n"
			"%s_Nobase = %u;\n"
			"%s_Mirror = %u;\n\n",
			k, krt->base,
			k, krt->increment,
			k, krt->scales.x, krt->scales.y, krt->scales.z,
			k, krt->minsc,
			k, krt->nobase,
			k, krt->mirror
		);

		// Make the main module
		fprintf(stdout,
			"module %s(n = %u, tw = %g, es = [%g, %g], mink = 0, t = [%g, %g, %g], r = [%g, %g, %g], s = [%g, %g, %g]) {\n"
			"\ttranslate(t)\n"
			"\t\trotate(r)\n"
			"\t\t\tscale(s)\n"
			"\t\t\t\t%s__(tw, es, mink, n, n, n);\n"
			"}\n\n",
			k,
			maxlayer,
			krt->twist, ex, ey,
			krt->translate.x, krt->translate.y, krt->translate.z,
			krt->rotate.x, krt->rotate.y, krt->rotate.z,
			(krt->scale.x) ? krt->scale.x : 1.0,
				(krt->scale.y) ? krt->scale.y : 1.0,
					(krt->scale.z) ? krt->scale.z : 1.0,
			k
		);

		// Add a recursive private module to produce
		// the desired kruhotvar.
		fprintf(stdout, mdl, k, krt->twist, ex, ey);
		fprintf(stdout,
			"\tMirror = (%s_Mirror <= layers) ? %s_Mirror : (layers > 1) ? layers - 1 : layers;\n\n"
			"\tif (n > 0) {\n"
			"\t\t%s__(tw, es, mink, n-1, n-1, layers);\n"
			"\t\tscale((n==Mirror) ? -[1, 1, %s_Scal.z] : (n < %s_Minsc) ?  [1, 1, %s_Scal.z] : %s_Scal) rotate([%g, %g, %g]*pow(0.5,n)) %s__(tw ,es, mink, n-1, f, layers);\n"
			"\t}\n"
			"\telse if (mink > 0) {\n"
			"\t\tminkowski() {\n\t\t\t",
			k, k,
			k,
			k, k, k, k, uhly.x, uhly.y, uhly.z, k
		);
		if ((krt->itrans.x != 0) || (krt->itrans.y != 0) || (krt->itrans.z != 0))
			fprintf(stdout, "translate([%g, %g, %g]) ", krt->itrans.x, krt->itrans.y, krt->itrans.z);
		fprintf(stdout,
			"linear_extrude(height=f*%s_Incr+(((f>=%s_Nobase)&&(%s_Incr>0))?0:%s_Base), center=%s, scale=es, twist=tw, $fn=%u) import(\"%s\", center=true, $fn=%u);\n"
			"\t\t\tsphere(mink, $fn=%u);\n"
			"\t\t}\n"
			"\t}\n"
			"\telse {\n\t\t",
			k, k, k, k, (krt->center) ? "true" : "false", krt->smooth,
			krt->fname, krt->smooth,
			(krt->mooth) ? krt->mooth : krtvr_defaults.mooth
		);
		if ((krt->itrans.x != 0) || (krt->itrans.y != 0) || (krt->itrans.z != 0))
			fprintf(stdout, "translate([%g, %g, %g]) ", krt->itrans.x, krt->itrans.y, krt->itrans.z);
		fprintf(stdout,
			"linear_extrude(height=f*%s_Incr+((f>=%s_Nobase)?0:%s_Base), center=%s, scale=es, twist=tw, $fn=%u) import(\"%s\", center=true, $fn=%u);\n"
			"\t}\n"
			"}\n\n",
			k, k, k, (krt->center) ? "true" : "false", krt->smooth, krt->fname, krt->smooth
		);
	}

	// All done, return success. The caller may—or may not—use it
	// as the return value of the program.
	//
	// At any rate, for us it is Hasta la Pasta!
	return 0;
}
