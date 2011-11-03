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

/*! \mainpage glutes
 *
 * \section intro_sec Introduction
 *
 * glut(es) is a opengles/openvg utility toolkit implementation for mobile
 * platform.<br>
 * It is a striped down version of glut, so it is only providing basic
 * functionality.<br>
 * Currently glutes implementation is provided for Symbian3 and Maemo.<br>
 * Visit http://code.google.com/p/glutes/ for more information.
 */

#include "glut.h"
#include "rglutinterface.h"

#if defined(GLUT_OS_SYMBIAN)
#define APIDEF  EXPORT_C
#endif

#ifndef APIDEF
#define APIDEF
#endif

#ifdef __cplusplus
extern "C" {
#endif

    inline RGlutInterface *getInterface()
    {
        return RGlutInterface::getInterface() ;
    }


    APIDEF void glutInit(int *argp, char **argv)
    {
        RGlutInterface *interface = getInterface();
        if (interface) {
            interface->intialize(*argp, argv);
        }
    }

    APIDEF void glutInitDisplayMode(unsigned int mode)
    {
        RGlutInterface *interface = getInterface();
        if (interface) {
            interface->initDisplayMode(mode);
        }
    }

    APIDEF void glutInitWindowPosition(int x, int y)
    {
        RGlutInterface *interface = getInterface();
        if (interface) {
            interface->setPos(x, y);
        }
    }

    APIDEF void glutInitWindowSize(int width, int height)
    {
        RGlutInterface *interface = getInterface();
        if (interface) {
            interface->setSize(width, height);
        }
    }

    APIDEF int glutCreateWindow(const char *title)
    {
        int window = 0;

        RGlutInterface *interface = getInterface();
        if (interface) {
            interface->setTitle(title);
            window = interface->createWindow();
        }

        return window;
    }

    APIDEF void glutDestroyWindow(int win)
    {
        RGlutInterface *interface = getInterface();
        if (interface) {
            interface->destroyWindow(win);
        }
    }

    APIDEF void glutPostRedisplay()
    {
        RGlutInterface *interface = getInterface();
        if (interface) {
            interface->redraw();
        }
    }

    APIDEF int glutGetWindow()
    {
        int win = 0;

        RGlutInterface *interface = getInterface();
        if (interface) {
            win = interface->getWindow();
        }

        return win;
    }

    APIDEF void glutSetWindow(int win)
    {
        RGlutInterface *interface = getInterface();
        if (interface) {
            interface->setWindow(win);
        }
    }

    APIDEF void glutPostWindowRedisplay(int win)
    {
        RGlutInterface *interface = getInterface();
        if (interface) {
            interface->redraw(win);
        }
    }

    APIDEF void glutSetWindowTitle(const char *title)
    {
        RGlutInterface *interface = getInterface();
        if (interface) {
            interface->setWindowTitle(title);
        }
    }

    APIDEF void glutShowWindow()
    {
        RGlutInterface *interface = getInterface();
        if (interface) {
            interface->showWindow();
        }
    }

    APIDEF void glutFullScreen()
    {
        RGlutInterface *interface = getInterface();
        if (interface) {
            interface->fullScreen();
        }
    }

    APIDEF void glutHideWindow()
    {
        RGlutInterface *interface = getInterface();
        if (interface) {
            interface->hideWindow();
        }
    }

    APIDEF void glutPositionWindow(int x, int y)
    {
        RGlutInterface *interface = getInterface();
        if (interface) {
            interface->positionWindow(x, y);
        }
    }

    APIDEF void glutReshapeWindow(int width, int height)
    {
        RGlutInterface *interface = getInterface();
        if (interface) {
            interface->reshapeWindow(width, height);
        }
    }

    APIDEF void glutPopWindow()
    {
        RGlutInterface *interface = getInterface();
        if (interface) {
            interface->popWindow();
        }
    }

    APIDEF void glutPushWindow()
    {
        RGlutInterface *interface = getInterface();
        if (interface) {
            interface->pushWindow();
        }
    }

    APIDEF void glutSwapBuffers()
    {
        RGlutInterface *interface = getInterface();
        if (interface) {
            interface->flush();
        }
    }

    APIDEF void glutMainLoop()
    {
        RGlutInterface *interface = getInterface();
        if (interface) {
            interface->exec();
        }
        RGlutInterface::destroyInterface();
    }

    APIDEF void glutTimerFunc(unsigned int millis, void (*func)(int), int value)
    {
        RGlutInterface *interface = getInterface();
        if (interface) {
            interface->timerFunc(millis, func, value);
        }
    }

    APIDEF void glutDisplayFunc(void (*func)())
    {
        RGlutInterface *interface = getInterface();
        if (interface) {
            interface->setDrawFnc(func);
        }
    }

    APIDEF void glutReshapeFunc(void (*func)(int, int))
    {
        RGlutInterface *interface = getInterface();
        if (interface) {
            interface->setReshapeFunc(func);
        }
    }

    APIDEF void glutKeyboardFunc(void (*func)(unsigned char key, int x, int y))
    {
        RGlutInterface *interface = getInterface();
        if (interface) {
            interface->setKeyboardFunc(func);
        }
    }

    APIDEF void glutMouseFunc(void (*func)(int button, int state, int x, int y))
    {
        RGlutInterface *interface = getInterface();
        if (interface) {
            interface->setMouseFunc(func);
        }
    }

    APIDEF void glutMotionFunc(void (*func)(int x, int y))
    {
        RGlutInterface *interface = getInterface();
        if (interface) {
            interface->setMotionFunc(func);
        }
    }

    APIDEF void glutPassiveMotionFunc(void (*func)(int x, int y))
    {
        RGlutInterface *interface = getInterface();
        if (interface) {
            interface->setPassiveMotionFunc(func);
        }
    }

    APIDEF int glutGetModifiers()
    {
        RGlutInterface *interface = getInterface();
        if (interface) {
            return interface->getModifiers();
        }

        return 0;
    }

    APIDEF int glutGet(unsigned int state)
    {
        RGlutInterface *interface = getInterface();
        if (interface) {
            return interface->getValue(state);
        }

        return 0;
    }

    APIDEF int glutCreateMenu(void (*menu)(int))
    {
        RGlutInterface *interface = getInterface();
        if (interface) {
            return interface->createMenu(menu);
        }

        return 0;
    }

    APIDEF void glutDestroyMenu(int menu)
    {
        RGlutInterface *interface = getInterface();
        if (interface) {
            interface->destroyMenu(menu);
        }
    }

    APIDEF int glutGetMenu()
    {
        RGlutInterface *interface = getInterface();
        if (interface) {
            return interface->getMenu();
        }

        return 0;
    }

    APIDEF void glutSetMenu(int menu)
    {
        RGlutInterface *interface = getInterface();
        if (interface) {
            interface->setMenu(menu);
        }
    }

    APIDEF void glutAddMenuEntry(const char *label, int value)
    {
        RGlutInterface *interface = getInterface();
        if (interface) {
            interface->addMenuEntry(label, value);
        }
    }

    APIDEF void glutRemoveMenuItem(int item)
    {
        RGlutInterface *interface = getInterface();
        if (interface) {
            interface->removeMenuItem(item);
        }
    }

    APIDEF void glutAttachMenu(int button)
    {
        RGlutInterface *interface = getInterface();
        if (interface) {
            interface->attachMenu(button);
        }
    }

    APIDEF void glutDetachMenu(int button)
    {
        RGlutInterface *interface = getInterface();
        if (interface) {
            interface->detachMenu(button);
        }
    }

#ifdef __cplusplus
}
#endif


