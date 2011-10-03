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
 
#include "rgluts60interface.h"
#include "rglutApplication.h"
#include "reglglutglbinder.h"
#include "rglutcontrol.h"
#include "rglutappui.h"

#include <pointerevent.h>
#include <apacmdln.h>
#include <eikenv.h>
#include <e32std.h>
#include <string.h>

RGlutInterface* RGlutInterface::getInterface()
{
#if !defined(GLUT_STATIC)
    RGlutInterface * interface = (RGlutInterface *)Dll::Tls();
    if (!interface) {
        interface = new RGlutS60Interface();
        TInt error = Dll::SetTls((TAny *)interface);
        if (error != KErrNone) {
            delete interface;
            interface = 0;
        }
    }

    return interface;
#else
    static RGlutInterface * interface = new  RGlutS60Interface();
    return interface;
#endif
}

bool RGlutInterface::destroyInterface()
{
    bool status = false;
#if !defined(GLUT_STATIC)
    RGlutInterface * interface = (RGlutInterface *)Dll::Tls();
    if (interface) {
        if (Dll::SetTls(0) == KErrNone) {
            delete interface;
            status = true;
        }
    }
#else
    status = true;
#endif
    return status;
}

CApaApplication *newApplication()
{
    return new RGlutApplication();
}

RGlutS60Interface::RGlutS60Interface()
  : mDisplayMode(0),
    mEikonEnv(0),
    mCurrentControl(0),
    mBinder(0),
    mFullScreen(false),
    mModifier(0),
    mCurrentMenu(0),
    mAttachedMenuButton(-1),
    KParamRenderer((TUint8 *)"-renderer", 9, 9),
    KParamRendererVG((TUint8 *)"vg", 2, 2),
    KParamRendererGL((TUint8 *)"gl", 2, 2),
    KParamRendererGLES((TUint8 *)"gles", 4, 4),
    KParamRendererGLES2((TUint8 *)"gles2", 5, 5),
    KParamOrientation((TUint8 *)"-orientation", 12, 12),
    KParamOrientationPortrait((TUint8 *)"portrait", 8, 8),
    KParamOrientationLandscape((TUint8 *)"landscape", 9, 9),
    KParamOrientationAutomatic((TUint8 *)"automatic", 9, 9)
{
    setRenderer(GLES);
    setOrientation(Automatic);
}

RGlutS60Interface::~RGlutS60Interface()
{
    terminate();
    mControllist.Close();
}

void RGlutS60Interface::intialize(int argc, char ** argv)
{    
    for (int i=0; i < argc; ++i) {
        
        const int nameLen = strlen(argv[i]);
        TPtr8 name((TUint8 *)argv[i], nameLen, nameLen);

        if (name == KParamRenderer) {
            
            if (i+1 >= argc) {
                break;
            }
            const int valueLen = strlen(argv[i+1]);
            TPtr8 value((TUint8 *)argv[i+1], valueLen, valueLen);
            if (value == KParamRendererVG) {
                setRenderer(VG);
            } else if (value == KParamRendererGLES2) {
                setRenderer(GLES2);
            } else if (value == KParamRendererGLES) {
                setRenderer(GLES);
            } else if (value == KParamRendererGL) {
                setRenderer(GL);
            }
            ++i;
        }
        
        if (name == KParamOrientation) {
            
            if (i+1 >= argc) {
                break;
            }
            
            const int valueLen = strlen(argv[i+1]);
            TPtr8 value((TUint8 *)argv[i+1], valueLen, valueLen);
            
            if (value == KParamOrientationPortrait) {
                setOrientation(Portrait);
            } else if (value == KParamOrientationLandscape) {
                setOrientation(Landscape);
            } else if (value == KParamOrientationAutomatic) {
                setOrientation(Automatic);
            }
            ++i;
        }
    }

    if (!CCoeEnv::Static()) {
        TApaApplicationFactory factory(newApplication);
        CApaCommandLine* commandLine = 0;
        TInt error = 
           CApaCommandLine::GetCommandLineFromProcessEnvironment(commandLine);
        if (error == KErrNone) {
            TFileName executableFileName = RProcess().FileName();
            error = User::RenameThread(TParsePtrC(executableFileName).Name());
            if (error == KErrNone) {
                mEikonEnv = new CEikonEnv;
                if (mEikonEnv) {
                    TRAP(error,
                        mEikonEnv->ConstructAppFromCommandLineL(factory,
                                                               *commandLine));
                }
            }
        }
        
        delete commandLine;
    }

    /*
     * currently egl is used to bind native window system
     * with opengl/openvg/opengles 
     */ 
    RGlutGLBinder::API api;
    switch (getRenderer()) {
        case GL:
            api = RGlutGLBinder::OPENGL_API;
            break;
        case GLES2:
            api = RGlutGLBinder::OPENGL_ES2_API;
            break;
        case VG:
            api = RGlutGLBinder::OPENVG_API;
            break;
        case GLES:
        default:
            api = RGlutGLBinder::OPENGL_ES_API;
            break;            
    }
    
    mBinder = new REGLGlutGLBinder(api);
    mBinder->initialize();
    mTimerMutex.CreateLocal();
}

