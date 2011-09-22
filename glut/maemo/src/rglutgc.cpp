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

#include "rglutgc.h"
#include "rglutwindow.h"
#include "rglutdisplay.h"

RGlutGC::RGlutGC(RGlutWindow * window)
    : mFont("*nokia*")
{
    mDisplay = window->display();
    mWindow = window->window();

    XGCValues gcv;

    gcv.fill_style = FillSolid;
    gcv.background = XWhitePixel(mDisplay, XDefaultScreen(mDisplay));
    gcv.foreground = XBlackPixel(mDisplay, XDefaultScreen(mDisplay));
    gcv.line_width = 5;
    gcv.font = mFont.id();

    int mask = GCLineWidth | GCFillStyle | GCBackground | GCForeground | GCFont;

    mGC = XCreateGC(mDisplay, mWindow, mask, &gcv);
}

RGlutGC::~RGlutGC()
{
    if (mGC) {
        XFreeGC(mDisplay, mGC);
    }
}

void RGlutGC::setForegroundColor(RGlutColor const & color)
{
    XSetForeground(mDisplay, mGC, color.pixel());
}

void RGlutGC::setBackgroundColor(RGlutColor const & color)
{
    XSetBackground(mDisplay, mGC, color.pixel());
}
    
void RGlutGC::fillRectangle(int x, int y, int w, int h)
{
    XFillRectangle(mDisplay, mWindow, mGC, x, y, w, h);
}

void RGlutGC::drawString(int x, int y, std::string const & str)
{
    XDrawString(mDisplay, mWindow, mGC, x, y,
                str.c_str(), str.length());
}

void RGlutGC::setFont(RGlutFont const & font)
{
    mFont = font;
    XSetFont(mDisplay, mGC, mFont.id());
}

