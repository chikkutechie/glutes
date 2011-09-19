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

#ifndef _GLUTMAEMOINTERFACE_H_
#define _GLUTMAEMOINTERFACE_H_

#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>

#include <string>
#include <vector>
#include <stack>
#include <set>

#include "glut.h"
#include "rglutinterface.h"
#include "rgluteventhandler.h"
#include "rglutdisplay.h"
#include "rglutwindow.h"
#include "rglutmenu.h"

class REGLGlutGLBinder;

class RGlutMaemoInterface: public RGlutInterface
{
public:
    RGlutMaemoInterface();
    ~RGlutMaemoInterface();

    void intialize(int argc, char **argv);
    void terminate();
    void initDisplayMode(unsigned int mode);

    int createWindow();
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
    
    void timerFunc(unsigned int millis, void (*func)(int), int value);
    
    void exec();
    void redraw(int win);
    void flush();
    
    int createMenu(void (*)(int menu));
    void destroyMenu(int menu);
    int getMenu();
    void setMenu(int menu);
    void addMenuEntry(const char* label, int value);
    void removeMenuItem(int item);
    void attachMenu(int button);
    void detachMenu(int button);
    
    int getModifiers();
    int getValue(unsigned int state);

    void draw();
    void reshape(int w, int h);
    void rerect(int x, int y, int w, int h);
    void repos(int x, int y);
 
private:
    class ControlEntry
    {
    public:
        ControlEntry()
         : mId(0),
           mSurface(0),
           mControl(0)
        {}
        
        int mId;
        unsigned int  mSurface;
        RGlutWindow * mControl;
    };

    class TimerEntry
    {
    public:
        TimerEntry()
            : mMilliSec(0),
              mCallBack(0),
              mValue(0),
              mSetTime(0)
        {}

        bool operator<(const TimerEntry & oth) const
        {
            return mMilliSec < oth.mMilliSec;
        }

        unsigned int mMilliSec;
        void(*mCallBack)(int);
	int mValue;
        unsigned int mSetTime;
    };

private:
    RGlutMaemoInterface(RGlutMaemoInterface const &);
    RGlutMaemoInterface & operator=(RGlutMaemoInterface const &);

private:
    int addControl(ControlEntry entry);
    ControlEntry removeControl(int id);
    void removeAllControl();
    ControlEntry * getControlEntry(int id);
    void checkTimers();
    int getModifiers(int state);
    void parseArguments(int argc, char ** argv);
    void createBinder();

private:
    typedef std::vector<ControlEntry> ControlList;
    typedef ControlList::iterator ControlListIter;
    typedef ControlList::const_iterator ControlListConstIter;

    typedef std::stack<ControlEntry> ControlStack;

    typedef std::multiset<TimerEntry> TimerEntrySet;
    typedef TimerEntrySet::iterator TimerEntrySetIter;
    typedef TimerEntrySet::const_iterator TimerEntrySetConstIter;

private:
    static const int ID_START_INDEX = 100;

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
    REGLGlutGLBinder * mBinder;
    unsigned int mDisplayMode;
    int mScreenNumber;
    int mScreenWidth;
    int mScreenHeight;
    
    int mCurrentControl;
    bool mFullScreen;
    int mModifier;

    ControlList mControllist;
    ControlStack mControlStack;
    bool mFinished;

    TimerEntrySet mTimers;
    std::vector<RGlutMenu *> mMenus;
    int mCurrentMenu;
    int mAttachedMenuButton;
};

#endif
