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

#ifndef _RGLUTGC_H_
#define _RGLUTGC_H_

#include <string>

#include "rglutfont.h"
#include "rglutcolor.h"

class RGlutWindow;

/**
 * @class RGlutGC
 *        Basic GC implementation
 */
class RGlutGC
{
public:
    RGlutGC(RGlutWindow *window);
    ~RGlutGC();

    void setForegroundColor(RGlutColor const &color);
    void setBackgroundColor(RGlutColor const &color);

    void fillRectangle(int x, int y, int w, int h);

    void setFont(RGlutFont const &font);
    RGlutFont font() const {
        return mFont;
    }

    int textWidth(std::string const &str);
    void drawString(int x, int y, std::string const &str);

    void drawPixmap(Pixmap pmap, int sx, int sy, int width, int height, int dx, int dy);

private:
    RGlutGC(RGlutGC const &);
    RGlutGC &operator=(RGlutGC const &);

private:
    GC mGC;
    RGlutFont mFont;
    Window mWindow;
    Display *mDisplay;
};

#endif

