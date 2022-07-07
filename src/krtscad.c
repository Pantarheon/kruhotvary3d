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

static char const mdl[] = "module %s_%u(tw = %g, es = [%g, %g], f = %u) {\n\t";

int krtvr3d_scad(KRTLIST * const krtvar) {
	KRTLIST *next, *prev;
	KRUHOTVAR3D const *krt;
	KRTXYZ uhly;
	char const *k;
	double ex, ey;
	unsigned int i, n;
	unsigned int maxlayer;
	bool err = false;

	if (krtvar == NULL) {
		fprintf(stderr, "krtvr3d_scad: Nothing to do.\n");
		return 1;
	}

	for (n = 0, next = krtvar; next; n++, next = next->next) {
		krt = next->thisone;
		k   = krt->modname;

		if (krt->fname == NULL) {
			fprintf(stderr, "krtvr3d_scad: Missing input file name in KRUHOTVAR3D[%u].\n", n);
			err = true;
		}

		if (k == NULL) {
			fprintf(stderr, "krtvr3d_scad: Missing module name in KRUHOTVAR3D[%u].\n", n);
			err = true;
		}
		else {
			if ((k[0] >= '0') && (k[0] <= '9')) {
				fprintf(stderr,
					"krtvr3d_scad: Module name '%s' in KRUHOTVAR3D[%u] starts with a digit.\n",
					k, n
				);
				err = true;
			}
			for (i = 1; i < strlen(k); i++) {
				if ((isalnum(k[i]) == 0) && (k[i] != '_')) {
					fprintf(stderr,
						"krtvr3d_scad: Invalid character(s) in module name '%s' in KRUHOTVAR3D[%u].\n",
						k, n
					);
					err = true;
					break;
				}
			}
			if (n) for (i = 0, prev = krtvar; i < n; i++, prev = prev->next) {
				if (!strcmp(k, prev->thisone->modname)) {
					fprintf(stderr,
						"krtvr3d_scad: Duplicate module name '%s' in KRUHOTVAR3D[%u] and KRUHOTVAR3D[%u].\n",
						k, i, n
					);
					err = true;
				}
			}
		}
	}
	if (err) return 2;

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
			"%s_Scal = [%g, %g, %g];\n\n",
			k, krt->base,
			k, krt->increment,
			k, krt->scales.x, krt->scales.y, krt->scales.z
		);

		// Make the main module
		fprintf(stdout,
			"module %s(tw = %g, es=[%g, %g], t = [%g, %g, %g], r = [%g, %g, %g], s = [%g, %g, %g]) {\n"
			"\ttranslate(t)\n"
			"\t\trotate(r)\n"
			"\t\t\tscale(s)\n"
			"\t\t\t\t%s_%u(tw, es);\n"
			"}\n\n",
			k,
			krt->twist, ex, ey,
			krt->translate.x, krt->translate.y, krt->translate.z,
			krt->rotate.x, krt->rotate.y, krt->rotate.z,
			(krt->scale.x) ? krt->scale.x : 1.0,
				(krt->scale.y) ? krt->scale.y : 1.0,
					(krt->scale.z) ? krt->scale.z : 1.0,
			k, maxlayer
		);

		for (i = maxlayer; i; i--) {
			fprintf(stdout, mdl, k, i, krt->twist, ex, ey, i);
			fprintf(stdout, "%s_%u(tw, es);\n", k, i-1);
			if ((signed)i >= krt->nobase) fprintf(stdout, (i == krt->mirror) ? "\tscale(-%s_Scal) " : "\tscale(%s_Scal) ", k);
			else  fprintf(stdout, (i == krt->mirror) ? "\tscale(-[1, 1, %s_Scal.z]) " : "\tscale([1, 1, %s_Scal.z]) ", k);
		fprintf(stdout, "rotate([%g, %g, %g]/%u) %s_%u(tw,es,f);\n}\n\n", uhly.x, uhly.y, uhly.z, 1 << i, k, i-1);
		}

		fprintf(stdout, "module %s_0(tw = %g, es=[%g, %g], f = 0) {\n\t", k,  krt->twist, ex, ey);
		if ((krt->itrans.x != 0) || (krt->itrans.y != 0) || (krt->itrans.y != 0))
			fprintf(stdout, "translate([%g, %g, %g]) ", krt->itrans.x, krt->itrans.y, krt->itrans.z);
		fprintf(stdout,
			"linear_extrude(height=f*%s_Incr+((f>=%i)?0:%s_Base), center=%s, scale=es, twist=tw, $fn=%u) "
			"import(\"%s\", center=true, $fn=%u);\n}\n\n",
			k, krt->nobase, k, (krt->center) ? "true" : "false", krt->smooth,
			krt->fname, krt->smooth
		);
	}

	return 0;
}
