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
 
#include "gluts60interface.h"
#include "glutApplication.h"
#include "eglglutglbinder.h"
#include "glutcontrol.h"
#include "glutappui.h"

#include <pointerevent.h>
#include <apacmdln.h>
#include <eikenv.h>
#include <e32std.h>
#include <string.h>

#define  ID_START_INDEX 100 

GlutInterface* GlutInterface::getInterface()
{
#if !defined(GLUT_STATIC)
    GlutInterface * interface = (GlutInterface *)Dll::Tls();
    if (!interface) {
        interface = new GlutS60Interface();
        TInt error = Dll::SetTls((TAny *)interface);
        if (error != KErrNone) {
            delete interface;
            interface = 0;
        }
    }

    return interface;
#else
    static GlutInterface * interface = new  GlutS60Interface();
    return interface;
#endif
}

bool GlutInterface::destroyInterface()
{
    bool status = false;
#if !defined(GLUT_STATIC)
    GlutInterface * interface = (GlutInterface *)Dll::Tls();
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
    return new GlutApplication();
}

GlutS60Interface::GlutS60Interface()
  : mEikonEnv(0),
    mCurrentControl(0),
    mBinder(0),
    mTimer(0),
    mFullScreen(false),
    mMouseModifier(0),
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

GlutS60Interface::~GlutS60Interface()
{
    terminate();
    removeAllControl();
    mControllist.Close();

}

void GlutS60Interface::intialize(int argc, char ** argv)
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
        TInt error = CApaCommandLine::GetCommandLineFromProcessEnvironment(commandLine);
        if (error == KErrNone) {
            TFileName executableFileName = RProcess().FileName();
            error = User::RenameThread(TParsePtrC(executableFileName).Name());
            if (error == KErrNone) {
                mEikonEnv = new CEikonEnv;
                if (mEikonEnv) {
                    TRAP(error, mEikonEnv->ConstructAppFromCommandLineL(factory,
                                                                        *commandLine));
                }
            }
        }
        
        delete commandLine;
    }

    // currently egl is used to bind native window system with opengl/openvg/opengles
    GlutGLBinder::API api;
    switch (getRenderer()) {
        case GL:
            api = GlutGLBinder::OPENGL_API;
            break;
        case GLES2:
            api = GlutGLBinder::OPENGL_ES2_API;
            break;
        case VG:
            api = GlutGLBinder::OPENVG_API;
            break;
        case GLES:
        default:
            api = GlutGLBinder::OPENGL_ES_API;
            break;            
    }
    
    mBinder = new EGLGlutGLBinder(api);
    mBinder->initialize();
}

void GlutS60Interface::terminate()
{
    if (mBinder) {
        mBinder->terminate();
        delete mBinder;
        mBinder = 0;
    }

    removeAllControl();

    if (mEikonEnv != 0) {
        mEikonEnv->DisableShutdownChecks();
        mEikonEnv->DestroyEnvironment();
    }

    mEikonEnv = 0;
    delete mTimer;
    mTimer = 0;
}

