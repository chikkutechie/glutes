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

#include "rglutmaemointerface.h"
#include "reglglutglbinder.h"

#include <time.h>
#include <sys/time.h>

static RGlutInterface * interface = 0;

RGlutInterface* RGlutInterface::getInterface()
{
    if (!interface) {
        interface = new  RGlutMaemoInterface();
    }
    return interface;
}

bool RGlutInterface::destroyInterface()
{
    delete interface;
    interface = 0;
    return true;
}

RGlutMaemoInterface::RGlutMaemoInterface()
  : KParamRenderer("-renderer"),
    KParamRendererVG("vg"),
    KParamRendererGL("gl"),
    KParamRendererGLES("gles"),
    KParamRendererGLES2("gles2"),
    KParamOrientation("-orientation"),
    KParamOrientationPortrait("portrait"),
    KParamOrientationLandscape("landscape"),
    KParamOrientationAutomatic("automatic"),
    mBinder(0),
    mDisplayMode(0),
    mDisplay(0),
    mScreenNumber(0),
    mScreenWidth(0),
    mScreenHeight(0),
    mRootWindow(0),
    mCurrentControl(0),
    mFullScreen(false),
    mModifier(0),
    mFinished(false)
{
    setRenderer(GLES);
}

RGlutMaemoInterface::~RGlutMaemoInterface()
{
    terminate();
}