void RGlutS60Interface::terminate()
{
    mExpiredTimer.ResetAndDestroy();
    
    if (mBinder) {
        mBinder->terminate();
        delete mBinder;
        mBinder = 0;
    }

    removeMenuEntries();
    removeAllControl();

    if (mEikonEnv != 0) {
        mEikonEnv->DisableShutdownChecks();
        mEikonEnv->DestroyEnvironment();
    }

    mEikonEnv = 0;
    mTimerMutex.Close();
}

void RGlutS60Interface::initDisplayMode(unsigned int mode)
{
    mDisplayMode = mode;

    int rgb = mDisplayMode & 1; 
    if (rgb == 0) {
        // GLUT_RGBA or GLUT_RGB
        mBinder->setRedSize(8);
        mBinder->setGreenSize(8);
        mBinder->setBlueSize(8);
        mBinder->setAlphaSize(8);
    }
        
    if (mDisplayMode & GLUT_DEPTH) {
        mBinder->setDepth(true);
    }
    
    if (mDisplayMode & GLUT_STENCIL) {
        mBinder->setStencil(true);
    }
}

int RGlutS60Interface::createWindow()
{
    RGlutControl * control = new RGlutControl();
    
    RGlutAppUi * appUI = (RGlutAppUi *)CCoeEnv::Static()-> AppUi();
    if (mFullScreen) {
        appUI->SetFullScreen(true);
    }
    
    if (mWindowProperty.mWidth == 0 || mWindowProperty.mHeight == 0) {
        control->ConstructL(appUI->ClientRect());
    } else {
        control->ConstructL(
                    TRect(TPoint(mWindowProperty.mX, mWindowProperty.mY),
                    TSize(mWindowProperty.mWidth, mWindowProperty.mHeight)));
    }

    control->SetMopParent(appUI);
    if (mFullScreen) {
        control->SetExtentToWholeScreen();
    }

    control->ControlEnv()->AppUi()->AddToStackL(control);
    
    control->setEventHandler(this);
    appUI->setEventHandler(this);
    
    // binding to egl, only window surface is supported
    REGLGlutGLBinder::EGLSurfaceInfo surfaceInfo;
    surfaceInfo.mType = REGLGlutGLBinder::EGLSurfaceInfo::TYPE_WINDOW;
    surfaceInfo.mData = (void *)&control->nativeWindow();
    
    ControlEntry entry;
    entry.mSurface = mBinder->createSurface(
                                (RGlutGLBinder::Surface)&surfaceInfo, 0, 0);
   
    if (entry.mSurface == 0) {
        appUI->RemoveFromStack(control);
        delete control;
        return 0;
    }
    
    entry.mControl = control;
    
    
    int id = addControl(entry);
    setWindow(id);
    
    return id;
}

void RGlutS60Interface::destroyWindow(int win)
{
    ControlEntry entry = removeControl(win);
    if (entry.mSurface) {
        mBinder->destroySurface(entry.mSurface);
    }
}

void RGlutS60Interface::recreateSurface(int win)
{
    ControlEntry *  entry = getControlEntry(win);
    if (entry && entry->mSurface) {
        mBinder->destroySurface(entry->mSurface);
        
        REGLGlutGLBinder::EGLSurfaceInfo surfaceInfo;
        surfaceInfo.mType = REGLGlutGLBinder::EGLSurfaceInfo::TYPE_WINDOW;
        surfaceInfo.mData = (void *)&entry->mControl->nativeWindow();
        
        entry->mSurface = mBinder->createSurface(
                                (RGlutGLBinder::Surface)&surfaceInfo, 0, 0);
        mBinder->makeCurrent(entry->mSurface);
    }
}

