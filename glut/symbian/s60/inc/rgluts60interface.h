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
#include "rglutinterface.h"
#include "rgluteventhandler.h"

#include <e32cmn.h>
#include <e32std.h>
#include <eikmobs.h>
#include <aknstyluspopupmenu.h>

class CEikonEnv;

class RGlutControl;
class RGlutGLBinder;

NONSHARABLE_CLASS(RGlutS60Interface): public RGlutInterface, public RGlutEventHandler
{
private:
    static const int ID_START_INDEX = 100;
    static const int MENU_START_INDEX = 10;
    static const int MENU_ITEM_BIAS = 0x6000;

public:
    RGlutS60Interface();
    ~RGlutS60Interface();
    
    void intialize(int argc, char **argv);
    void terminate();
    void initDisplayMode(unsigned int mode);

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
    
    virtual void draw();
    virtual void reshape(int w, int h);
    virtual void rerect(int x, int y, int w, int h);
    virtual void repos(int x, int y);
    virtual void keyboard(unsigned char key, unsigned int modifier, int x, int y);
    virtual void mouse(int button, int modifier, int x, int y, bool pressed = true);
    virtual void motion(int, int, int, int) {}
    
    virtual int createMenu(void (*)(int menu));
    virtual void destroyMenu(int menu);
    virtual int getMenu();
    virtual void setMenu(int menu);
    virtual void addMenuEntry(const char* label, int value);
    virtual void removeMenuItem(int item);
    virtual void attachMenu(int button);
    virtual void detachMenu(int button);
    
    int getModifiers();
    virtual int getValue(unsigned int state);
    
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
        RGlutControl * mControl;
    };
    
    class MenuEntry: public MEikMenuObserver
    {
    public:
        MenuEntry()
         : mId(0),
           mCallback(0),
           mPopupMenu(0),
           mMenuItems(6)
        {}

        ~MenuEntry()
        {
            mMenuItems.Close();
            delete mPopupMenu;
        }
    
        void ProcessCommandL(TInt id)
        {
            if (id != KErrCancel) {
                mCallback(id - MENU_ITEM_BIAS);
            }
        }
        
        void SetEmphasis(CCoeControl *, TBool)
        {}
        
        int mId;
        void (*mCallback)(int);
        CAknStylusPopUpMenu * mPopupMenu;
        RArray<int> mMenuItems;
    };

    class TimerEntry
    {
    public:
        TimerEntry()
         : mCallback(0),
           mTimer(0),
           mValue(0)
        {}
        
        ~TimerEntry()
        {
            delete mTimer;
        }
        
        void (*mCallback)(int);
        CTimer * mTimer;
        int mValue;
    };
    
    static TInt timerCallbackFunction(TAny * a);

    int addControl(ControlEntry entry);
    ControlEntry removeControl(int id);
    void removeAllControl();
    ControlEntry * getControlEntry(int id);

    MenuEntry * getMenuEntry(int id);
    void removeMenuEntries();
    
    void recreateSurface(int win);

private:
    unsigned int mDisplayMode;
    CEikonEnv* mEikonEnv;
    int mCurrentControl;
    RGlutGLBinder * mBinder;
    RArray<ControlEntry> mControllist;
    RArray<ControlEntry> mControlStack;
    
    bool mFullScreen;
    int mModifier;

    RPointerArray<MenuEntry> mMenuList;
    int mCurrentMenu;
    int mAttachedMenuButton;
    
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
