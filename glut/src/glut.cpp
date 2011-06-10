/**
 * Copyright (c) 2010-2015 Ranjith TV.            
 * All rights reserved.  
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *    This product includes software developed by the <organization>.
 * 4. Neither the name of the <organization> nor the
 *    names of its contributors may be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY Ranjith TV ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL Ranjith TV BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "glut.h"
#include "glutinterface.h"

#if defined(GLUT_OS_SYMBIAN)
#define APIDEF  EXPORT_C
#endif

#ifndef APIDEF
#define APIDEF
#endif

#ifdef __cplusplus
extern "C" {
#endif

inline GlutInterface * getInterface()
{
    return GlutInterface::getInterface() ;
}


APIDEF void glutInit(int *argp, char **argv)
{
    GlutInterface * interface = getInterface(); 
    if (interface) {
        interface->intialize(*argp, argv);    
    }
}

APIDEF void glutInitDisplayMode(unsigned int mode)
{
    GlutInterface * interface = getInterface(); 
    if (interface) {
        interface->setDisplayMode(mode);
    }
} 

APIDEF void glutInitWindowPosition(int x, int y)
{
    GlutInterface * interface = getInterface(); 
    if (interface) {
        interface->setPos(x, y);
    }
}

APIDEF void glutInitWindowSize(int width, int height)
{
    GlutInterface * interface = getInterface(); 
    if (interface) {
        interface->setSize(width, height);
    }
}

APIDEF int glutCreateWindow(const char *title)
{
    int window = 0;
    
    GlutInterface * interface = getInterface(); 
    if (interface) {
        interface->setTitle(title);
        window = interface->createWindow();
    }
    
    return window;
}

APIDEF void glutDestroyWindow(int win)
{
    GlutInterface * interface = getInterface(); 
    if (interface) {
        interface->destroyWindow(win);
    }
}

APIDEF void glutPostRedisplay()
{
    GlutInterface * interface = getInterface(); 
    if (interface) {
        interface->redraw();
    }
}

APIDEF int glutGetWindow()
{
    int win = 0;
    
    GlutInterface * interface = getInterface(); 
    if (interface) {
        win = interface->getWindow();
    }
    
    return win;
}

APIDEF void glutSetWindow(int win)
{
    GlutInterface * interface = getInterface(); 
    if (interface) {
        interface->setWindow(win);
    }
}

APIDEF void glutPostWindowRedisplay(int win)
{
    GlutInterface * interface = getInterface(); 
    if (interface) {
        interface->redraw(win);
    }
}

APIDEF void glutSetWindowTitle(const char *title)
{
    GlutInterface * interface = getInterface(); 
    if (interface) {
        interface->setWindowTitle(title);
    }
}

APIDEF void glutShowWindow()
{
    GlutInterface * interface = getInterface(); 
    if (interface) {
        interface->showWindow();
    }
}

APIDEF void glutFullScreen()
{
    GlutInterface * interface = getInterface(); 
    if (interface) {
        interface->fullScreen();
    }
}

APIDEF void glutHideWindow()
{
    GlutInterface * interface = getInterface(); 
    if (interface) {
        interface->hideWindow();
    }
}

APIDEF void glutPositionWindow(int x, int y)
{
    GlutInterface * interface = getInterface(); 
    if (interface) {
        interface->positionWindow(x, y);
    }
}

APIDEF void glutReshapeWindow(int width, int height)
{
    GlutInterface * interface = getInterface(); 
    if (interface) {
        interface->reshapeWindow(width, height);
    }
}

APIDEF void glutPopWindow()
{
    GlutInterface * interface = getInterface(); 
    if (interface) {
        interface->popWindow();
    }
}

APIDEF void glutPushWindow()
{
    GlutInterface * interface = getInterface(); 
    if (interface) {
        interface->pushWindow();
    }
}

APIDEF void glutSwapBuffers()
{
    GlutInterface * interface = getInterface(); 
    if (interface) {
        interface->flush();
    }
}

APIDEF void glutMainLoop()
{
    GlutInterface * interface = getInterface(); 
    if (interface) {
        interface->exec();
    }
    GlutInterface::destroyInterface();
}

APIDEF void glutTimerFunc(unsigned int millis, void (*func)(int), int value)
{
    GlutInterface * interface = getInterface(); 
    if (interface) {
        interface->timerFunc(millis, func, value);
    }
}

APIDEF void glutDisplayFunc(void (*func)())
{
    GlutInterface * interface = getInterface(); 
    if (interface) {
        interface->setDrawFnc(func);
    }
}

APIDEF void glutReshapeFunc(void (*func)(int, int))
{
    GlutInterface * interface = getInterface(); 
    if (interface) {
        interface->setReshapeFunc(func);
    }
}

APIDEF void glutKeyboardFunc(void (*func)(unsigned char key, int x, int y))
{
    GlutInterface * interface = getInterface(); 
    if (interface) {
        interface->setKeyboardFunc(func);
    }
}

APIDEF void glutMouseFunc(void (*func)(int button, int state, int x, int y))
{
    GlutInterface * interface = getInterface(); 
    if (interface) {
        interface->setMouseFunc(func);
    }
}

APIDEF void glutMotionFunc(void (*func)(int x, int y))
{   
    GlutInterface * interface = getInterface(); 
    if (interface) {
        interface->setMotionFunc(func);
    }
}

APIDEF void glutPassiveMotionFunc(void (*func)(int x, int y))
{
    GlutInterface * interface = getInterface(); 
    if (interface) {
        interface->setPassiveMotionFunc(func);
    }
}

APIDEF int glutGetModifiers()
{
    GlutInterface * interface = getInterface(); 
    if (interface) {
        return interface->getModifiers();
    }    

    return 0;
}

APIDEF int glutGet(GLenum state)
{
	return 0;
}

#ifdef __cplusplus
}
#endif


