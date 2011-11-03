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

#include "rglutlineargradient.h"
#include "rglutdisplay.h"
#include "rglutcolor.h"

#include <X11/Xlib.h>

Pixmap RGlutLinearGradient::createPixmap(int width, int height)
{
    Display *dpy = RGlutDisplay::instance()->display();

    GC gc = XDefaultGC(dpy, XDefaultScreen(dpy));

    Pixmap pmap = XCreatePixmap(dpy, XDefaultRootWindow(dpy),
                                width, height,
                                XDefaultDepth(dpy, XDefaultScreen(dpy)));

    color *clrs = getColors(height);

    for (int y = 0; y < height; ++y) {
        RGlutColor c(clrs[y].mR, clrs[y].mG, clrs[y].mB);

        XSetForeground(dpy, gc, c.pixel());

        for (int x = 0; x < width; ++x) {
            XDrawPoint(dpy, pmap, gc, x, y);

        }
    }
    delete [] clrs;
    return pmap;
}

// this function returns the vertical linear gradient
// the first coloumn, to be precise
RGlutLinearGradient::color *RGlutLinearGradient::getColors(int h)
{
    color *c = new color[h];

    StopsIter iter2 = mStops.begin();
    StopsIter iter1 = iter2++;

    for (int i = 0; i < h; ++i) {
        float co = float(i) / float(h);
        if (co > iter2->first) {
            ++iter1;
            ++iter2;
        }
        float dy = iter2->first * h - iter1->first * h;
        float gxy = (float(i) - iter1->first * float(h)) / dy;

        color c1 = iter1->second;
        color c2 = iter2->second;

        c[i].mR = (1.0f - gxy) * c1.mR + c2.mR * gxy;
        c[i].mG = (1.0f - gxy) * c1.mG + c2.mG * gxy;
        c[i].mB = (1.0f - gxy) * c1.mB + c2.mB * gxy;
    }

    return c;
}

