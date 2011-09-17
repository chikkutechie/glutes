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
    mScreenNumber(0),
    mScreenWidth(0),
    mScreenHeight(0),
    mCurrentControl(0),
    mFullScreen(false),
    mModifier(0),
    mFinished(false),
    mCurrentMenu(0),
    mAttachedMenuButton(-1)
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
    
    mBinder->setNativeDisplay((int)RGlutDisplay::instance()->display());
    mBinder->initialize();
}

void RGlutMaemoInterface::intialize(int argc, char ** argv)
{    
    parseArguments(argc, argv);

    Display * display = RGlutDisplay::instance()->display();
    if (display == 0) {
        return;
    }

    GLUTES_DEBUGP2("Display %d", display);

    mScreenNumber = XDefaultScreen(display);

    mScreenWidth = XDisplayWidth(display, mScreenNumber);
    mScreenHeight = XDisplayHeight(display, mScreenNumber);

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

    RGlutWindow * window = new RGlutWindow;
    window->setGeometry(x, y, width, height);
    window->setTitle(mWindowProperty.mTitle);
    window->show();

    REGLGlutGLBinder::EGLSurfaceInfo surfaceInfo;
    surfaceInfo.mType = REGLGlutGLBinder::EGLSurfaceInfo::TYPE_WINDOW;
    surfaceInfo.mData = (void *)window->window();

    GLUTES_DEBUGP2("Window Created %d", window->window());

    ControlEntry entry;
    entry.mSurface = mBinder->createSurface(
                                (RGlutGLBinder::Surface)&surfaceInfo, 0, 0);
    if (entry.mSurface == 0) {
        delete window;
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
            delete entry.mControl;
            mControllist.erase(mControllist.begin() + index);
        }
    }
    
    return entry;
}

