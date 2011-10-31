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

#ifndef _RGLUTWINDOWSTATEANIMATION_H_
#define _RGLUTWINDOWSTATEANIMATION_H_

#include "rglutanimation.h"
#include "rglutwindow.h"
#include "rgluteasingcurve.h"

/**
 * @class RGlutWindowStateAnimation
 *        Animation implementation for window properties
 */
class RGlutWindowStateAnimation: public RGlutAnimation
{
public:
    enum WindowState
    {
        Position,
        Size,
        PositionAndSize
    };

    RGlutWindowStateAnimation(RGlutWindow * window);

    void setWindowState(WindowState state)
    {
        mWindowState = state;
    }

    void setStartPos(RGlutPointI point)
    {
        mWindowRectStart.setX1(point.x());
        mWindowRectStart.setY1(point.y());
    }
    void setEndPos(RGlutPointI point)
    {
        mWindowRectEnd.setX1(point.x());
        mWindowRectEnd.setY1(point.y());
    }

    void setStartSize(RGlutSizeI size)
    {
        mWindowRectStart.setWidth(size.width());
        mWindowRectStart.setHeight(size.height());
    }
    void setEndSize(RGlutSizeI size)
    {
        mWindowRectEnd.setWidth(size.width());
        mWindowRectEnd.setHeight(size.height());
    }

    void setStartRect(RGlutRectI rect)
    {
        mWindowRectStart = rect;
    }
    void setEndRect(RGlutRectI rect)
    {
        mWindowRectEnd = rect;
    }

    void setEasingCurve(RGlutEasingCurve::Type type)
    {
        mEasingCurve.setType(type);
    }

private:
    void update(int time);
    int interpolate(int start, int end, float t);

private:
    RGlutWindow * mWindow;
    WindowState mWindowState;
    RGlutRectI mWindowRectStart;
    RGlutRectI mWindowRectEnd;
    RGlutEasingCurve mEasingCurve;
};

#endif