int RGlutS60Interface::getWindow()
{
    return mCurrentControl;
}

void RGlutS60Interface::setWindow(int win)
{
    if (win != mCurrentControl) {
        ControlEntry * entry = getControlEntry(win);
        if (entry) {
            mBinder->makeCurrent(entry->mSurface);
            mCurrentControl = entry->mId;
        }
    }
}

void RGlutS60Interface::setWindowTitle(const char * title)
{
    if (title) {
        ;
    }
}

void RGlutS60Interface::showWindow()
{
    ControlEntry * entry = getControlEntry(mCurrentControl);
    if (entry) {
        entry->mControl->MakeVisible(ETrue);
    }
}

void RGlutS60Interface::fullScreen()
{
    mFullScreen = true;
	
    ControlEntry * entry = getControlEntry(mCurrentControl);
    if (entry) {
        entry->mControl->SetExtentToWholeScreen();
        entry->mControl->MakeVisible(ETrue);
        // in some device EGLSurface need to recreate
        // once size is changed
        recreateSurface(mCurrentControl);
    }
}

void RGlutS60Interface::hideWindow()
{
    ControlEntry * entry = getControlEntry(mCurrentControl);
    if (entry) {
        entry->mControl->MakeVisible(EFalse);
    }
}

void RGlutS60Interface::positionWindow(int x, int y)
{
    ControlEntry * entry = getControlEntry(mCurrentControl);
    if (entry && entry->mControl) {
        TPoint point(x, y);
        if (entry->mControl->Position() != point) {
            if (mFullScreen) {
                entry->mControl->SetExtentToWholeScreen();
            } else {
                entry->mControl->SetPosition(point);
                // in some device EGLSurface need to recreate
                // once size is changed 
                recreateSurface(mCurrentControl);
            }
        }
    }
}

void RGlutS60Interface::reshapeWindow(int width, int height)
{
    ControlEntry * entry = getControlEntry(mCurrentControl);
    if (entry && entry->mControl) {
        TSize size(width, height);
        if (entry->mControl->Size() != size) {
            if (mFullScreen) {
                entry->mControl->SetExtentToWholeScreen();
            } else {
                entry->mControl->SetSize(size);
                // in some device EGLSurface need to recreate
                // once size is changed
                recreateSurface(mCurrentControl);
            }
        }
    }
}

void RGlutS60Interface::popWindow()
{
    int count = mControlStack.Count();
    if (count > 0) {
        ControlEntry entry = mControlStack[count - 1];
        setWindow(entry.mId);
        mControlStack.Remove(count - 1);
    }
}

void RGlutS60Interface::pushWindow()
{
    int cntcount = mControllist.Count();
    if (cntcount > 1) {
        ControlEntry * entry = getControlEntry(mCurrentControl);
        if (entry) {
            mControlStack.Append(*entry);
        }
        setWindow(getControlEntry(mCurrentControl-1)->mId);
    }
}

int RGlutS60Interface::addControl(ControlEntry entry)
{
    entry.mId = ID_START_INDEX + mControllist.Count();
    mControllist.Append(entry);   
    
    return entry.mId;
}

RGlutS60Interface::ControlEntry * RGlutS60Interface::getControlEntry(int id)
{
    ControlEntry * entry = 0;
    int count = mControllist.Count();
    
    for (int i=0; i<count; ++i) {
        if (mControllist[i].mId == id) {
            entry = &mControllist[i];
            break;
        }
    }
    
    return entry;
}

RGlutS60Interface::ControlEntry RGlutS60Interface::removeControl(int id)
{
    ControlEntry entry;
    
    if (getControlEntry(id)) {
        if (id == mCurrentControl) {
            mCurrentControl = 0;
        }
        
        int index = id - ID_START_INDEX;
        if (index < mControllist.Count()) {
            entry = mControllist[index];
            RGlutControl * ctrl = entry.mControl;
            ctrl->ControlEnv()->AppUi()->RemoveFromStack(ctrl);
            delete ctrl;
            mControllist.Remove(index);
        }
    }
    
    return entry;
}

void RGlutS60Interface::removeAllControl()
{
    int count = mControllist.Count();

    for (int i=0; i<count; ++i) {
        RGlutControl * ctrl = mControllist[i].mControl;
        ctrl->ControlEnv()->AppUi()->RemoveFromStack(ctrl);
        delete ctrl;
    }
    mControllist.Reset();
}

