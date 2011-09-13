#
# Copyright (c) 2010-2015, Ranjith TV
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
#   * Redistributions of source code must retain the above copyright notice,
#     this list of conditions and the following disclaimer.
#   * Redistributions in binary form must reproduce the above copyright notice,
#     this list of conditions and the following disclaimer
#     in the documentation and/or other materials provided with the distribution.
#   * Neither the name the Ranjith nor the names of its contributors may be used
#     to endorse or promote products derived
#     from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
# IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
# INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#

lib_PROGRAMME = libglut.so

glut_SOURCES = ../../src/glut.cpp \
	       ../../common/src/reglglutglbinder.cpp \
	       ../../common/src/reglproperties.cpp \
	       ../src/rglutmaemointerface.cpp
glut_INCExports = ../../inc/glut.h
glut_OBJS = $(patsubst ../src/%.cpp, %.o, $(glut_SOURCES))

CXX = g++
LIBS = -lX11
LIBDIR = 
INCLUDES =  -I../inc -I../../inc -I../../common/inc
DEFINES = -DGLUT_ES 
#-DGLUTES_DEBUG
CXXFLAGS = -g3 -Wall -fPIC  $(INCLUDES) $(DEFINES)
VPATH = ../src

all: $(lib_PROGRAMME) install

$(lib_PROGRAMME): $(glut_OBJS)
	$(CXX) -shared -Wl,-soname,$@ -o $@ $(CXXFLAGS) $(glut_OBJS)
	cp $(lib_PROGRAMME) ../lib

.o.cpp:

install:
	cp $(lib_PROGRAMME) /usr/lib
	cp $(glut_INCExports) /usr/include

clean:
	rm -rf *.o $(lib_PROGRAMME) ../lib/$(lib_PROGRAMME)

rebuild: clean all

.PHONY: all

