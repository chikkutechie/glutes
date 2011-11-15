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

#ifndef _GLUTWINDOW_H_
#define _GLUTWINDOW_H_

#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>

#include <string>
#include <set>

#include "rglutcolor.h"
#include "rglutpoint.h"
#include "rglutsize.h"
#include "rglutrect.h"

/**
 * @class RGlutWindow
 * @brief Window class implementation, wrapper for X11 window.
 */
class RGlutWindow
{
public:
    RGlutWindow(RGlutWindow *parent = 0);
    virtual ~RGlutWindow();

    /*!
     * Retrieves the window position.
     * @see RGlutPointI
     */
    RGlutPointI pos() const {
        return RGlutPointI(mX, mY);
    }
    
    /*!
     * Sets the window position.
     */
    virtual void setPos(int x, int y);

    /*!
     * Retrieves the size of the window.
     * @see RGlutSizeI
     */
    RGlutSizeI size() const {
        return RGlutSizeI(mWidth, mHeight);
    }
    
    /*!
     * Sets the size of the window.
     */
    virtual void setSize(int w, int h);

    /*!
     * Sets the geometry for the window. Geometry is postion + size.
     */
    virtual void setGeometry(int x, int y, int w, int h);
    
    /*!
     * Retrieves the geometry for the window. Geometry is postion + size.
     * @see RGlutRectI
     */
    RGlutRectI geometry() const {
        return RGlutRectI(mX, mY, mWidth, mHeight);
    }

    /*!
     * Sets the titke for the window
     * @param title The string which should be set as the title.
     */
    void setTitle(std::string const &title);
    
    /*!
     * Make the window full screen
     */
    void setFullscreen();

    /*!
     * Sets the background color of the window.
     * @param color The color that should be applied to background
     */
    void setBackgroundColor(RGlutColor const &color);
    
    /*!
     * Clears the window.
     */
    void clear();

    /*!
     * Checks whether the window is visible.
     * @return true is window is visible
     */
    bool isVisible() const {
        return mVisible;
    }
    
    /*!
     * Shows the window. The parent window should be visible inorder the window to be visible.
     */
    virtual void show();
    
    /*!
     * Hides the window.
     */
    virtual void hide();

    /*!
     * Causes the window to redraw.
     */
    virtual void redraw();
    
    /*!
     * Callback for draw event.
     */
    virtual void draw();

    /*!
     * Event handler function.
     */
    virtual bool handleEvent(XEvent &event);

    /*!
     * Returns the X11 Display object associated with the window.
     */
    Display *display() const {
        return mDisplay;
    }

    /*!
     * Returns the X11 Window object associated with the window.
     */
    Window window();

    /*!
     * Returns the parent window.
     */
    RGlutWindow *parent() {
        return mParent;
    }

    /*!
     * Adds the given window as a child
     * @param window The window which should be added as a child
     */
    void addChild(RGlutWindow *window) {
        if (window) {
            mChilds.insert(window);
        }
    }

    /*!
     * Removes the window from child list.
     * @param window The window that should be removed from child list
     */
    void removeChild(RGlutWindow *window);

    /*!
     * Remove this window from the parents child list.
     */
    void removeFromParent();

private:
    RGlutWindow(RGlutWindow const &);
    RGlutWindow &operator=(RGlutWindow const &);

protected:
    virtual void create();
    virtual void destroy();

protected:
    Display *mDisplay;
    RGlutWindow *mParent;
    Window mParentWindow;
    Window mWindow;
    int mX;
    int mY;
    int mWidth;
    int mHeight;
    bool mVisible;
    std::string mTitle;

    typedef std::set<RGlutWindow *> WindowSet;
    typedef WindowSet::iterator WindowSetIter;
    typedef WindowSet::const_iterator WindowSetConstIter;

    WindowSet  mChilds;
};

#endif

