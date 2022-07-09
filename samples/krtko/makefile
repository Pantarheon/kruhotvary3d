#	Unix makefile for kruhotvary3d/samples/krtko
#
#	Copyright 2022 G. Adam Stanislav
#	All rights reserved
#
#	Redistribution and use in source and binary forms,
#	with or without modification, are permitted provided
#	that the following conditions are met:
#
#	1. Redistributions of source code must retain the
#	above copyright notice, this list of conditions
#	and the following disclaimer.
#
#	2. Redistributions in binary form must reproduce the
#	above copyright notice, this list of conditions and
#	the following disclaimer in the documentation and/or
#	other materials provided with the distribution.
#
#	3. Neither the name of the copyright holder nor the
#	names of its contributors may be used to endorse or
#	promote products derived from this software without
#	specific prior written permission.
#
#	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS
#	AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
#	WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
#	IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
#	FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
#	SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
#	FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
#	OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
#	PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
#	DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
#	CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
#	STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
#	OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
#	SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

CC=gcc
CFLG=-O3
CFLAGS=$(CFLG)
PREFIX=/usr
OBJS=krtko.o krtisko.o
APPS=krtko krtisko

all: $(APPS)

rebuild: clean all

krtko: krtko.o
	$(CC) $(CFLG) -o krtko krtko.o -L$(PREFIX)/lib -lkrtvr3d

krtisko: krtisko.o
	$(CC) $(CFLG) -o krtisko krtisko.o -L$(PREFIX)/lib -lkrtvr3d

%.o: %.c $(PREFIX)/include/krtvr3d.h
	$(CC) -c $(CFLAGS) $< -o $@ -I$(PREFIX)/include

install-krtko: krtko
	install -p -s krtko $(PREFIX)/bin

install-krtisko: krtisko
	install -p -s krtisko $(PREFIX)/bin

install: install-krtko install-krtisko

clean:
	rm -f $(OBJS) $(APPS)