void RGlutS60Interface::exec()
{
    ControlEntry * entry = getControlEntry(mCurrentControl);
    if (entry && entry->mControl) {
        reshape(entry->mControl->Rect().Width(),
                entry->mControl->Rect().Height());

        if (mEikonEnv) {
            mEikonEnv->Execute();
        }
    }
    terminate();
}

void RGlutS60Interface::redraw(int win)
{
    if (win == 0) {
        ControlEntry * entry = getControlEntry(mCurrentControl);
        if (entry && entry->mControl) {
            entry->mControl->DrawNow();
        }
    } else {
        ControlEntry * entry = getControlEntry(win);
        if (entry) {
            mBinder->swapBuffer(entry->mSurface);
        }
    }
}

void RGlutS60Interface::flush()
{
    if (mBinder) {
        ControlEntry * entry = getControlEntry(mCurrentControl);
        if (entry) {
            mBinder->swapBuffer(entry->mSurface);
        }
    }
}

void RGlutS60Interface::draw()
{
    if (mCallbacks.draw) {
        mCallbacks.draw();
    }
}

void RGlutS60Interface::rerect(int x, int y, int w, int h)
{
    ControlEntry * entry = getControlEntry(mCurrentControl);
    if (entry && entry->mControl) {
        TRect rect(TPoint(x, y), TSize(w, h));
        if (entry->mControl->Rect() != rect) {
            if (mFullScreen) {
                entry->mControl->SetExtentToWholeScreen();
                w = entry->mControl->Size().iWidth;
                h = entry->mControl->Size().iHeight;
                x = entry->mControl->Position().iX;
                y = entry->mControl->Position().iY;
            } else {
                entry->mControl->SetRect(rect);
                // in some device EGLSurface need to recreate
                // once size is changed 
                recreateSurface(mCurrentControl);
            }
            if (mCallbacks.reshape) {
                mCallbacks.reshape(w, h);
            }
            if (mCallbacks.repos) {
                mCallbacks.repos(x, y);
            }
        }
    }
}

void RGlutS60Interface::reshape(int w, int h)
{
    reshapeWindow(w, h);
    
    if (mCallbacks.reshape) {
        mCallbacks.reshape(w, h);
    }
}

void RGlutS60Interface::repos(int x, int y)
{
    positionWindow(x, y);

    if (mCallbacks.repos) {
        mCallbacks.repos(x, y);
    }
}

void RGlutS60Interface::keyboard(unsigned char key, unsigned int modifier, int x, int y)
{
    if (mCallbacks.keyboard) {
        mModifier = modifier;
        mCallbacks.keyboard(key, x, y);
    }
}

void RGlutS60Interface::mouse(int button, int modifier, int x, int y, bool)
{
    mModifier = modifier;
    
    if (mCallbacks.mouse || mAttachedMenuButton != -1) {
        int glutButton = -1;
        int glutState  = -1;
        
        switch (button) {
            case TPointerEvent::EButton1Down: {
                glutButton = GLUT_LEFT_BUTTON;
                glutState = GLUT_DOWN;
                break;
            }
            case TPointerEvent::EButton1Up: {
                glutButton = GLUT_LEFT_BUTTON;
                glutState = GLUT_UP;
                break;
            }
            case TPointerEvent::EButton2Down: {
                glutButton = GLUT_MIDDLE_BUTTON;
                glutState = GLUT_DOWN;
                break;
            }
            case TPointerEvent::EButton2Up: {
                glutButton = GLUT_MIDDLE_BUTTON;
                glutState = GLUT_UP;
                break;
            }
            case TPointerEvent::EButton3Down: {
                glutButton = GLUT_RIGHT_BUTTON;
                glutState = GLUT_DOWN;
                break;
            }
            case TPointerEvent::EButton3Up: {
                glutButton = GLUT_RIGHT_BUTTON;
                glutState = GLUT_UP;
                break;
            }
        }
        
        if (glutButton != -1) {
            if (mAttachedMenuButton == glutButton && glutState == GLUT_DOWN) {
                MenuEntry * entry = getMenuEntry(mCurrentMenu);
                if (entry) {
                    entry->mPopupMenu->SetPosition(TPoint(x, y));
                    entry->mPopupMenu->ShowMenu();
                    return;
                }
            } else if (mCallbacks.mouse) {
                mCallbacks.mouse(glutButton, glutState, x, y);
            }
        }
    }
    
    switch (button) {
        case TPointerEvent::EDrag: {
            if (mCallbacks.motion) {
                mCallbacks.motion(x, y);
            }
            break;
        }
        case TPointerEvent::EMove: {
            if (mCallbacks.passiveMotion) {
                mCallbacks.passiveMotion(x, y);
            }
            break;
        }
    }
}

