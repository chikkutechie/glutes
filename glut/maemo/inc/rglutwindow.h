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

#ifndef _GLUTWINDOW_H_
#define _GLUTWINDOW_H_

#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include <string>

class RGlutWindow
{
public:
    RGlutWindow(RGlutWindow * parent = 0);
    virtual ~RGlutWindow();

    void setPos(int x, int y);
    void setSize(int w, int h);
    void setGeometry(int x, int y, int w, int h);

    void setTitle(std::string const & title);
    void setFullscreen();

    bool isVisible() const
    {
        return mVisible;
    }
    virtual void show();
    virtual void hide();

    virtual void draw();

    virtual bool handleEvent(XEvent * event);

    Display * display() const
    {
        return mDisplay;
    }

    Window window();

protected:
    virtual void create();
    virtual void destroy();

protected:
    Display * mDisplay;
    RGlutWindow * mParent;
    Window mParentWindow;
    Window mWindow;
    GC mGC;
    int mX;
    int mY;
    int mWidth;
    int mHeight;
    bool mVisible;
    std::string mTitle;
};

#endif

