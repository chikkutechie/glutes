/*
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

#include "rglutfont.h"
#include "rglutdisplay.h"

RGlutFont::RGlutFont(std::string const & pattern)
    : mPattern(pattern),
      mFontInfo(0) 
{
    create();
}

RGlutFont::RGlutFont(RGlutFont const & oth)
{
    mPattern = oth.mPattern;
    create();
}

RGlutFont & RGlutFont::operator=(RGlutFont const & oth)
{
    if (this != &oth) {
        destroy();
        mPattern = oth.mPattern;
        create();
    }

    return *this;
}

RGlutFont::~RGlutFont()
{
    destroy();
}

XFontStruct * RGlutFont::fontStruct()
{
    return mFontInfo;
}

Font RGlutFont::id() const
{
    return mFontInfo->fid;
}

int RGlutFont::textHeight() const
{
    return mFontInfo->ascent + mFontInfo->descent;
}

void RGlutFont::fontInfoPrint() const
{
#ifdef GLUTES_DEBUG
    Display * display = RGlutDisplay::instance()->display();

    int no = 0;
    
    char ** fontPaths = XGetFontPath(display, &no);
    GLUTES_DEBUGP2("Available Font Paths Count %d", no);
    if (fontPaths) {
        for (int i=0; i<no; ++i) {
            GLUTES_DEBUGP2("%s", fontPaths[i]);
        }
        XFreeFontPath(fontPaths);
    }

    char ** fonts = XListFonts(display, "*", 1000, &no);
    GLUTES_DEBUGP2("Available Fonts Count %d", no);
    if (fonts) {
        for (int i=0; i<no; ++i) {
            GLUTES_DEBUGP2("%s", fonts[i]);
        }
        XFreeFontNames(fonts);
    }
    //char *dirs[] = {"built-ins", "/usr/share/fonts"};
    //XSetFontPath(display, dirs, sizeof(dirs)/sizeof(dirs[0]));
#endif
}

void RGlutFont::create()
{
    Display * display = RGlutDisplay::instance()->display();
    mFontInfo  = XLoadQueryFont(display, mPattern.c_str());
    if (!mFontInfo) {
        // trying to get any font, if the requested one is not available
        // this step might not be needed
        mFontInfo = XLoadQueryFont(display, "*");
    }
}

void RGlutFont::destroy()
{
    if (mFontInfo) {
        XFreeFont(RGlutDisplay::instance()->display(), mFontInfo);
    }
}