int RGlutS60Interface::getModifiers()
{
    int modifier = 0;
    
    switch (mModifier) {
        case EModifierAlt:
        case EModifierLeftAlt:
        case EModifierRightAlt: {            
            modifier = GLUT_ACTIVE_ALT;
            break;
        }
    
        case EModifierCtrl:
        case EModifierLeftCtrl:
        case EModifierRightCtrl: {
            modifier = GLUT_ACTIVE_CTRL;
            break;
        }
    
        case EModifierShift:
        case EModifierLeftShift: 
        case EModifierRightShift: {
            modifier = GLUT_ACTIVE_SHIFT;
            break;
        }
    }
    
    return modifier;
}

TInt RGlutS60Interface::timerCallbackFunction(TAny * a)
{
    TimerEntry * arg = (TimerEntry*)a;
    arg->mCallback(arg->mValue);
    arg->mGlutInterface->addExpiredTimer(arg);
    return 0;
}

void RGlutS60Interface::addExpiredTimer(TimerEntry * entry)
{
    mTimerMutex.Wait();
    mExpiredTimer.ResetAndDestroy();
    mExpiredTimer.Append(entry);
    mTimerMutex.Signal();
}

void RGlutS60Interface::timerFunc(unsigned int millis,
                                  void (*func)(int), int value)
{
    CPeriodic * timer = CPeriodic::New(CActive::EPriorityStandard);
    if (timer) {
        TimerEntry * arg = new TimerEntry;
        if (arg) {
            arg->mCallback = func;
            arg->mValue = value;
            arg->mTimer = timer;
            arg->mGlutInterface = this;
            TCallBack callback(timerCallbackFunction, arg);
            
            timer->Start(millis * 1000, 0, callback);
        }
    }
}

RGlutS60Interface::MenuEntry * RGlutS60Interface::getMenuEntry(int id)
{
    MenuEntry * entry = 0;
    
    const int count = mMenuList.Count();
    
    for (int i=0; i<count; ++i) {
        if (mMenuList[i]->mId == id) {
            entry = mMenuList[i];
            break;
        }
    }
    
    return entry;
}

void RGlutS60Interface::removeMenuEntries()
{
    mCurrentMenu = 0;
    mAttachedMenuButton = -1;
    mMenuList.ResetAndDestroy();    
}

int RGlutS60Interface::createMenu(void (*callback)(int))
{
    int id = 0;

    if (!callback) {
        return 0;
    }
    
    MenuEntry * entry = new MenuEntry;
    
    TRAP(id, entry->mPopupMenu = CAknStylusPopUpMenu::NewL( 
                                    entry, TPoint(0, 0), 0); );
    if (id != KErrNone) {
        delete entry;
        return 0;
    }

    if (mMenuList.Append(entry) != KErrNone) {
        delete entry;
        return 0;
    }

    entry->mId = MENU_START_INDEX + mMenuList.Count();
    entry->mCallback = callback;
    
    mCurrentMenu = entry->mId;
    
    return mCurrentMenu;
}

void RGlutS60Interface::destroyMenu(int menu)
{
    if (menu == mCurrentMenu) {
        mCurrentMenu = 0;
        mAttachedMenuButton = -1;
    }
    
    const int count = mMenuList.Count();
    for (int i=0; i<count; ++i) {
        if (mMenuList[i]->mId == menu) {
            delete mMenuList[i];
            mMenuList.Remove(i);
            break;
        }
    }
}

int RGlutS60Interface::getMenu()
{
    return mCurrentMenu;
}

void RGlutS60Interface::setMenu(int menu)
{
    MenuEntry * entry = getMenuEntry(menu);
    if (entry) {
        mCurrentMenu = menu;        
    }
}

