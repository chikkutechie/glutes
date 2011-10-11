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

#include "rglutmenu.h"
#include "rglutdisplay.h"
#include "rglutgc.h"
#include "rcommon.h"
#include "rglutlineargradient.h"
#include "rglutlaf.h"
#include "rglutapplication.h"

RGlutMenu::RGlutMenu(void (*callback)(int), RGlutWindow * parent)
 : RGlutWindow(parent),
   mCallback(callback),
   mPressedId(-1),
   mPixmapCreated(false)
{
    RGlutLAF * laf = RGlutApplication::activeApplication()->LAF();
    mColor = laf->getMenuBackgroundColor();
    mTextColor = laf->getMenuTextColor();
    mMenuGap = laf->menuItemGap();
    mMinMenuItemHeight = laf->minimumMenuItemHeight();
    mMinMenuWidth = laf->minimumMenuWidth();
    mMaxMenuItemHeight = laf->maximumMenuItemHeight();
    mMaxMenuWidth = laf->maximumMenuWidth();
}

RGlutMenu::~RGlutMenu()
{
    delete mGC;
}

void RGlutMenu::create()
{
    if (mWindow != 0) {
        return;
    }

    updateSize();

    RGlutWindow::create();

    mGC = new RGlutGC(this);
    mGC->setFont(RGlutApplication::activeApplication()->LAF()->menuItemFont());

    setBackgroundColor(mColor);
}

void RGlutMenu::updateSize()
{
    int textHeight = mMinMenuItemHeight;

    mWidth = mMinMenuWidth;
    mHeight = mMenuEntries.size() * textHeight;
}

void RGlutMenu::createPixmaps(int w, int h)
{
    if (mPixmapCreated) {
        return;
    }

    RGlutLAF * laf = RGlutApplication::activeApplication()->LAF();
    
    mItemNormalPixmap = laf->createMenuItemBackgroundNormal(w, h);
    mItemPressedPixmap = laf->createMenuItemBackgroundPressed(w, h);

    mPixmapCreated = true;
}

void RGlutMenu::destroy()
{
    if (mPixmapCreated) {
        XFreePixmap(RGlutDisplay::instance()->display(), mItemNormalPixmap);
        XFreePixmap(RGlutDisplay::instance()->display(), mItemPressedPixmap);
        mPixmapCreated = false;
    }

    delete mGC;
    mGC = 0;

    RGlutWindow::destroy();
}

void RGlutMenu::addEntry(std::string name, int id)
{
    mMenuEntries.push_back(MenuEntry(name, id));
    if (mWindow) {
        destroy();
        create();
    }
}

void RGlutMenu::drawBackground()
{
    clear();
}

void RGlutMenu::drawItemBackgroundNormal(int x, int y, int w, int h)
{
    mGC->drawPixmap(mItemNormalPixmap, 0, 0, w-2*mMenuGap, h-2*mMenuGap, x+mMenuGap, y+mMenuGap);
}

void RGlutMenu::drawItemBackgroundPressed(int x, int y, int w, int h)
{
    mGC->drawPixmap(mItemPressedPixmap, 0, 0, w-2*mMenuGap, h-2*mMenuGap, x+mMenuGap, y+mMenuGap);
}

RGlutPointI RGlutMenu::preferedPos(int x, int y)
{
    if (mParent) {
        // change the origin of the menu, if not properly fit into the current window
        GLUTES_DEBUGP3("Original (x, y) = (%d, %d)", mX, mY);
        
        if (!mWindow) {
            updateSize();
        }

        if (x+mWidth > mParent->size().width()) {
            x = x - mWidth;
        }
        if (y+mHeight > mParent->size().height()) {
            y = y - mHeight;
        }
        GLUTES_DEBUGP3("Modified (x, y) = (%d, %d)", x, y);
    }

    return RGlutPointI(x, y);
}

void RGlutMenu::hide()
{
   RGlutWindow::hide();
   mPressedId = -1;
}

void RGlutMenu::draw()
{
    if (!mVisible) {
        return;
    }

    int itemHeight = mHeight / mMenuEntries.size();
    int x = 0;
    int y = 0;
    int w = mWidth;
    int h = itemHeight;
    int hw = h/2;

    create();
    createPixmaps(w-2*mMenuGap, h-2*mMenuGap);
    drawBackground();

    for (unsigned int i=0; i<mMenuEntries.size(); ++i) {
        MenuEntry & me = mMenuEntries[i];
 
        // set the background according to the pressed state
        if (mPressedId == me.mId) {
            drawItemBackgroundPressed(x, y, w, h);
        } else {
            drawItemBackgroundNormal(x, y, w, h);
        }

        // draw the text aligned to center
        int tx = (w - mGC->textWidth(me.mName)) / 2;
        int ty = itemHeight-hw;

        if (tx < 0) {
            tx = 0;
        }

        mGC->setForegroundColor(mTextColor);
        mGC->drawString(tx, ty, me.mName); 

        me.mX = x;
        me.mY = y;
        me.mWidth = w;
        me.mHeight = h;
        y = itemHeight;
        itemHeight += itemHeight;
    }
}

RGlutMenu::MenuEntry * RGlutMenu::getMenuEntry(int x, int y)
{
    MenuEntry * mentry = 0;

    for (unsigned int i=0; i<mMenuEntries.size(); ++i) {
        MenuEntry & me = mMenuEntries[i];
        bool outOfMenu = x < me.mX || x > me.mX + me.mWidth ||
                         y < me.mY || y > me.mY + me.mHeight;
        if (!outOfMenu) {
                mentry = &me;
        }
    }
    return mentry;
}

bool RGlutMenu::handleEvent(XEvent & xev)
{
    bool handled = false;

    if (xev.xany.window != mWindow) {
        return handled;
    }

    switch (xev.type) {
        case ButtonPress:
        case ButtonRelease: {
            handled = true;
            bool pressed = false;
            bool toRedraw = false;
            if (xev.type == ButtonPress) {
                pressed = true;
            }
            int x = xev.xbutton.x;
            int y = xev.xbutton.y;

            MenuEntry * mentry = getMenuEntry(x, y);
            if (mentry) {
                MenuEntry & me = *mentry;
                if (pressed) {
                    GLUTES_DEBUGP2("Pressed %s", me.mName.c_str());
                    mPressedId = me.mId;
                    toRedraw = true;
                } else {
                    if (mPressedId == me.mId) {
                        hide();
                        mCallback(me.mId);
                    } else {
                        // pressed and released item is not the same
                        // go back to the begining
                        toRedraw = true;
                        mPressedId = -1;
                    }
                }
            }
            if (toRedraw) {
                redraw();
            }
            break;
        }
        case MotionNotify: {
            handled = true;
            int x = xev.xmotion.x;
            int y = xev.xmotion.y;
            if (mPressedId != -1) {
                MenuEntry * mentry = getMenuEntry(x, y);
                if (!mentry) {
                    // released the mouse button outside the menu 
                    // hide the menu
                    GLUTES_DEBUGP3("Moved out of Menu (%d, %d)", xev.xmotion.x, xev.xmotion.y);
                    mPressedId = -1;
                    redraw();
                } else {
                    if (mPressedId != mentry->mId) {
                        mPressedId = mentry->mId;
                        redraw();
                    }
                }
            }
            break;
        }
    }

    return handled;
}

