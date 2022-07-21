/*
	krterrck.c

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

// Check for the validity of a KRUHOTVAR3D list.

KRTDC bool krtvr3d_errorcheck(KRTLIST * const krtvar) {
	KRTLIST *next, *prev;
	KRUHOTVAR3D const *krt;
	char const *k;
	unsigned int i, n;
	bool err = false;

	for (n = 0, next = krtvar; next; n++, next = next->next) {
		krt = next->thisone;
		k   = krt->modname;

		if (krt->fname == NULL) {
			fprintf(stderr, "krtvr3d error: Missing input file name in KRUHOTVAR3D[%u].\n", n);
			err = true;
		}

		if (k == NULL) {
			fprintf(stderr, "krtvr3d error: Missing module name in KRUHOTVAR3D[%u].\n", n);
			err = true;
		}
		else {
			if ((k[0] >= '0') && (k[0] <= '9')) {
				fprintf(stderr,
					"krtvr3d error: Module name '%s' in KRUHOTVAR3D[%u] starts with a digit.\n",
					k, n
				);
				err = true;
			}
			for (i = 1; i < strlen(k); i++) {
				if ((isalnum(k[i]) == 0) && (k[i] != '_')) {
					fprintf(stderr,
						"krtvr3d error: Invalid character(s) in module name '%s' in KRUHOTVAR3D[%u].\n",
						k, n
					);
					err = true;
					break;
				}
			}
			if (n) for (i = 0, prev = krtvar; i < n; i++, prev = prev->next) {
				if (!strcmp(k, prev->thisone->modname)) {
					fprintf(stderr,
						"krtvr3d error: Duplicate module name '%s' in KRUHOTVAR3D[%u] and KRUHOTVAR3D[%u].\n",
						k, i, n
					);
					err = true;
				}
			}
		}

		if (((krt->mode & KRVTR_XYZ) == KRVTR_CUSTOM) &&
			(
				(!((krt->angles.x == 360.0) || (krt->angles.y == 360.0) || (krt->angles.z == 360.0))) ||
				(krt->angles.x > 360.0) || (krt->angles.x < 0.0) ||
				(krt->angles.y > 360.0) || (krt->angles.y < 0.0) ||
				(krt->angles.z > 360.0) || (krt->angles.z < 0.0)
			)
		) {
			fprintf(stderr, 
				"krtvr3d error: In KRUHOTVAR3D[%u]=[%g,%g,%g], at least one angle must be 360, all must be <= 360 and >= 0.\n",
				n, krt->angles.x, krt->angles.y, krt->angles.z
			);
		}
	}
	return err;
}