void RGlutMaemoInterface::parseArguments(int argc, char ** argv)
{
    for (int i=0; i < argc; ++i) {
        
        std::string name(argv[i]);

        if (name == KParamRenderer) {
            
            if (i+1 >= argc) {
                break;
            }
            
            std::string value(argv[i+1]);
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
            
            std::string value(argv[i+1]);
            
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
}

void RGlutMaemoInterface::createBinder()
{
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
    
    mBinder->setNativeDisplay((int)mDisplay);
    mBinder->initialize();
}

void RGlutMaemoInterface::intialize(int argc, char ** argv)
{    
    parseArguments(argc, argv);

    mDisplay = XOpenDisplay(0);
    if (mDisplay == 0) {
        return;
    }

    mScreenNumber = XDefaultScreen(mDisplay);

    mScreenWidth = XDisplayWidth(mDisplay, mScreenNumber);
    mScreenHeight = XDisplayHeight(mDisplay, mScreenNumber);

    mRootWindow = XDefaultRootWindow(mDisplay);
    
    createBinder();
}

void RGlutMaemoInterface::terminate()
{
    if (mBinder) {
        mBinder->terminate();
        delete mBinder;
        mBinder = 0;
    }

    removeAllControl();

    if (mDisplay) {
        XCloseDisplay(mDisplay);
    }
}

void RGlutMaemoInterface::initDisplayMode(unsigned int mode)
{
    mDisplayMode = mode;

    // buffer size 32 is not working properly
    mBinder->setBufferSize(16);

    int rgb = mDisplayMode & 1; 
    if (rgb == 0) {
        // GLUT_RGBA or GLUT_RGB
        //mBinder->setRedSize(8);
        //mBinder->setGreenSize(8);
        //mBinder->setBlueSize(8);
        //mBinder->setAlphaSize(8);
    }
        
    if (mDisplayMode & GLUT_DEPTH) {
        mBinder->setDepth(true);
    }
    
    if (mDisplayMode & GLUT_STENCIL) {
        mBinder->setStencil(true);
    }
}

int RGlutMaemoInterface::createWindow()
{
    int width = mScreenWidth;
    int height = mScreenHeight;
    int x = 0;
    int y = 0;

    if (mWindowProperty.mWidth > 0 && mWindowProperty.mHeight > 0) {
        width = mWindowProperty.mWidth;
        height = mWindowProperty.mHeight;
    }

    if (mWindowProperty.mX > 0) {
        x = mWindowProperty.mX;
    }
    if (mWindowProperty.mY > 0) {
        y = mWindowProperty.mY;
    }


    XSetWindowAttributes wa;
    wa.event_mask  =  StructureNotifyMask |
                      SubstructureNotifyMask |
                      ExposureMask | 
                      PointerMotionMask |
                      KeyPressMask |
                      KeyReleaseMask |
                      ResizeRedirectMask | 
                      ButtonMotionMask |
                      ButtonPressMask |
                      ButtonReleaseMask;
    wa.background_pixmap = None;
    wa.background_pixel = 0;
    wa.border_pixel = 0;
    

    unsigned long mask = CWBackPixmap | CWEventMask;
    Window window = XCreateWindow(mDisplay, mRootWindow,
                                  x, y, width, height,
                                  0,
                                  CopyFromParent, InputOutput,
                                  CopyFromParent, mask,
                                  &wa);

    XSetWindowAttributes  xattr;
    xattr.override_redirect = False;
    XChangeWindowAttributes(mDisplay, window, CWOverrideRedirect, &xattr);

    int one = 1;
    XChangeProperty(mDisplay, window,
                    XInternAtom(mDisplay, "_HILDON_NON_COMPOSITED_WINDOW", True),
                    XA_INTEGER,  32,  PropModeReplace,
                    (unsigned char*)&one,  1);

    XWMHints hints;
    hints.input = True;
    hints.flags = InputHint;
    XSetWMHints(mDisplay, window, &hints);
 
    XMapWindow(mDisplay, window);
    XStoreName(mDisplay, window, mWindowProperty.mTitle);

    REGLGlutGLBinder::EGLSurfaceInfo surfaceInfo;
    surfaceInfo.mType = REGLGlutGLBinder::EGLSurfaceInfo::TYPE_WINDOW;
    surfaceInfo.mData = (void *)window;

    ControlEntry entry;
    entry.mSurface = mBinder->createSurface(
                                (RGlutGLBinder::Surface)&surfaceInfo, 0, 0);

    if (entry.mSurface == 0) {
        XDestroyWindow(mDisplay, window);
        return 0;
    }

    entry.mControl = window;

    int id = addControl(entry);

    setWindow(id);
    
    return id;
}

int RGlutMaemoInterface::addControl(ControlEntry entry)
{
    entry.mId = ID_START_INDEX + mControllist.size();
    mControllist.push_back(entry);   
    
    return entry.mId;
}

RGlutMaemoInterface::ControlEntry * RGlutMaemoInterface::getControlEntry(int id)
{
    ControlEntry * entry = 0;
    int count = mControllist.size();
    
    for (int i=0; i<count; ++i) {
        if (mControllist[i].mId == id) {
            entry = &mControllist[i];
            break;
        }
    }
    
    return entry;
}

RGlutMaemoInterface::ControlEntry RGlutMaemoInterface::removeControl(int id)
{
    ControlEntry entry;
    
    if (getControlEntry(id)) {
        if (id == mCurrentControl) {
            mCurrentControl = 0;
        }
        
        unsigned int index = id - ID_START_INDEX;
        if (index < mControllist.size()) {
            entry = mControllist[index];
            XDestroyWindow(mDisplay, entry.mControl);
            mControllist.erase(mControllist.begin() + index);
        }
    }
    
    return entry;
}

void RGlutMaemoInterface::removeAllControl()
{
    int count = mControllist.size();

    for (int i=0; i<count; ++i) {
        XDestroyWindow(mDisplay, mControllist[i].mControl);
    }
    mControllist.clear();
}

void RGlutMaemoInterface::destroyWindow(int win)
{
    ControlEntry entry = removeControl(win);
    if (entry.mSurface) {
        mBinder->destroySurface(entry.mSurface);
    }
}

int RGlutMaemoInterface::getWindow()
{
    return mCurrentControl;
}

void RGlutMaemoInterface::setWindow(int win)
{
    if (win != mCurrentControl) {
        ControlEntry * entry = getControlEntry(win);
        if (entry) {
            mBinder->makeCurrent(entry->mSurface);
            mCurrentControl = entry->mId;
        }
    }
}

void RGlutMaemoInterface::fullScreen()
{
    mFullScreen = true;
    ControlEntry * entry = getControlEntry(mCurrentControl);
    if (entry) {
        Atom atom = XInternAtom(mDisplay, "_NET_WM_STATE_FULLSCREEN", True);

        XChangeProperty(mDisplay, entry->mControl,
                    XInternAtom(mDisplay, "_NET_WM_STATE", True),
                    XA_ATOM,  32,  PropModeReplace,
                    (unsigned char*)&atom,  1);
        Atom wmState = XInternAtom(mDisplay, "_NET_WM_STATE", False);
        Atom fullscreen = XInternAtom(mDisplay, "_NET_WM_STATE_FULLSCREEN", False);

        XEvent xev;
        memset(&xev, 0, sizeof(xev));

        xev.type                 = ClientMessage;
        xev.xclient.window       = entry->mControl;
        xev.xclient.message_type = wmState;
        xev.xclient.format       = 32;
        xev.xclient.data.l[0]    = 1;
        xev.xclient.data.l[1]    = fullscreen;

        XSendEvent(mDisplay, mRootWindow, False, SubstructureNotifyMask, &xev);
    }
}

void RGlutMaemoInterface::timerFunc(unsigned int millis, void (*func)(int), int value)
{
    TimerEntry entry;
    entry.mMilliSec = millis;
    entry.mCallBack = func;
    entry.mValue = value;

    struct timeval now;
    gettimeofday(&now, 0);
    entry.mSetTime = now.tv_usec/1000 + now.tv_sec*1000;

    mTimers.insert(entry);
}

void RGlutMaemoInterface::checkTimers()
{
    if (mTimers.empty()) {
        return;
    }

    struct timeval now;
    gettimeofday(&now, 0);
    unsigned int timems = now.tv_usec/1000 + now.tv_sec*1000;

    TimerEntrySetIter iter;
    for (iter = mTimers.begin(); iter != mTimers.end(); ++iter) {
        if (timems - iter->mSetTime > iter->mMilliSec) {
            iter->mCallBack(iter->mValue);
        } else {
            break;
	}
    }

    if (iter != mTimers.begin()) {
        mTimers.erase(mTimers.begin(), iter--);
    }
}

int RGlutMaemoInterface::getModifiers(int state)
{
    int ret = 0;

    if (state & (ShiftMask | LockMask)) {
        ret |= GLUT_ACTIVE_SHIFT;
    }

    if (state & ControlMask) {
        ret |= GLUT_ACTIVE_CTRL;
    }

    if (state & Mod1Mask) {
        ret |= GLUT_ACTIVE_ALT;
    }

    return ret;
}


void RGlutMaemoInterface::exec()
{
    ControlEntry * entry = getControlEntry(mCurrentControl);
    if (entry) {
        XWindowAttributes wa;
        if (XGetWindowAttributes(mDisplay, entry->mControl, &wa)) {
            reshape(wa.width, wa.height);
        }
    }

    while (!mFinished) {
        while (XPending(mDisplay)) {
            XEvent  xev;
            XNextEvent(mDisplay, &xev);

            GLUTES_DEBUGP2("Event revieved %d", xev.type);

            switch (xev.type) {
                //case KeyRelease:
                case KeyPress: {
                    if (mCallbacks.keyboard) {
                        char asciiTable[64];
                        XComposeStatus cs;
                        KeySym keySym;
                        int length;

                        length = XLookupString(&xev.xkey, asciiTable, sizeof(asciiTable), &keySym, &cs);
                        if (length > 0) {
                            mModifier = getModifiers(xev.xkey.state);
                            mCallbacks.keyboard(asciiTable[0], xev.xkey.x, xev.xkey.y);
                        }
                        mModifier = 0xffffffff;
                    }

                    break;
                }

                case ButtonPress:
                case ButtonRelease: {
                    int button = 0;
                    bool pressed = false;

                    if (xev.type == ButtonPress) {
                        pressed = true;
                    }

                    switch (xev.xbutton.button) {
                        case Button1Mask:
                            button = GLUT_LEFT_BUTTON;
                            break;

                        case Button2Mask:
                            button = GLUT_MIDDLE_BUTTON;
                            break;

                        case Button3Mask:
                            button = GLUT_RIGHT_BUTTON;
                            break;

                        case Button4Mask:
                        case Button5Mask:
                            break;
                    }

                    mModifier = getModifiers(xev.xmotion.state);

                    if (mCallbacks.mouse) {
                        mCallbacks.mouse(button, pressed ? GLUT_DOWN : GLUT_UP, xev.xbutton.x, xev.xbutton.y);
                    }

                    break;
                }

                case MotionNotify: {
                    mModifier = getModifiers(xev.xmotion.state);

                    if (xev.xmotion.state & (Button1Mask | Button2Mask | Button3Mask | Button4Mask | Button5Mask)) {
                        if (mCallbacks.motion) {
                            mCallbacks.motion(xev.xmotion.x, xev.xmotion.y);
                        }
                    } else {
                        if (mCallbacks.passiveMotion) {
                            mCallbacks.passiveMotion(xev.xmotion.x, xev.xmotion.y);
                        }
                    }
                    break;
                }

                case Expose:
                case GraphicsExpose:
                case VisibilityNotify: {
                    draw();
                    break;
                }

                case ResizeRequest: {
                    reshape(xev.xresizerequest.width, xev.xresizerequest.height);
                    break;
                }
            }
        }

        checkTimers();

        usleep(1000 * 5);
    }
}

void RGlutMaemoInterface::redraw(int win)
{
    draw();
}

void RGlutMaemoInterface::flush()
{
    if (mBinder) {
        ControlEntry * entry = getControlEntry(mCurrentControl);
        if (entry) {
            mBinder->swapBuffer(entry->mSurface);

        }
    }
}

void RGlutMaemoInterface::setWindowTitle(const char * title)
{
    ControlEntry * entry = getControlEntry(mCurrentControl);
    if (entry) {
        XStoreName(mDisplay, entry->mControl, title);
    }
}

void RGlutMaemoInterface::showWindow()
{
    ControlEntry * entry = getControlEntry(mCurrentControl);
    if (entry) {
        XMapWindow(mDisplay, entry->mControl);
    }
}

void RGlutMaemoInterface::hideWindow()
{
    ControlEntry * entry = getControlEntry(mCurrentControl);
    if (entry) {
        XUnmapWindow(mDisplay, entry->mControl);
    }
}

void RGlutMaemoInterface::positionWindow(int x, int y)
{
    ControlEntry * entry = getControlEntry(mCurrentControl);
    if (entry) {
        int mask = CWX | CWY;
        XWindowChanges wc;
        wc.x = x;
        wc.y = y;
        XConfigureWindow(mDisplay, entry->mControl, mask, &wc);
    }
}

void RGlutMaemoInterface::reshapeWindow(int width, int height)
{
    ControlEntry * entry = getControlEntry(mCurrentControl);
    if (entry) {
        int mask = CWWidth | CWHeight;
        XWindowChanges wc;
        wc.width = width;
        wc.height = height;
        XConfigureWindow(mDisplay, entry->mControl, mask, &wc);
    }
}

void RGlutMaemoInterface::draw()
{
    if (mCallbacks.draw) {
        mCallbacks.draw();
    }
}

void RGlutMaemoInterface::reshape(int w, int h)
{
    GLUTES_DEBUGP3("Reshaping called with Size(%d, %d)", w, h);
    if (mCallbacks.reshape) {
        GLUTES_DEBUGP3("Reshaping with Size(%d, %d)", w, h);
        mCallbacks.reshape(w, h);
    }
}

void RGlutMaemoInterface::rerect(int x, int y, int w, int h)
{
    if (mCallbacks.reshape) {
        mCallbacks.reshape(w, h);
    }
    if (mCallbacks.repos) {
        mCallbacks.repos(x, y);
    }
}

void RGlutMaemoInterface::repos(int x, int y)
{
    if (mCallbacks.repos) {
        mCallbacks.repos(x, y);
    }
}

int RGlutMaemoInterface::getModifiers()
{
    return mModifier;
}

int RGlutMaemoInterface::getValue(unsigned int state)
{
    int value = 0;

    switch (state) {
        case GLUT_WINDOW_WIDTH: {
            ControlEntry * entry = getControlEntry(mCurrentControl);
            if (entry) {
                XWindowAttributes wa;
                if (XGetWindowAttributes(mDisplay, entry->mControl, &wa)) {
                    value = wa.width;
                }
            }
            break;
        }

        case GLUT_WINDOW_HEIGHT: {
            ControlEntry * entry = getControlEntry(mCurrentControl);
            if (entry) {
                XWindowAttributes wa;
                if (XGetWindowAttributes(mDisplay, entry->mControl, &wa)) {
                    value = wa.height;
                }
            }
            break;
        }

        case GLUT_WINDOW_X: {
            ControlEntry * entry = getControlEntry(mCurrentControl);
            if (entry) {
                XWindowAttributes wa;
                if (XGetWindowAttributes(mDisplay, entry->mControl, &wa)) {
                    value = wa.x;
                }
            }
            break;
        }

        case GLUT_WINDOW_Y: {
            ControlEntry * entry = getControlEntry(mCurrentControl);
            if (entry) {
                XWindowAttributes wa;
                if (XGetWindowAttributes(mDisplay, entry->mControl, &wa)) {
                    value = wa.y;
                }
            }
            break;
        }

        case GLUT_WINDOW_BUFFER_SIZE: {
            if (mBinder) {
                value = mBinder->getBufferSize();
            }
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
            value = mScreenWidth;
            break;
        }

        case GLUT_SCREEN_HEIGHT: {
            value = mScreenHeight;
            break;
        }
    }

    return value;
}


void RGlutMaemoInterface::popWindow()
{
    //TODO
}

void RGlutMaemoInterface::pushWindow()
{
    //TODO
}

int RGlutMaemoInterface::createMenu(void (*)(int menu))
{
    //TODO
    return 0;
}

void RGlutMaemoInterface::destroyMenu(int)
{
    //TODO
}

int RGlutMaemoInterface::getMenu()
{
    //TODO
    return 0;
}

void RGlutMaemoInterface::setMenu(int)
{
    //TODO
}

void RGlutMaemoInterface::addMenuEntry(const char *, int)
{
    //TODO
}

void RGlutMaemoInterface::removeMenuItem(int)
{
    //TODO
}

void RGlutMaemoInterface::attachMenu(int)
{
    //TODO
}

void RGlutMaemoInterface::detachMenu(int)
{
    //TODO
}