int GlutS60Interface::createWindow()
{
    GlutControl * control = new GlutControl();
    
    GlutAppUi * appUI = (GlutAppUi *)CCoeEnv::Static()-> AppUi();
    if (mFullScreen) {
        appUI->SetFullScreen(true);
    }
    
    if (mWindowProperty.mWidth == 0 || mWindowProperty.mHeight == 0) {
        control->ConstructL(appUI->ClientRect());
    } else {
        control->ConstructL(TRect(TPoint(mWindowProperty.mX, mWindowProperty.mY),
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
    EGLGlutGLBinder::EGLSurfaceInfo surfaceInfo;
    surfaceInfo.mType = EGLGlutGLBinder::EGLSurfaceInfo::TYPE_WINDOW;
    surfaceInfo.mData = (void *)&control->nativeWindow();
    
    ControlEntry entry;
    entry.mSurface = mBinder->createSurface((GlutGLBinder::Surface)&surfaceInfo, 0, 0);
    entry.mControl = control;
    
    int id = addControl(entry);
    setWindow(id);
    
    return id;
}

void GlutS60Interface::destroyWindow(int win)
{
    ControlEntry entry = removeControl(win);
    if (entry.mSurface) {
        mBinder->destroySurface(entry.mSurface);
    }
}

void GlutS60Interface::recreateSurface(int win)
{
    ControlEntry *  entry = getControlEntry(win);
    if (entry && entry->mSurface) {
        mBinder->destroySurface(entry->mSurface);
        
        EGLGlutGLBinder::EGLSurfaceInfo surfaceInfo;
        surfaceInfo.mType = EGLGlutGLBinder::EGLSurfaceInfo::TYPE_WINDOW;
        surfaceInfo.mData = (void *)&entry->mControl->nativeWindow();
        
        entry->mSurface = mBinder->createSurface((GlutGLBinder::Surface)&surfaceInfo, 0, 0);
        mBinder->makeCurrent(entry->mSurface);
    }
}

int GlutS60Interface::getWindow()
{
    return mCurrentControl;
}

void GlutS60Interface::setWindow(int win)
{
    if (win != mCurrentControl) {
        ControlEntry * entry = getControlEntry(win);
        if (entry) {
            mBinder->makeCurrent(entry->mSurface);
            mCurrentControl = entry->id;
        }
    }
}

void GlutS60Interface::setWindowTitle(const char * title)
{
    if (title) {
        ;
    }
}

void GlutS60Interface::showWindow()
{
    ControlEntry * entry = getControlEntry(mCurrentControl);
    if (entry) {
        entry->mControl->MakeVisible(ETrue);
    }
}

void GlutS60Interface::fullScreen()
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

void GlutS60Interface::hideWindow()
{
    ControlEntry * entry = getControlEntry(mCurrentControl);
    if (entry) {
        entry->mControl->MakeVisible(EFalse);
    }
}

void GlutS60Interface::positionWindow(int x, int y)
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

void GlutS60Interface::reshapeWindow(int width, int height)
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

void GlutS60Interface::popWindow()
{
    int count = mControlStack.Count();
    if (count > 0) {
        ControlEntry entry = mControlStack[count - 1];
        setWindow(entry.id);
        mControlStack.Remove(count - 1);
    }
}

void GlutS60Interface::pushWindow()
{
    int cntcount = mControllist.Count();
    if (cntcount > 1) {
        ControlEntry * entry = getControlEntry(mCurrentControl);
        if (entry) {
            mControlStack.Append(*entry);
        }
        setWindow(getControlEntry(mCurrentControl-1)->id);
    }
}

int GlutS60Interface::addControl(ControlEntry entry)
{
    entry.id = ID_START_INDEX + mControllist.Count();
    mControllist.Append(entry);   
    
    return entry.id;
}

GlutS60Interface::ControlEntry * GlutS60Interface::getControlEntry(int id)
{
    ControlEntry * entry = 0;
    int count = mControllist.Count();
    
    for (int i=0; i<count; ++i) {
        if (mControllist[i].id == id) {
            entry = &mControllist[i];
            break;
        }
    }
    
    return entry;
}

GlutS60Interface::ControlEntry GlutS60Interface::removeControl(int id)
{
    ControlEntry entry;
    
    if (getControlEntry(id)) {
        if (id == mCurrentControl) {
            mCurrentControl = 0;
        }
        
        int index = id - ID_START_INDEX;
        if (index < mControllist.Count()) {
            entry = mControllist[index];
            GlutControl * ctrl = entry.mControl;
            ctrl->ControlEnv()->AppUi()->RemoveFromStack(ctrl);
            delete ctrl;
            mControllist.Remove(index);
        }
    }
    
    return entry;
}

void GlutS60Interface::removeAllControl()
{
    int count = mControllist.Count();

    for (int i=0; i<count; ++i) {
        GlutControl * ctrl = mControllist[i].mControl;
        ctrl->ControlEnv()->AppUi()->RemoveFromStack(ctrl);
        delete ctrl;
    }
    mControllist.Reset();
}

void GlutS60Interface::exec()
{
    ControlEntry * entry = getControlEntry(mCurrentControl);
    if (entry && entry->mControl) {
        reshape(entry->mControl->Rect().Width(), entry->mControl->Rect().Height());

        if (mEikonEnv) {
            mEikonEnv->Execute();
        }
    }
    terminate();
}

void GlutS60Interface::redraw(int win)
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

void GlutS60Interface::flush()
{
    if (mBinder) {
        ControlEntry * entry = getControlEntry(mCurrentControl);
        if (entry) {
            mBinder->swapBuffer(entry->mSurface);
        }
    }
}

void GlutS60Interface::draw()
{
    if (mCallbacks.draw) {
        mCallbacks.draw();
    }
}

void GlutS60Interface::rerect(int x, int y, int w, int h)
{
    ControlEntry * entry = getControlEntry(mCurrentControl);
    if (entry && entry->mControl) {
        TRect rect(TPoint(x, y), TSize(w, h));
        if (entry->mControl->Rect() != rect) {
            if (mFullScreen) {
                entry->mControl->SetExtentToWholeScreen();
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

void GlutS60Interface::reshape(int w, int h)
{
    reshapeWindow(w, h);
    
    if (mCallbacks.reshape) {
        mCallbacks.reshape(w, h);
    }
}

void GlutS60Interface::repos(int x, int y)
{
    positionWindow(x, y);

    if (mCallbacks.repos) {
        mCallbacks.repos(x, y);
    }
}

void GlutS60Interface::keyboard(unsigned char key, int x, int y)
{
    if (mCallbacks.keyboard) {
        mCallbacks.keyboard(key, x, y);
    }
}

void GlutS60Interface::mouse(int button, int modifier, int x, int y)
{
    mMouseModifier = modifier;
    
    if (mCallbacks.mouse) {
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
            mCallbacks.mouse(glutButton, glutState, x, y);
        }
    }
    
    if (mCallbacks.motion) {
        switch (button) {
            case TPointerEvent::EDrag: {
                mCallbacks.motion(x, y);
                break;
            }
        }
    }

    if (mCallbacks.passiveMotion) {
        switch (button) {
            case TPointerEvent::EMove: {
                mCallbacks.passiveMotion(x, y);
                break;
            }
        }
    }
}

int GlutS60Interface::getModifiers()
{
    int modifier = 0;
    
    switch (mMouseModifier) {
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

struct CallbackArg
{
    void (*func)(int);
    int value;
};

TInt callbackFunction(TAny * a)
{
    CallbackArg * arg = (CallbackArg*)a;
    arg->func(arg->value);
    delete arg;
    return 0;
}

void GlutS60Interface::timerFunc(unsigned int millis, void (*func)(int), int value)
{
    delete mTimer;
    mTimer = 0;
    mTimer = CPeriodic::New(CActive::EPriorityStandard);
    
    CallbackArg * arg = new CallbackArg;
    arg->func = func;
    arg->value = value;
    TCallBack callback(callbackFunction, arg);
    
    mTimer->Start(millis * 1000, 0, callback);
}
