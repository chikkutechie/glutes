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

#ifndef GLUTINTERFACE_H_
#define GLUTINTERFACE_H_

#include "rcommon.h"

/**
 * @class RGlutInterface
 * @brief Base class for the implementation of glut for different platform.
 *        If glut should be ported on to a new platform derive from this -
 *        class.
 */
class RGlutInterface
{
public:
    static const int TitleLength = 255;

public:
    enum Renderer {
        GL,
        GLES,
        GLES2,
        VG
    };

    enum Orientation {
        Portrait,
        Landscape,
        Automatic
    };

    RGlutInterface() {}
    virtual ~RGlutInterface() {}

    static RGlutInterface *getInterface();
    static bool destroyInterface();

    /*!
     * Does the initialization. glutInit functionality will be implemented by this function.
     */
    virtual void intialize(int argc, char **argv) = 0;
    
    /*!
     * Terminate the glut.
     */
    virtual void terminate() = 0;
    
    /*!
     * Sets the mode of glut. glutInitDisplayMode functionality will be implemented by this function.
     */
    virtual void initDisplayMode(unsigned int mode) = 0;

    /*!
     * Creates a glut window. glutCreateWindow functionality will be implemented by this function.
     */
    virtual int createWindow() = 0;
    
    /*!
     * glutDestroyWindow functionality will be implemented by this function.
     */
    virtual void destroyWindow(int win) = 0;
    
    /*!
     * glutGetWindow functionality will be implemented by this function.
     */
    virtual int getWindow() = 0;
    
    /*!
     * glutSetWindow functionality will be implemented by this function.
     */
    virtual void setWindow(int win) = 0;
    
    /*!
     * glutSetWindowTitle functionality will be implemented by this function.
     */
    virtual void setWindowTitle(const char *title) = 0;
    
    /*!
     * glutShowWindow functionality will be implemented by this function.
     */
    virtual void showWindow() = 0;
    
    /*!
     * glutFullScreen functionality will be implemented by this function.
     */
    virtual void fullScreen() = 0;
    
    /*!
     * glutHideWindow functionality will be implemented by this function.
     */
    virtual void hideWindow() = 0;
    
    /*!
     * glutPositionWindow functionality will be implemented by this function.
     */
    virtual void positionWindow(int x, int y) = 0;
    
    /*!
     * glutReshapeWindow functionality will be implemented by this function.
     */
    virtual void reshapeWindow(int width, int height) = 0;
    
    /*!
     * glutPopWindow functionality will be implemented by this function.
     */
    virtual void popWindow() = 0;
    
    /*!
     * glutPushWindow functionality will be implemented by this function.
     */
    virtual void pushWindow() = 0;

    /*!
     * glutTimerFunc functionality will be implemented by this function.
     */
    virtual void timerFunc(unsigned int millis, void (*func)(int), int value) = 0;

    /*!
     * glutMainLoop functionality will be implemented by this function.
     */
    virtual void exec() = 0;
    
    /*!
     * glutPostRedisplay functionality will be implemented by this function.
     */
    virtual void redraw(int win = 0) = 0;
    
    /*!
     * glutSwapBuffers functionality will be implemented by this function.
     */
    virtual void flush() = 0;

    /*!
     * glutCreateMenu functionality will be implemented by this function.
     */
    virtual int createMenu(void ( *)(int menu)) = 0;
    
    /*!
     * glutDestroyMenu functionality will be implemented by this function.
     */
    virtual void destroyMenu(int menu) = 0;
    
    /*!
     * glutGetMenu functionality will be implemented by this function.
     */
    virtual int getMenu() = 0;
    
    /*!
     * glutSetMenu functionality will be implemented by this function.
     */
    virtual void setMenu(int menu) = 0;
    
    /*!
     * glutAddMenuEntry functionality will be implemented by this function.
     */
    virtual void addMenuEntry(const char *label, int value) = 0;
    
    /*!
     * glutRemoveMenuItem functionality will be implemented by this function.
     */
    virtual void removeMenuItem(int item) = 0;
    
    /*!
     * glutAttachMenu functionality will be implemented by this function.
     */
    virtual void attachMenu(int button) = 0;
    
    /*!
     * glutDetachMenu functionality will be implemented by this function.
     */
    virtual void detachMenu(int button) = 0;

    /*!
     * glutGetModifiers functionality will be implemented by this function.
     */
    virtual int getModifiers() = 0;
    
    /*!
     * glutGet functionality will be implemented by this function.
     */
    virtual int getValue(unsigned int state) = 0;

public:
    /*!
     * glutKeyboardFunc functionality will be implemented by this function.
     */
    void setKeyboardFunc(void (*func)(unsigned char key, int x, int y)) {
        mCallbacks.keyboard = func;
    }

    /*!
     * glutMouseFunc functionality will be implemented by this function.
     */
    void setMouseFunc(void (*func)(int button, int state, int x, int y)) {
        mCallbacks.mouse = func;
    }

    /*!
     * glutMotionFunc functionality will be implemented by this function.
     */
    int setMotionFunc(void (*func)(int x, int y)) {
        mCallbacks.motion = func;
        return 0;
    }

    /*!
     * glutPassiveMotionFunc functionality will be implemented by this function.
     */
    int setPassiveMotionFunc(void (*func)(int x, int y)) {
        mCallbacks.passiveMotion = func;
        return 0;
    }

    void setRenderer(Renderer v) {
        this->mRenderer = v;
    }

    Renderer getRenderer() const {
        return mRenderer;
    }

    void setOrientation(Orientation v) {
        this->mOrientation = v;
    }

    Orientation getOrientation() const {
        return mOrientation;
    }

    void setPos(int x, int y) {
        mWindowProperty.mX = x;
        mWindowProperty.mY = y;
    }

    void setSize(int w, int h) {
        mWindowProperty.mWidth  = w;
        mWindowProperty.mHeight = h;
    }

    void setTitle(const char *title) {
        int i = 0;
        for (i = 0; title[i] != '\0'; ++i) {
            mWindowProperty.mTitle[i] = title[i];
        }
        mWindowProperty.mTitle[i] = '\0';
    }

    void setDrawFnc(void (*draw)()) {
        mCallbacks.draw = draw;
    }

    void setReshapeFunc(void (*reshape)(int, int)) {
        mCallbacks.reshape = reshape;
    }

private:
    RGlutInterface(RGlutInterface const &);
    RGlutInterface &operator=(RGlutInterface const &);

protected:
    struct GlutWindowProperty {
        GlutWindowProperty()
            : mX(0),
              mY(0),
              mWidth(0),
              mHeight(0) {
            mTitle[0] = '\0';
        }

        char mTitle[TitleLength];
        int mX;
        int mY;
        int mWidth;
        int mHeight;
    };

    struct GlutCallbacks {
        GlutCallbacks()
            : draw(0),
              reshape(0),
              repos(0),
              keyboard(0),
              mouse(0),
              motion(0),
              passiveMotion(0)
        {}

        void (*draw)();
        void (*reshape)(int, int);
        void (*repos)(int, int);
        void (*keyboard)(unsigned char key, int x, int y);
        void (*mouse)(int button, int state, int x, int y);
        void (*motion)(int x, int y);
        void (*passiveMotion)(int x, int y);
    };

    GlutWindowProperty mWindowProperty;
    GlutCallbacks mCallbacks;
    Renderer mRenderer;
    Orientation mOrientation;
};

#endif

