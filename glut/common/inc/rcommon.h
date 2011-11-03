/*
 * Copyright (c) 2011, Ranjith TV
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer
 *     in the documentation and/or other materials provided with the distribution.
 *   * Neither the name the Ranjith nor the names of its contributors may be used
       to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _RCOMMON_H_
#define _RCOMMON_H_

#if defined(__SYMBIAN32__) || defined(SYMBIAN)
#include <e32base.h>
#define DBGPRINTF   RDebug::Printf
#else
#include <stdio.h>
#define DBGPRINTF   printf
#define NONSHARABLE_CLASS(X) class X
#endif

#if defined(GLUTES_DEBUG)
#define GLUTES_DEBUGP1(x1) do{DBGPRINTF("(%s:%d) " x1 "\n", __FILE__, __LINE__);}while (0)
#define GLUTES_DEBUGP2(x1, x2) do{DBGPRINTF("(%s:%d) " x1 "\n", __FILE__, __LINE__, x2);}while (0)
#define GLUTES_DEBUGP3(x1, x2, x3) do{DBGPRINTF("(%s:%d) " x1 "\n", __FILE__, __LINE__, x2, x3);}while (0)
#define GLUTES_DEBUGP4(x1, x2, x3, x4) do{DBGPRINTF("(%s:%d) " x1 "\n", __FILE__, __LINE__, x2, x3, x4);}while (0)
#define GLUTES_DEBUGP5(x1, x2, x3, x4, x5) do{DBGPRINTF("(%s:%d) " x1 "\n", __FILE__, __LINE__, x2, x3, x4, x5);}while (0)
#define GLUTES_DEBUGP6(x1, x2, x3, x4, x5, x6) do{DBGPRINTF("(%s:%d) " x1 "\n", __FILE__, __LINE__, x2, x3, x4, x5, x6);}while (0)
#define GLUTES_DEBUGP7(x1, x2, x3, x4, x5, x6, x7) do{DBGPRINTF("(%s:%d) " x1 "\n", __FILE__, __LINE__, x2, x3, x4, x5, x6, x7);}while (0)
#define GLUTES_DEBUGP8(x1, x2, x3, x4, x5, x6, x7, x8) do{DBGPRINTF("(%s:%d) " x1 "\n", __FILE__, __LINE__, x2, x3, x4, x5, x6, x7, x8);}while (0)
#define GLUTES_DEBUGP9(x1, x2, x3, x4, x5, x6, x7, x8, x9) do{DBGPRINTF("(%s:%d) " x1 "\n", __FILE__, __LINE__, x2, x3, x4, x5, x6, x7, x8, x9);}while (0)
#else
#define GLUTES_DEBUGP1(x1)
#define GLUTES_DEBUGP2(x1, x2)
#define GLUTES_DEBUGP3(x1, x2, x3)
#define GLUTES_DEBUGP4(x1, x2, x3, x4)
#define GLUTES_DEBUGP5(x1, x2, x3, x4, x5)
#define GLUTES_DEBUGP6(x1, x2, x3, x4, x5, x6)
#define GLUTES_DEBUGP7(x1, x2, x3, x4, x5, x6, x7)
#define GLUTES_DEBUGP8(x1, x2, x3, x4, x5, x6, x7, x8)
#define GLUTES_DEBUGP9(x1, x2, x3, x4, x5, x6, x7, x8, x9)
#endif

#endif

