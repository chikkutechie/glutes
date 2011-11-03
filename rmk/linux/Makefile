#
# Copyright (c) 2011, Ranjith TV
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
#   * Redistributions of source code must retain the above copyright notice,
#     this list of conditions and the following disclaimer.
#   * Redistributions in binary form must reproduce the above copyright notice,
#      this list of conditions and the following disclaimer
#     in the documentation and/or other materials provided with the distribution.
#   * Neither the name the Ranjith nor the names of its contributors may be used
#       to endorse or promote products derived
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

bin_PROGRAMME = rmk

rmk_parser_INPUT = ../parser/rmk.l ../parser/rmk.y
rmk_parser_OUTPUT = ../parser/lex.yy.c ../parser/rmk.tab.c

rmk_O_SOURCE = ../src/main.cpp ../src/rmk.cpp

rmk_O_OBJS = main.o rmk.o
rmk_PARSER_OBJS = lex.yy.o rmk.tab.o

CC = gcc
CXX = g++
INCLUDE = -I../inc -I../parser
BINDIR = ./
LIBS =  -lm
CXXFLAGS = -g3 $(INCLUDE)
CFLAGS = -g3 $(INCLUDE)

all: $(bin_PROGRAMME)

$(bin_PROGRAMME): $(rmk_PARSER_OBJS) $(rmk_O_OBJS) 
	$(CXX) -o $(BINDIR)$@ $(CXXFLAGS) $(LIBDIR) $(LIBS) $(rmk_PARSER_OBJS) $(rmk_O_OBJS)

$(rmk_O_OBJS): $(rmk_O_SOURCE)
	$(CXX) -c $(CXXFLAGS)  $(rmk_O_SOURCE)

$(rmk_PARSER_OBJS): $(rmk_parser_OUTPUT) 
	$(CC) -c $(CFLAGS) $(rmk_parser_OUTPUT) 

$(rmk_parser_OUTPUT): $(rmk_parser_INPUT)
	cd ../parser && bison --defines rmk.y
	cd ../parser && flex rmk.l
	cd ../linux

clean:
	rm -rf $(rmk_parser_OUTPUT) $(rmk_PARSER_OBJS) $(rmk_O_OBJS) $(bin_PROGRAMME)

rebuild: clean all

.PHONY: all

