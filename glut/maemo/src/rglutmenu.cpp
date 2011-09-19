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
#include "rglutgc.h"

#include <iostream>

RGlutMenu::RGlutMenu(void (*callback)(int), RGlutWindow * parent)
 : RGlutWindow(parent),
   mCallback(callback),
   mPressedId(-1),
   mColor(100, 100, 100),
   mItemNormalColor(200, 200, 200),
   mItemPressedColor(100, 100, 200),
   mTextColor(10, 10, 10)
{}

RGlutMenu::~RGlutMenu()
{
    delete mGC;
}

void RGlutMenu::create()
{
    if (mWindow != 0) {
        return;
    }

    int textHeight = MinMenuHeight;

    mWidth = MinMenuWidth;
    mHeight = mMenuEntries.size() * textHeight;

    RGlutWindow::create();

    mGC = new RGlutGC(this);

    setBackgroundColor(mColor);
}

void RGlutMenu::destroy()
{
    RGlutWindow::destroy();
    delete mGC;
    mGC = 0;
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
    mGC->setForegroundColor(mItemNormalColor);
    mGC->fillRectangle(x+MenuGap, y+MenuGap, w-2*MenuGap, h-2*MenuGap);
}

void RGlutMenu::drawItemBackgroundPressed(int x, int y, int w, int h)
{
    mGC->setForegroundColor(mItemPressedColor);
    mGC->fillRectangle(x+MenuGap, y+MenuGap, w-2*MenuGap, h-2*MenuGap);
}

void RGlutMenu::show()
{
    if (mParent) {
        bool changed = false;
        if (mX+mWidth > mParent->size().width()) {
            mX = mX - mWidth;
            changed = true;
        }
        if (mY+mHeight > mParent->size().height()) {
            mY = mY - mHeight;
            changed = true;
        }
        if (changed) {
            setPos(mX, mY); 
        }
    } 

    RGlutWindow::show();
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

    create();

    drawBackground();

    int itemHeight = mHeight / mMenuEntries.size();
    int x = 0;
    int y = 0;
    int w = mWidth;
    int h = itemHeight;
    int hw = h/2;
    for (unsigned int i=0; i<mMenuEntries.size(); ++i) {
        MenuEntry & me = mMenuEntries[i];
 
        if (mPressedId == me.mId) {
            drawItemBackgroundPressed(x, y, w, h);
        } else {
            drawItemBackgroundNormal(x, y, w, h);
        }

        mGC->setForegroundColor(mTextColor);
        mGC->drawString(5, itemHeight-hw, me.mName); 
        me.mX = x;
        me.mY = y;
        me.mWidth = w;
        me.mHeight = h;
        y = itemHeight;
        itemHeight += itemHeight;
    }
}

bool RGlutMenu::handleEvent(XEvent * xev)
{
    bool handled = false;

    switch (xev->type) {
        case ButtonPress:
        case ButtonRelease: {
            if (mWindow != xev->xbutton.window) {
                break;
            }
            handled = true;
            bool pressed = false;
            if (xev->type == ButtonPress) {
                pressed = true;
            }
            int x = xev->xbutton.x;
            int y = xev->xbutton.y;

            for (unsigned int i=0; i<mMenuEntries.size(); ++i) {
                MenuEntry & me = mMenuEntries[i];
                bool outOfMenu = x < me.mX || x > me.mX + me.mWidth ||
                                 y < me.mY || y > me.mY + me.mHeight;
                if (!outOfMenu) {
                    if (pressed) {
                        mPressedId = me.mId;
                    } else {
                        hide();
             	        mCallback(me.mId);
                    }
                    break;
                }
            }
        }
    }

    return handled;
}

