/**
 * Copyright (c) 2010-2015, Ranjith TV
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


#if !defined(__GLUT_H__)
#define __GLUT_H__

#if defined(__SYMBIAN32__) || defined(SYMBIAN)
#include <e32def.h>
#include <e32base.h>
#define GLUT_OS_SYMBIAN
#define GLUT_ES
#define APIENTRY  IMPORT_C
#endif

#if !defined(APIENTRY)
#define APIENTRY
#endif

#if defined(GLUT_ES)
#ifdef GLUT_GLES2
#include <GLES2/gl2.h>
#elif GLUT_OPENVG
#include <VG/openvg.h>
#include <VG/vgu.h>
#else
#include <GLES/gl.h>
#endif
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#if defined(__cplusplus)
extern "C" {
#endif

#define GLUT_RGB            0
#define GLUT_RGBA           GLUT_RGB
#define GLUT_INDEX          1
#define GLUT_SINGLE         0
#define GLUT_DOUBLE         2
#define GLUT_ACCUM          4
#define GLUT_ALPHA          8
#define GLUT_DEPTH          16

#define GLUT_RED            0
#define GLUT_GREEN          1
#define GLUT_BLUE           2

/* Mouse buttons. */
#define GLUT_LEFT_BUTTON    0
#define GLUT_MIDDLE_BUTTON  1
#define GLUT_RIGHT_BUTTON   2

/* Mouse button  state. */
#define GLUT_DOWN           0
#define GLUT_UP             1

/* glutGetModifiers return mask. */
#define GLUT_ACTIVE_SHIFT   1
#define GLUT_ACTIVE_CTRL    2
#define GLUT_ACTIVE_ALT     4

APIENTRY void glutInit(int *argp, char **argv);
APIENTRY void glutInitDisplayMode(unsigned int mode);
APIENTRY void glutInitWindowPosition(int x, int y);
APIENTRY void glutInitWindowSize(int width, int height);
APIENTRY void glutMainLoop();

APIENTRY int glutCreateWindow(const char *title);
APIENTRY void glutDestroyWindow(int win);
APIENTRY void glutPostRedisplay();
APIENTRY void glutPostWindowRedisplay(int win);
APIENTRY void glutSwapBuffers();
APIENTRY int glutGetWindow();
APIENTRY void glutSetWindow(int win);
APIENTRY void glutSetWindowTitle(const char *title);
APIENTRY void glutShowWindow();
APIENTRY void glutHideWindow();
APIENTRY void glutPositionWindow(int x, int y);
APIENTRY void glutReshapeWindow(int width, int height);
APIENTRY void glutPopWindow();
APIENTRY void glutPushWindow();

APIENTRY void glutDisplayFunc(void (*func)());
APIENTRY void glutReshapeFunc(void (*func)(int, int));
APIENTRY void glutKeyboardFunc(void (*func)(unsigned char key, int x, int y));
APIENTRY void glutMouseFunc(void (*func)(int button, int state, int x, int y));
APIENTRY void glutMotionFunc(void (*func)(int x, int y));
APIENTRY void glutPassiveMotionFunc(void (*func)(int x, int y));
APIENTRY int  glutGetModifiers();
APIENTRY void glutTimerFunc(unsigned int millis, void (*func)(int), int value);
APIENTRY void glutFullScreen();

APIENTRY int glutGet(unsigned int state);

#if defined(__cplusplus)
}
#endif

#endif
