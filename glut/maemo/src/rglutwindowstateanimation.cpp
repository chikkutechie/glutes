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

#include "rglutwindowstateanimation.h"

RGlutWindowStateAnimation::RGlutWindowStateAnimation(RGlutWindow * window)
    : mWindow(window),
      mWindowState(Position),
      mWindowRectStart(0, 0, 0, 0),
      mWindowRectEnd(0, 0, 0, 0)
{}

inline int RGlutWindowStateAnimation::interpolate(int start, int end, float t)
{
    if (t < 0.0f) {
        t = 0.0f;
    }
    if (t > 1.0f) {
        t = 1.0f;
    }
    float et = mEasingCurve.eval(t);
    return end * et - start * et + start;
}

void RGlutWindowStateAnimation::update(int time)
{
    float t = (float)time / (float)duration();

    switch (mWindowState) {
        case Position: {
            int x = interpolate(mWindowRectStart.x1(), mWindowRectEnd.x1(), t);
            int y = interpolate(mWindowRectStart.y1(), mWindowRectEnd.y1(), t);
            mWindow->setPos(x, y);
            break;
        }

        case Size: {
            int w = interpolate(mWindowRectStart.width(), mWindowRectEnd.width(), t);
            int h = interpolate(mWindowRectStart.height(), mWindowRectEnd.height(), t);
            mWindow->setSize(w, h);
            break;
        }

        case PositionAndSize: {
            int x = interpolate(mWindowRectStart.x1(), mWindowRectEnd.x1(), t);
            int y = interpolate(mWindowRectStart.y1(), mWindowRectEnd.y1(), t);
            int w = interpolate(mWindowRectStart.width(), mWindowRectEnd.width(), t);
            int h = interpolate(mWindowRectStart.height(), mWindowRectEnd.height(), t);
            mWindow->setGeometry(x, y, w, h);
            break;
        }
    }
}