void RGlutS60Interface::addMenuEntry(const char * label, int value)
{
    const int BiasedId = value + MENU_ITEM_BIAS;
    
    MenuEntry * entry = getMenuEntry(mCurrentMenu);
    if (entry) {
        
        const int count = mMenuList.Count();
        for (int i=0; i<count; ++i) {
            if (mMenuList[i]->mId == BiasedId) {
                return;
            }
        }

        const int llen = strlen(label);
        if (llen < 255) {
            TBuf<255> item;
            item.Copy(TPtrC8((const TUint8 *)label, llen));

            TRAPD(err, entry->mPopupMenu->AddMenuItemL(item, BiasedId); );
            if (err == KErrNone) {
                entry->mMenuItems.Append(BiasedId);
            }
        }
    }
}

void RGlutS60Interface::removeMenuItem(int item)
{
    item = item - 1;

    MenuEntry * entry = getMenuEntry(mCurrentMenu);
    if (entry) {
        if (item >= 0 && item < entry->mMenuItems.Count()) {
            entry->mPopupMenu->RemoveMenuItem(entry->mMenuItems[item]);
            entry->mMenuItems.Remove(item);
        }
    }
}

void RGlutS60Interface::attachMenu(int button)
{
    if (mCurrentMenu) {
        mAttachedMenuButton = button;
    }
}

void RGlutS60Interface::detachMenu(int button)
{
    if (mCurrentMenu && mAttachedMenuButton == button) {
        mAttachedMenuButton = -1;
    }
}

int RGlutS60Interface::getValue(unsigned int state)
{
    int value = 0;
    
    switch (state) {
        case GLUT_WINDOW_X: {
            ControlEntry * entry = getControlEntry(mCurrentControl);
            if (entry && entry->mControl) {
                value = entry->mControl->Position().iX;
            }
            break;
        }
        
        case GLUT_WINDOW_Y: {
            ControlEntry * entry = getControlEntry(mCurrentControl);
            if (entry && entry->mControl) {
                value = entry->mControl->Position().iY;
            }        
            break;
        }
        
        case GLUT_WINDOW_WIDTH: {
            ControlEntry * entry = getControlEntry(mCurrentControl);
            if (entry && entry->mControl) {
                value = entry->mControl->Size().iWidth;
            }
            break;
        }

        case GLUT_WINDOW_HEIGHT: {
            ControlEntry * entry = getControlEntry(mCurrentControl);
            if (entry && entry->mControl) {
                value = entry->mControl->Size().iHeight;
            }
            break;
        }

        case GLUT_WINDOW_BUFFER_SIZE: {
            value = mBinder->getBufferSize();
            break;
        }
            
        case GLUT_WINDOW_STENCIL_SIZE: {
            value = mBinder->getStencilSize();
            break;
        }
        
        case GLUT_WINDOW_DEPTH_SIZE: {
            value = mBinder->getDepthSize();
            break;
        }
        
        case GLUT_WINDOW_RED_SIZE: {
            value = mBinder->getRedSize();
            break;
        }
        
        case GLUT_WINDOW_GREEN_SIZE: {
            value = mBinder->getGreenSize();
            break;
        }
        
        case GLUT_WINDOW_BLUE_SIZE: {
            value = mBinder->getBlueSize();
            break;
        }
        
        case GLUT_WINDOW_ALPHA_SIZE: {
            value = mBinder->getAlphaSize();
            break;
        }
        
        case GLUT_WINDOW_DOUBLEBUFFER: {
            value = 1;
            break;
        }
                
        case GLUT_SCREEN_WIDTH: {
            if (mEikonEnv) {
                value = mEikonEnv->ScreenDevice()->SizeInPixels().iWidth;
            }
            break;
        }
        
        case GLUT_SCREEN_HEIGHT: {
            if (mEikonEnv) {
                value = mEikonEnv->ScreenDevice()->SizeInPixels().iHeight;
            }
            break;
        }
        
        case GLUT_SCREEN_WIDTH_MM: {
            if (mEikonEnv) {
                value = mEikonEnv->ScreenDevice()->SizeInTwips().iWidth;
                value = static_cast<int>(10.0f/567.0f * (float)value);
            }
            break;
        }
        
        case GLUT_SCREEN_HEIGHT_MM: {
            if (mEikonEnv) {
                value = mEikonEnv->ScreenDevice()->SizeInTwips().iHeight;
                value = static_cast<int>(10.0f/567.0f * (float)value);
            }
            break;
        }

        case GLUT_MENU_NUM_ITEMS: {
            MenuEntry * entry = getMenuEntry(mCurrentMenu);
            if (entry) {
                value = entry->mMenuItems.Count();
            }
            break;
        }

    }
    
    return value;
}
