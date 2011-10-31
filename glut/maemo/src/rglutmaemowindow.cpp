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

#include "rglutmaemowindow.h"
#include "rcommon.h"

RGlutMaemoWindow::RGlutMaemoWindow(RGlutEventHandler * eh)
    : mEH(eh)
{}

RGlutMaemoWindow::~RGlutMaemoWindow()
{}

void RGlutMaemoWindow::draw()
{
    if (mEH) {
        mEH->draw();
    }
    RGlutWindow::draw();
}

bool RGlutMaemoWindow::handleEvent(XEvent & xev)
{
    if (RGlutWindow::handleEvent(xev) || xev.xany.window != mWindow) {
        return false;
    }

    mCurrentEvent = &xev;
    switch (xev.type) {
        //case KeyRelease:
        case KeyPress: {
            if (mEH) {
                char asciiTable[64];
                XComposeStatus cs;
                KeySym keySym;
                int length;

                length = XLookupString(&xev.xkey, asciiTable, sizeof(asciiTable), &keySym, &cs);
                if (length > 0) {
                    mEH->keyboard(asciiTable[0], xev.xkey.state, xev.xkey.x, xev.xkey.y);
                }
            }
            break;
        }

        case ButtonPress:
        case ButtonRelease: {
            if (mEH) {
                bool pressed = false;

                if (xev.type == ButtonPress) {
                    pressed = true;
                }

                mEH->mouse(xev.xbutton.button, xev.xbutton.state, xev.xbutton.x, xev.xbutton.y, pressed);
            }
            break;
        }

        case MotionNotify: {
            if (mEH) {
                mEH->motion(0, xev.xmotion.state, xev.xmotion.x, xev.xmotion.y);
            }
            break;
        }

        case ConfigureNotify: {
            if (mEH) {
                if (mWindow == xev.xconfigure.window) {
                    mEH->reshape(xev.xconfigure.width, xev.xconfigure.height);
                }
            }
        }
        break;
    }
    mCurrentEvent = 0;
    return true;
}

