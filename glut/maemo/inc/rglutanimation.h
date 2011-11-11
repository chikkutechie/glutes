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

#ifndef _RGLUTANIMATION_H_
#define _RGLUTANIMATION_H_

#include "rgluttime.h"

class RGlutTimer;
class RGlutAnimationTimer;

class RGlutAnimationListner
{
public:
    virtual ~RGlutAnimationListner()
    {}
    virtual void started() = 0;
    virtual void stopped() = 0;
};

/**
 * @class RGlutAnimation
 *        Base class for animation(or effects) framework.<br>
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
class RGlutAnimation
{
private:
    enum State {
        Running,
        Stopped
    };

public:
    RGlutAnimation();
    virtual ~RGlutAnimation();

    /**
     * Retrieves the duration
     * @return The animation duration
     */
    int duration() const {
        return mDuration;
    }
    /**
     * Sets the duration
     * @param d The animation duration to set.
     */
    void setDuration(int d) {
        mDuration = d;
    }
    
    
    /**
     * Retrieves the interval
     * @return The animation interval
     */
    int interval() const {
        return mInterval;
    }
    /**
     * Sets the interval
     * @param interval The animation interval to set.
     */
    void setInterval(int interval) {
        mInterval = interval;
    }

    /**
     * This method will be called to start the animation
     */
    virtual void start();
    /**
     * This method will be called to stop the animation
     */
    virtual void stop();

    /**
     * Sets the animation listner
     * @param l This listner object. Ownership is with the caller.
     */
    void setListner(RGlutAnimationListner *l) {
        mListner = l;
    }

protected:
    virtual void update(int time) = 0;
    virtual void timeExpired();

protected:
    friend class RGlutAnimationTimer;

    State mState;
    RGlutTimer *mTimer;
    int mDuration;
    RGlutTime mTime;
    int mInterval;
    RGlutAnimationListner *mListner;
};

#endif