void RGlutMaemoInterface::removeAllControl()
{
    int count = mControllist.size();

    for (int i=0; i<count; ++i) {
        delete mControllist[i].mControl;
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
        entry->mControl->setFullscreen();
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

    std::vector<TimerEntry> expiredTimers;

    for (TimerEntrySetIter iter = mTimers.begin(); iter != mTimers.end(); ++iter) {
        if (timems - iter->mSetTime > iter->mMilliSec) {
            expiredTimers.push_back(*iter);
        } else {
            break;
	}
    }

    unsigned int expiredTimersCount = expiredTimers.size();
    for (unsigned int i = 0; i < expiredTimersCount; ++i) {
        mTimers.erase(expiredTimers[i]);
    }

    for (unsigned int i = 0; i < expiredTimersCount; ++i) {
        expiredTimers[i].mCallBack(expiredTimers[i].mValue);
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
    Display * display = RGlutDisplay::instance()->display();

    ControlEntry * entry = getControlEntry(mCurrentControl);
    if (entry) {
        XWindowAttributes wa;
        if (XGetWindowAttributes(display, entry->mControl->window(), &wa)) {
            reshape(wa.width, wa.height);
        }
    }

    while (!mFinished) {
        while (XPending(display)) {
            XEvent  xev;
            XNextEvent(display, &xev);

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

                    if (mAttachedMenuButton == button) {
                        RGlutMenu * menu = mMenus[mCurrentMenu-1];
                        if (menu->isVisible() && pressed) {
                            if (!menu->handleEvent(&xev)) {
                                menu->hide();
                            }
                        } else if (!menu->isVisible() && pressed) {
                            menu->setPos(xev.xbutton.x, xev.xbutton.y);
                            menu->show();
                        } else {
                            menu->handleEvent(&xev);
                        }
                    } else if (mCallbacks.mouse) {
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

                case Expose: {
                    if (xev.xexpose.count == 0) {
                        draw();
                    }
                    break;
                }

                case ConfigureNotify: {
    		    entry = getControlEntry(mCurrentControl);
                    if (entry) {
                        if (entry->mControl->window() == xev.xconfigure.window) {
                            reshape(xev.xconfigure.width, xev.xconfigure.height);
                        }
                    }
                }
                break;

                case ResizeRequest: {
    		    entry = getControlEntry(mCurrentControl);
                    if (entry) {
                        if (entry->mControl->window() == xev.xresizerequest.window) {
                            reshape(xev.xresizerequest.width, xev.xresizerequest.height);
                        }
                    }
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
        entry->mControl->setTitle(title);
    }
}

void RGlutMaemoInterface::showWindow()
{
    ControlEntry * entry = getControlEntry(mCurrentControl);
    if (entry) {
        entry->mControl->show();
    }
}

void RGlutMaemoInterface::hideWindow()
{
    ControlEntry * entry = getControlEntry(mCurrentControl);
    if (entry) {
        entry->mControl->hide();
    }
}

void RGlutMaemoInterface::positionWindow(int x, int y)
{
    ControlEntry * entry = getControlEntry(mCurrentControl);
    if (entry) {
        entry->mControl->setPos(x, y);
    }
}

void RGlutMaemoInterface::reshapeWindow(int width, int height)
{
    ControlEntry * entry = getControlEntry(mCurrentControl);
    if (entry) {
        entry->mControl->setSize(width, height);
    }
}

void RGlutMaemoInterface::draw()
{
    if (mCallbacks.draw) {
        mCallbacks.draw();
    }
    int id = mCurrentMenu-1;
    if (id >= 0 && id < mMenus.size()) {
        if (mMenus[id]->isVisible()) {
            mMenus[id]->draw();
        }
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
    Display * display = RGlutDisplay::instance()->display();

    switch (state) {
        case GLUT_WINDOW_WIDTH: {
            ControlEntry * entry = getControlEntry(mCurrentControl);
            if (entry) {
                XWindowAttributes wa;
                if (XGetWindowAttributes(display, entry->mControl->window(), &wa)) {
                    value = wa.width;
                }
            }
            break;
        }

        case GLUT_WINDOW_HEIGHT: {
            ControlEntry * entry = getControlEntry(mCurrentControl);
            if (entry) {
                XWindowAttributes wa;
                if (XGetWindowAttributes(display, entry->mControl->window(), &wa)) {
                    value = wa.height;
                }
            }
            break;
        }

        case GLUT_WINDOW_X: {
            ControlEntry * entry = getControlEntry(mCurrentControl);
            if (entry) {
                XWindowAttributes wa;
                if (XGetWindowAttributes(display, entry->mControl->window(), &wa)) {
                    value = wa.x;
                }
            }
            break;
        }

        case GLUT_WINDOW_Y: {
            ControlEntry * entry = getControlEntry(mCurrentControl);
            if (entry) {
                XWindowAttributes wa;
                if (XGetWindowAttributes(display, entry->mControl->window(), &wa)) {
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

int RGlutMaemoInterface::createMenu(void (*cb)(int))
{
    int id = 0;
    ControlEntry * entry = getControlEntry(mCurrentControl);
    if (entry) {
        mMenus.push_back(new RGlutMenu(cb, entry->mControl));
        id = mMenus.size();
        mCurrentMenu = id;
    }
    return id;
}

void RGlutMaemoInterface::destroyMenu(int id)
{
    id = id -1;

    if (id < mMenus.size()) {
        if (mCurrentMenu == id) {
            mCurrentMenu = 0;
        }
        delete mMenus[id];
        mMenus.erase(mMenus.begin() + id);
    }
}

int RGlutMaemoInterface::getMenu()
{
    return mCurrentMenu;
}

void RGlutMaemoInterface::setMenu(int id)
{
    id = id-1;
    if (id >= 0 && id < mMenus.size()) {
        mCurrentMenu = id;
    }
}

void RGlutMaemoInterface::addMenuEntry(const char *name, int eid)
{
    int id = mCurrentMenu-1;
    if (id >= 0 && id < mMenus.size()) {
        mMenus[id]->addEntry(name, eid);
    }
}

void RGlutMaemoInterface::removeMenuItem(int eid)
{
    int id = mCurrentMenu-1;
    if (id >= 0 && id < mMenus.size()) {
        mMenus[id]->removeEntry(eid);
    }
}

void RGlutMaemoInterface::attachMenu(int b)
{
    if (mCurrentMenu) {
        mAttachedMenuButton = b;
    }
}

void RGlutMaemoInterface::detachMenu(int b)
{
    if (mCurrentMenu && mAttachedMenuButton == b) {
        mAttachedMenuButton = -1;
    }
}

void RGlutMaemoInterface::popWindow()
{
    //TODO
}

void RGlutMaemoInterface::pushWindow()
{
    //TODO
}

