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
 * @brief Basic drawing function implementation.
 */
class RGlutGC
{
public:
    RGlutGC(RGlutWindow *window);
    ~RGlutGC();

    /**
     * Sets the foreground color
     * @see RGlutColor
     */
    void setForegroundColor(RGlutColor const &color);
    /**
     * Sets the background color
     * @see RGlutColor
     */
    void setBackgroundColor(RGlutColor const &color);

    /**
     * Fill the given area.
     * @param x x position
     * @param y y position
     * @param width width of the rectangle
     * @param height height of the rectangle
     */
    void fillRectangle(int x, int y, int w, int h);

    /**
     * Sets the font.
     * @see RGlutFont
     * @param font The font object to set
     */
    void setFont(RGlutFont const &font);
    /**
     * Retrieves the font.
     * @see RGlutFont
     */
    RGlutFont font() const {
        return mFont;
    }

    /**
     * Retrieves the width that will be used for the given string.
     * @param str The string for which the width should be found.
     */
    int textWidth(std::string const &str);
    /**
     * Draws the given string.
     * @param x x position
     * @param y y position
     * @param str The string which should be drawn
     */
    void drawString(int x, int y, std::string const &str);

    /**
     * Draws the given pixmap.
     * @param pmap The Pixmap which should be drawn
     * @param sx source x position(pixmap x position)
     * @param sy source y position(pixmap y position)
     * @param width The width that is to draw from Pixmap
     * @param height The height that is to draw from Pixmap 
     * @param dx destination x position(window x position)
     * @param dy destination y position(window y position)
     */
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

