/*
	krtvr3d.c

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

#include "krtvr3d.h"

char const kruhotvar[] = "kruhotvar3D";
static char const mdl[] = "module %s_%u(f=%u) {\n\t";

int ktvar3d(KRUHOTVAR3D const *krt) {
	KRTXYZ uhly;
	char const *k;
	unsigned int i;
	unsigned int maxlayer;

  	if (krt->fname == NULL) return 1;

	k = (krt->vname) ? krt->vname : kruhotvar;
	maxlayer = (krt->layers > 0) ? krt->layers : 1;

	if ((i = krt->mode & 7) == 0) i = 7;
	uhly.x = (i & 4) ? 360.0 : 0.0;
	uhly.y = (i & 2) ? 360.0 : 0.0;
	uhly.z = (i & 1) ? 360.0 : 0.0;

	fprintf(stdout,
		"// Script created by the kruhotvary 3d engine,\n"
		"// https://github.com/Pantarheon/kruhotvary3d.git\n\n"
		"%s_Base = %g;\n"
		"%s_Incr = %g;\n"
		"%s_Scal = [%g, %g, %g];\n\n"
		"%s_%i();\n\n",
		k, krt->base,
		k, krt->increment,
		k, krt->scales.x, krt->scales.y, krt->scales.z,
		k, maxlayer);

	for (i = maxlayer; i; i--) {
		fprintf(stdout, mdl, k, i, i);
		fprintf(stdout, "\t%s_%u();\n", k, i-1);
		if ((signed)i >= krt->nobase) fprintf(stdout, (i == krt->mirror) ? "\tscale(-%s_Scal) " : "\tscale(%s_Scal) ", k);
		else  fprintf(stdout, (i == krt->mirror) ? "\tscale(-[1, 1, %s_Scal.z]) " : "\tscale([1, 1, %s_Scal.z]) ", k);
	fprintf(stdout, "rotate([%g, %g, %g]/%u) %s_%u(f);\n}\n\n", uhly.x, uhly.y, uhly.z, 1 << i, k, i-1);
	}

	fprintf(stdout,
		"module %s_0(f=0) {\n"
		"\tlinear_extrude(height=f*%s_Incr+((f>=%i)?0:%s_Base), center=%s, $fn=%u) "
		"import(\"%s\", center=true, $fn=%u);\n}\n\n",
		k, k, krt->nobase, k, (krt->center) ? "true" : "false", krt->smooth,
		krt->fname, krt->smooth
	);

	return 0;
}
