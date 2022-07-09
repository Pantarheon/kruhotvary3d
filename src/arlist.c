/*
	arlist.c

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
#include <stdlib.h>
#include "krtvr3d.h"

/* Convert an array of KRUHOTVAR3D structures to a linked list. */

KRTDC KRTLIST *krtvr3d_arraytolist(KRUHOTVAR3D * const krtvar, unsigned int n) {
	KRTLIST *krtlist = NULL;
	KRTLIST *last, *next;
	unsigned int i;

	for (i = 0, last = NULL; i < n; i++) {
		if ((next = (KRTLIST *)calloc(sizeof(KRTLIST), 1)) == NULL) return krtlist;
		next->thisone = krtvar + i;
		if (krtlist == NULL) krtlist = next;
		else last->next = next;
		last = next;
	}
	return krtlist;
}

KRTDC KRTLIST *krtvr3d_freelist(KRTLIST *list) {
	KRTLIST *next, *first;
	if (list != NULL) for (first = list, next = list->next; first; first = next){
		if (first->allocated) {
			if (first->thisone != NULL) {
				free(first->thisone);
				first->thisone = NULL;
			}
			first->allocated = false;
		}
		free(first);
	}
	return NULL;
}
