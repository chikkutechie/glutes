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

#include <vector>

#include "rcommon.h"
#include "rglutapplication.h"
#include "rglutwindow.h"
#include "rglutdisplay.h"
#include "rgluttimer.h"
#include "rglutlafmanager.h"

namespace
{
RGlutApplication nullApp;
}
RGlutApplication *RGlutApplication::mActiveApplication = &nullApp;

RGlutApplication::RGlutApplication(RGlutWindow *window)
    : mMainWindow(window),
      mFinished(false),
      mLAF(0)
{
    mActiveApplication = this;

    Display *display = RGlutDisplay::instance()->display();
    if (display) {
        mScreenNumber = XDefaultScreen(display);
        mScreenWidth = XDisplayWidth(display, mScreenNumber);
        mScreenHeight = XDisplayHeight(display, mScreenNumber);
    }
    std::string laf("rglut");
    mLAF = RGlutLAFManager::getLAF(laf);
}

RGlutApplication::~RGlutApplication()
{
    if (mActiveApplication == this) {
        mActiveApplication = &nullApp;
    }
    delete mMainWindow;
}

RGlutLAF *RGlutApplication::setLAF(std::string const &name)
{
    mLAF = RGlutLAFManager::getLAF(name);
    if (!mLAF) {
        std::string laf("rglut");
        mLAF = RGlutLAFManager::getLAF(laf);
    }
    return mLAF;
}

void RGlutApplication::exec()
{
    mActiveApplication = this;

    Display *display = mMainWindow->display();

    while (!mFinished) {
        while (XPending(display)) {
            XEvent  xev;
            XNextEvent(display, &xev);

            GLUTES_DEBUGP2("Event Recieved %d", xev.type);

            switch (xev.type) {
            case KeyRelease:
            case KeyPress:
            case ButtonPress:
            case ButtonRelease:
            case MotionNotify: {
                case ConfigureNotify:
                    mMainWindow->handleEvent(xev);
                    break;
                }
            case Expose: {
                if (xev.xexpose.count == 0) {
                    mMainWindow->draw();
                }
                break;
            }
            }
        }

        // check for timers to execution
        // this may not be the perfect way to implement
        // but this is good enough
        checkTimers();

        // wait for some time, this might help the CPU
        usleep(1000 * EventLoopInterval);
    }
}

void RGlutApplication::registerTimer(RGlutTimer *timer)
{
    TimerEntry entry;
    entry.mTime.start();
    entry.mTimer = timer;
    mTimers.push_back(entry);
}

void RGlutApplication::unregisterTimer(RGlutTimer *timer)
{
    for (TimerSetIter iter = mTimers.begin(); iter != mTimers.end(); ++iter) {
        if (iter->mTimer == timer) {
            mTimers.erase(iter);
            break;
        }
    }
}

void RGlutApplication::checkTimers()
{
    if (mTimers.empty()) {
        return;
    }

    std::vector<TimerEntry> expiredTimers;

    for (unsigned int i = 0; i < mTimers.size();) {
        TimerEntry &te = mTimers[i];
        if (te.mTimer->interval() < te.mTime.elapsed()) {
            expiredTimers.push_back(te);
            mTimers.erase(mTimers.begin() + i);
        } else {
            ++i;
        }
    }

    int expiredTimersCount = expiredTimers.size();
    for (int i = 0; i < expiredTimersCount; ++i) {
        expiredTimers[i].mTimer->run();
    }
}

