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

#ifndef _RGLUTANDROIDINTERFACE_H_
#define _RGLUTANDROIDINTERFACE_H_

#include "glut.h"
#include "rglutinterface.h"
#include "rgluteventhandler.h"
#include "rglutglbinder.h"

#include <string>

class REGLGlutGLBinder;
class RGlutTimer;

class RGlutAndroidInterface: public RGlutInterface, public RGlutEventHandler
{
public:
    RGlutAndroidInterface();
    ~RGlutAndroidInterface();

    void intialize(int argc, char **argv);
    void terminate();
    void initDisplayMode(unsigned int mode);

    int createWindow();
    void destroyWindow(int win);
    int getWindow();
    void setWindow(int win);
    void setWindowTitle(const char *title);
    void showWindow();
    void fullScreen();
    void hideWindow();
    void positionWindow(int x, int y);
    void reshapeWindow(int width, int height);
    void popWindow();
    void pushWindow();

    void timerFunc(unsigned int millis, void (*func)(int), int value);

    void exec();
    void redraw(int win);
    void flush();

    int createMenu(void ( *)(int menu));
    void destroyMenu(int menu);
    int getMenu();
    void setMenu(int menu);
    void addMenuEntry(const char *label, int value);
    void removeMenuItem(int item);
    void attachMenu(int button);
    void detachMenu(int button);

    int getModifiers();
    int getValue(unsigned int state);

    void draw();
    void reshape(int w, int h);
    void rerect(int x, int y, int w, int h);
    void repos(int x, int y);
    void keyboard(unsigned char key, unsigned int modifier, int x, int y);
    void mouse(int button, int modifier, int x, int y, bool pressed = true);
    void motion(int button, int modifier, int x, int y);
    void removeTimer(RGlutTimer *timer);

private:
    RGlutAndroidInterface(RGlutAndroidInterface const &);
    RGlutAndroidInterface &operator=(RGlutAndroidInterface const &);

    void parseArguments(int argc, char **argv);
    void createBinder();

private:
    private:
    const std::string KParamRenderer;
    const std::string KParamRendererVG;
    const std::string KParamRendererGL;
    const std::string KParamRendererGLES;
    const std::string KParamRendererGLES2;
    const std::string KParamOrientation;
    const std::string KParamOrientationPortrait;
    const std::string KParamOrientationLandscape;
    const std::string KParamOrientationAutomatic;

private:
    RGlutGLBinder * mBinder;
    unsigned int mDisplayMode;
};

#endif

