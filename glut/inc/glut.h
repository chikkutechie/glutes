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

/*
 * Symbian platform specific include files
 */
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

/*
 * GL, GLES, GLES2 or VG include files
 */
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

/* 
 * Display mode definitions
 */
#define GLUT_RGB                            0
#define GLUT_RGBA                           GLUT_RGB
#define GLUT_INDEX                          1
#define GLUT_SINGLE                         0
#define GLUT_DOUBLE                         2
#define GLUT_ACCUM                          4
#define GLUT_ALPHA                          8
#define GLUT_DEPTH                          16

/* 
 * RGB color component 
 */
#define GLUT_RED                            0
#define GLUT_GREEN                          1
#define GLUT_BLUE                           2

/*
 * The glutGet parameters
 */
#define  GLUT_WINDOW_X                      0x0064
#define  GLUT_WINDOW_Y                      0x0065
#define  GLUT_WINDOW_WIDTH                  0x0066
#define  GLUT_WINDOW_HEIGHT                 0x0067
#define  GLUT_WINDOW_BUFFER_SIZE            0x0068
#define  GLUT_WINDOW_STENCIL_SIZE           0x0069
#define  GLUT_WINDOW_DEPTH_SIZE             0x006A
#define  GLUT_WINDOW_RED_SIZE               0x006B
#define  GLUT_WINDOW_GREEN_SIZE             0x006C
#define  GLUT_WINDOW_BLUE_SIZE              0x006D
#define  GLUT_WINDOW_ALPHA_SIZE             0x006E
#define  GLUT_WINDOW_DOUBLEBUFFER           0x0073

#define  GLUT_SCREEN_WIDTH                  0x00C8
#define  GLUT_SCREEN_HEIGHT                 0x00C9
#define  GLUT_SCREEN_WIDTH_MM               0x00CA
#define  GLUT_SCREEN_HEIGHT_MM              0x00CB
#define  GLUT_MENU_NUM_ITEMS                0x012C
/* 
 * Mouse buttons.
 */
#define GLUT_LEFT_BUTTON                    0
#define GLUT_MIDDLE_BUTTON                  1
#define GLUT_RIGHT_BUTTON                   2

/* 
 * Mouse button  state.
 */
#define GLUT_DOWN                           0
#define GLUT_UP                             1

/* 
 * glutGetModifiers return mask.
 */
#define GLUT_ACTIVE_SHIFT                   1
#define GLUT_ACTIVE_CTRL                    2
#define GLUT_ACTIVE_ALT                     4

/* 
 * Initialization functions 
 */
APIENTRY void glutInit(int *argp, char **argv);
APIENTRY void glutInitDisplayMode(unsigned int mode);
APIENTRY void glutInitWindowPosition(int x, int y);
APIENTRY void glutInitWindowSize(int width, int height);

/* 
 * Main loop function 
 */
APIENTRY void glutMainLoop();

/* 
 * Window management functions 
 */
APIENTRY int glutCreateWindow(const char *title);
APIENTRY void glutDestroyWindow(int win);
APIENTRY int glutGetWindow();
APIENTRY void glutSetWindow(int win);
APIENTRY void glutSetWindowTitle(const char *title);
APIENTRY void glutShowWindow();
APIENTRY void glutHideWindow();
APIENTRY void glutPositionWindow(int x, int y);
APIENTRY void glutReshapeWindow(int width, int height);
APIENTRY void glutPopWindow();
APIENTRY void glutPushWindow();
APIENTRY void glutFullScreen();

/*
 * Display related functions 
 */
APIENTRY void glutPostRedisplay();
APIENTRY void glutPostWindowRedisplay(int win);
APIENTRY void glutSwapBuffers();

/* 
 * Callback functions 
 */
APIENTRY void glutDisplayFunc(void (*func)());
APIENTRY void glutReshapeFunc(void (*func)(int, int));
APIENTRY void glutKeyboardFunc(void (*func)(unsigned char key, int x, int y));
APIENTRY void glutMouseFunc(void (*func)(int button, int state, int x, int y));
APIENTRY void glutMotionFunc(void (*func)(int x, int y));
APIENTRY void glutPassiveMotionFunc(void (*func)(int x, int y));
APIENTRY void glutTimerFunc(unsigned int millis, void (*func)(int), int value);

/* 
 * Menu related functions 
 */
APIENTRY int glutCreateMenu(void (*)(int menu));
APIENTRY void glutDestroyMenu(int menu);
APIENTRY int glutGetMenu();
APIENTRY void glutSetMenu(int menu);
APIENTRY void glutAddMenuEntry(const char* label, int value);
APIENTRY void glutRemoveMenuItem(int item);
APIENTRY void glutAttachMenu(int button);
APIENTRY void glutDetachMenu(int button);

/* 
 * State retrieval functions 
 */
APIENTRY int  glutGetModifiers();
APIENTRY int glutGet(unsigned int state);

#if defined(__cplusplus)
}
#endif

#endif
