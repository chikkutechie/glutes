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
 
#ifndef GLUTS60INTERFACE_H_
#define GLUTS60INTERFACE_H_

#include "glut.h"
#include "glutinterface.h"
#include "gluteventhandler.h"

class GlutControl;
class CEikonEnv;
class GlutGLBinder;

class GlutS60Interface: public GlutInterface, public GlutEventHandler
{
public:
    GlutS60Interface();
    ~GlutS60Interface();
    
    void intialize(int argc, char **argv);
    void terminate();
    void timerFunc(unsigned int millis, void (*func)(int), int value);
    int  createWindow();
    void destroyWindow(int win);
    int getWindow();
    void setWindow(int win);
    void setWindowTitle(const char * title);
    void showWindow();
    void fullScreen();
    void hideWindow();
    void positionWindow(int x, int y);
    void reshapeWindow(int width, int height);
    void popWindow();
    void pushWindow();
    void exec();
    void redraw(int win = 0);
    void flush();
    int getModifiers();
    
public:
    virtual void draw();
    virtual void reshape(int, int);
    virtual void rerect(int, int, int, int);
    virtual void repos(int, int);
    virtual void keyboard(unsigned char key, int x, int y);
    virtual void mouse(int button, int modifier, int x, int y);

private:
    struct ControlEntry
    {
        ControlEntry() : id(0), mSurface(0), mControl(0) {}
        int id;
        unsigned int  mSurface;
        GlutControl * mControl;
    };
    
    int addControl(ControlEntry entry);
    ControlEntry removeControl(int id);
    void removeAllControl();
    ControlEntry * getControlEntry(int id);
    
private:
    CEikonEnv* mEikonEnv;
    int mCurrentControl;
    GlutGLBinder * mBinder;
    CPeriodic * mTimer;
    RArray<ControlEntry> mControllist;
    RArray<ControlEntry> mControlStack;
    bool mFullScreen;
    int mMouseModifier;
    
    const TPtr8 KParamRenderer;
    const TPtr8 KParamRendererVG;
    const TPtr8 KParamRendererGL;
    const TPtr8 KParamRendererGLES;
    const TPtr8 KParamRendererGLES2;
    
    const TPtr8 KParamOrientation;
    const TPtr8 KParamOrientationPortrait;
    const TPtr8 KParamOrientationLandscape;
    const TPtr8 KParamOrientationAutomatic;
};

#endif
