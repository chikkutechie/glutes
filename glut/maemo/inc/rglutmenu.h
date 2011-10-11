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

#ifndef _GLUTMENU_H_
#define _GLUTMENU_H_

#include <string>
#include <vector>

#include "rglutwindow.h"
#include "rglutcolor.h"

class RGlutGC;

class RGlutMenu: public RGlutWindow
{
public:
    RGlutMenu(void (*callback)(int), RGlutWindow * parent = 0);
    ~RGlutMenu();

    void addEntry(std::string name, int id);

    void removeEntry(int id)
    {
    }

    RGlutPointI preferedPos(int x, int y);

    void hide();
    void draw();
    
    bool handleEvent(XEvent & event);

private:
    class MenuEntry
    {
    public:
        MenuEntry()
        {}
        MenuEntry(std::string name, int id)
            : mName(name),
              mId(id)
        {}

        std::string mName;
        int mId;
        int mX;
        int mY;
        int mWidth;
        int mHeight;
    };

private:
    RGlutMenu(RGlutMenu const &);
    RGlutMenu & operator=(RGlutMenu const &);

    void create();
    void destroy();
    void drawBackground();
    void drawItemBackgroundPressed(int x, int y, int w, int h);
    void drawItemBackgroundNormal(int x, int y, int w, int h);
    void createPixmaps(int w, int h);
    void updateSize();
    MenuEntry * getMenuEntry(int x, int y);

private:
    std::vector<MenuEntry> mMenuEntries;
    void (*mCallback)(int);
    int mPressedId;
    RGlutColor mColor;
    Pixmap mItemNormalPixmap;
    Pixmap mItemPressedPixmap;
    bool mPixmapCreated;
    RGlutColor mTextColor;
    RGlutGC * mGC;
    int mMenuGap;
    int mMinMenuWidth;
    int mMinMenuItemHeight;
    int mMaxMenuWidth;
    int mMaxMenuItemHeight;
};

#endif

