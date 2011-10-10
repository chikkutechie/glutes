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

#include "rglutwindow.h"
#include "rglutdisplay.h"
#include "rcommon.h"

RGlutWindow::RGlutWindow(RGlutWindow * parent)
  : mDisplay(0),
    mParent(parent),
    mWindow(0),
    mX(0),
    mY(0),
    mWidth(100),
    mHeight(100),
    mVisible(false)
{
    if (mParent) {
        mDisplay = mParent->mDisplay;
        mParentWindow = mParent->mWindow;
        mParent->addChild(this);
    } else {
        mDisplay = RGlutDisplay::instance()->display();
        mParentWindow = XDefaultRootWindow(mDisplay);
    }
}

RGlutWindow::~RGlutWindow()
{
    destroy();
}

void RGlutWindow::setTitle(std::string const & title)
{
    mTitle = title;
    if (mWindow) {
        XStoreName(mDisplay, mWindow, mTitle.c_str());
    }
}

void RGlutWindow::setFullscreen()
{
    if (mWindow) {
        Atom atom = XInternAtom(mDisplay, "_NET_WM_STATE_FULLSCREEN", True);

        XChangeProperty(mDisplay, mWindow,
                        XInternAtom(mDisplay, "_NET_WM_STATE", True),
                        XA_ATOM,  32,  PropModeReplace,
                        (unsigned char*)&atom,  1);
        Atom wmState = XInternAtom(mDisplay, "_NET_WM_STATE", False);
        Atom fullscreen = XInternAtom(mDisplay, "_NET_WM_STATE_FULLSCREEN", False);

        XEvent xev;
        memset(&xev, 0, sizeof(xev));

        xev.type                 = ClientMessage;
        xev.xclient.window       = mWindow;
        xev.xclient.message_type = wmState;
        xev.xclient.format       = 32;
        xev.xclient.data.l[0]    = 1;
        xev.xclient.data.l[1]    = fullscreen;

        XSendEvent(mDisplay, XDefaultRootWindow(mDisplay), False, SubstructureNotifyMask, &xev);
    }
}

void RGlutWindow::setPos(int x, int y)
{
    if (x == mX && y == mY) {
        return;
    }

    mX = x;
    mY = y;

    if (mWindow) {
        int mask = CWX | CWY;
        XWindowChanges wc;
        wc.x = x;
        wc.y = y;
        XConfigureWindow(mDisplay, mWindow, mask, &wc);
    }
}

void RGlutWindow::setSize(int w, int h)
{
    mWidth = w;
    mHeight = h;
    if (mWindow) {
        int mask = CWWidth | CWHeight;
        XWindowChanges wc;
        wc.width = w;
        wc.height = h;
        XConfigureWindow(mDisplay, mWindow, mask, &wc);
    }
}
 
void RGlutWindow::setGeometry(int x, int y, int w, int h)
{
    mX = x;
    mY = y;
    mWidth = w;
    mHeight = h;
    if (mWindow) {
        int mask = CWWidth | CWHeight | CWX | CWY;
        XWindowChanges wc;
        wc.x = x;
        wc.y = y;
        wc.width = w;
        wc.height = h;
        XConfigureWindow(mDisplay, mWindow, mask, &wc);
    }
}
 
void RGlutWindow::setBackgroundColor(RGlutColor const & color)
{
    if (mWindow) {
        XSetWindowBackground(mDisplay, mWindow, color.pixel());
    }
}

void RGlutWindow::clear()
{
    if (mWindow) {
        XClearWindow(mDisplay, mWindow);
    }
}

void RGlutWindow::show()
{
    if (!mVisible) {
        create();
        XMapWindow(mDisplay, mWindow);
        XFlush(mDisplay);
        mVisible = true;
    }
}

Window RGlutWindow::window()
{
    create();
    return mWindow;
}

void RGlutWindow::hide()
{
    if (mVisible) {
        mVisible = false;
        XUnmapWindow(mDisplay, mWindow);
    }
}

bool RGlutWindow::handleEvent(XEvent & event)
{
    bool handled = false;
    if (isVisible()) {    
        for (WindowSetIter iter = mChilds.begin(); iter != mChilds.end() && !handled; ++iter) {
            if ((*iter)->isVisible()) {
                handled = (*iter)->handleEvent(event);
            }
        }
    }

    return handled;
}

// redraw just calls draw directly
// might need to send an event to X server
// and the server should send the expose event back
void RGlutWindow::redraw()
{
    draw();
}

void RGlutWindow::draw()
{
    if (isVisible()) {
        for (WindowSetIter iter = mChilds.begin(); iter != mChilds.end(); ++iter) {
            if ((*iter)->isVisible()) {
                (*iter)->draw();
            }
        }
    }
}

void RGlutWindow::destroy()
{
    XDestroyWindow(mDisplay, mWindow);
    mWindow = 0;
}

void RGlutWindow::create()
{
    if (mWindow) {
        return;
    }
    
    XSetWindowAttributes theWindowAttributes;
    theWindowAttributes.override_redirect = False;

    theWindowAttributes.event_mask  =  StructureNotifyMask |
                                       SubstructureNotifyMask |
                                       ExposureMask |
                                       PointerMotionMask |
                                       KeyPressMask |
                                       KeyReleaseMask |
                                       ResizeRedirectMask |
                                       ButtonMotionMask |
                                       ButtonPressMask |
                                       ButtonReleaseMask;

    int theWindowMask = CWEventMask |
                        CWOverrideRedirect;

    mWindow = XCreateWindow(mDisplay,
                            mParentWindow,
                            mX, mY, mWidth, mHeight,
                            0, CopyFromParent,
                            InputOutput,
                            CopyFromParent,
                            theWindowMask,
                            &theWindowAttributes);

    XWMHints hints;
    hints.input = True;
    hints.flags = InputHint;
    XSetWMHints(mDisplay, mWindow, &hints);
    XStoreName(mDisplay, mWindow, mTitle.c_str());
}

void RGlutWindow::removeChild(RGlutWindow * window)
{
    WindowSetIter iter = mChilds.find(window);
    if (iter != mChilds.end()) {
        mChilds.erase(iter);
    }
}

void RGlutWindow::removeFromParent()
{
    if (mParent) {
        mParent->removeChild(this);
    }
}
