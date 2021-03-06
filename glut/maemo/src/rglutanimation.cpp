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

#include "rglutanimation.h"
#include "rgluttimer.h"
#include "rgluttimer.h"

/**
 * @class RGlutAnimation
 *        To implement an Animation class derive from this class and implement
 *        void update(int) method.<br>
 *        For Example:
 *        @code
 *        class MyAnimation: public RGlutAnimation
 *        {
 *        public:
 *            void update(int)
 *            {
 *            // do the updation of the state here
 *            }
 *        };
 *        @endcode
 */

class RGlutAnimationTimer: public RGlutTimer
{
public:
    RGlutAnimationTimer(RGlutAnimation *animation)
        : mAnimation(animation)
    {}

    void run() {
        mAnimation->timeExpired();
    }

private:
    RGlutAnimation *mAnimation;
};

RGlutAnimation::RGlutAnimation()
    : mState(Stopped),
      mTimer(0),
      mDuration(0),
      mInterval(20),
      mListner(0)
{
    mTimer = new RGlutAnimationTimer(this);
}

RGlutAnimation::~RGlutAnimation()
{
    delete mTimer;
}

void RGlutAnimation::timeExpired()
{
    int elapsed = mTime.elapsed();
    update(elapsed);
    if (elapsed > mDuration) {
        stop();
    } else {
        mTimer->start();
    }
}

void RGlutAnimation::start()
{
    if (mState == Running) {
        return;
    }

    mTime.start();
    mTimer->setInterval(mInterval);
    mTimer->start();

    mState = Running;

    if (mListner) {
        mListner->started();
    }
}

void RGlutAnimation::stop()
{
    if (mState == Stopped) {
        return;
    }

    mTimer->stop();

    mState = Stopped;

    if (mListner) {
        mListner->stopped();
    }
    delete this;
}

