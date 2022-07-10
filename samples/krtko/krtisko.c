/*
	krtisko.c

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
#include <krtvr3d.h>

static KRUHOTVAR3D krtisko3data =  {
	NULL,				/* fname */
	NULL,				/* modname */
	{.81, .81, -1.0},	/* scales */
	5.0,				/* base */
	3.75,				/* increment */
	7,					/* layers */
	3,					/* mode */
	100,				/* smooth */
	3,					/* nobase */
	3,					/* mirror */
	true,				/* center */
	{0.75, 0.75, 0.7},	/* scale */
	.itrans.y = 62,
	.escale = {0.5, 0.7, true},
	.rotate = {-30, -25, 45},
	.twist = 10
};

static KRTLIST krtlist = { &krtisko3data, NULL };

static const char lic[] =
"\tkrtisko.scad\n"
"\n"
"\tCopyright 2022 G. Adam Stanislav\n"
"\tAll rights reserved\n"
"\n"
"\tRedistribution and use in source and binary forms,\n"
"\twith or without modification, are permitted provided\n"
"\tthat the following conditions are met:\n"
"\n"
"\t1. Redistributions of source code must retain the\n"
"\tabove copyright notice, this list of conditions\n"
"\tand the following disclaimer.\n"
"\n"
"\t2. Redistributions in binary form must reproduce the\n"
"\tabove copyright notice, this list of conditions and\n"
"\tthe following disclaimer in the documentation and/or\n"
"\tother materials provided with the distribution.\n"
"\n"
"\t3. Neither the name of the copyright holder nor the\n"
"\tnames of its contributors may be used to endorse or\n"
"\tpromote products derived from this software without\n"
"\tspecific prior written permission.\n"
"\n"
"\tTHIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS\n"
"\tAND CONTRIBUTORS \"AS IS\" AND ANY EXPRESS OR IMPLIED\n"
"\tWARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE\n"
"\tIMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS\n"
"\tFOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT\n"
"\tSHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE\n"
"\tFOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,\n"
"\tOR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,\n"
"\tPROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,\n"
"\tDATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER\n"
"\tCAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,\n"
"\tSTRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR\n"
"\tOTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS\n"
"\tSOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.\n";

int main(int argc, char*argv[]) {
	FILE *fp;
	int retval;

	krtisko3data.fname = "krtko.svg";
	krtisko3data.modname = "krtisko";
	fp = freopen("krtisko.scad", "wb", stdout);
	retval = krtvr3d_scad(&krtlist, lic);
	fclose(fp);
}
