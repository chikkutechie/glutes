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

#include "rglutlaf.h"
#include "rglutlineargradient.h"

RGlutLAF::RGlutLAF()
{} 

RGlutLAF::~RGlutLAF()
{} 

RGlutFont RGlutLAF::defaultFont()
{
    return RGlutFont("*Nokia*Sans*");
}

RGlutFont RGlutLAF::menuItemFont()
{
    return RGlutFont("*Nokia*Sans*");
}

int RGlutLAF::maximumMenuWidth()
{
    return 400;
}

int RGlutLAF::maximumMenuItemHeight()
{
    return 200;
}

int RGlutLAF::minimumMenuWidth()
{
    return 200;
}

int RGlutLAF::minimumMenuItemHeight()
{
    return 75;
}

int RGlutLAF::menuItemGap()
{
    return 2;
}

RGlutColor RGlutLAF::getMenuBackgroundColor()
{
    return RGlutColor(167, 211, 162);
} 

RGlutColor RGlutLAF::getMenuTextColor()
{
    return RGlutColor(10, 10, 10);
}

Pixmap RGlutLAF::createMenuItemBackgroundNormal(int w, int h)
{
    RGlutLinearGradient normalGrad;
    RGlutGradient::color c;

    c.mR = 167;
    c.mG = 211;
    c.mB = 162;
    normalGrad.addStop(0.0f, c);
    c.mR = 0;
    c.mG = 180;
    c.mB = 0;
    normalGrad.addStop(0.4f, c);
    c.mR = 0;
    c.mG = 180;
    c.mB = 0;
    normalGrad.addStop(0.6f, c);
    c.mR = 0;
    c.mG = 250;
    c.mB = 0;
    normalGrad.addStop(1.0f, c);

    return normalGrad.createPixmap(w, h);
} 

Pixmap RGlutLAF::createMenuItemBackgroundPressed(int w, int h)
{
    RGlutGradient::color c;
    RGlutLinearGradient pressedGrad;

    c.mR = 67;
    c.mG = 111;
    c.mB = 62;
    pressedGrad.addStop(0.0f, c);
    c.mR = 0;
    c.mG = 80;
    c.mB = 0;
    pressedGrad.addStop(0.4f, c);
    c.mR = 0;
    c.mG = 80;
    c.mB = 0;
    pressedGrad.addStop(0.6f, c);
    c.mR = 0;
    c.mG = 150;
    c.mB = 0;
    pressedGrad.addStop(1.0f, c);

    return pressedGrad.createPixmap(w, h);
} 

