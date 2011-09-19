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

Font RGlutFont::id()
{
    return mFontInfo->fid;
}

int RGlutFont::textHeight()
{
    return mFontInfo->ascent + mFontInfo->descent;
}

void RGlutFont::create()
{
    mFontInfo  = XLoadQueryFont(RGlutDisplay::instance()->display(), mPattern.c_str());
}

void RGlutFont::destroy()
{
    if (mFontInfo) {
        XFreeFont(RGlutDisplay::instance()->display(), mFontInfo);
    }
}

