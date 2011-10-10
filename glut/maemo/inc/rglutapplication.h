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

#ifndef _RGLUTAPPLICATION_H_
#define _RGLUTAPPLICATION_H_

#include <vector>

#include "rgluttime.h"

class RGlutWindow;
class RGlutTimer;
class RGlutLAF;

class RGlutApplication
{
public:
    RGlutApplication(RGlutWindow * window = 0);
    virtual ~RGlutApplication();
    
    void setMainWindow(RGlutWindow * window)
    {
        mMainWindow = window;
    }

    RGlutWindow * mainWindow()
    {
        return mMainWindow;
    }
    
    RGlutLAF * LAF()
    {
        return mLAF;
    }

    RGlutLAF * setLAF(std::string const & name);

    int screenWidth() const
    {
        return mScreenWidth;
    }
    int screenHeight() const
    {
        return mScreenHeight;
    }
    int screen() const
    {
        return mScreenNumber;
    }
    
    void exec();
    
    static RGlutApplication * activeApplication()
    {
        return mActiveApplication;
    }
    
    void registerTimer(RGlutTimer * timer);
    void unregisterTimer(RGlutTimer * timer);
    
private:
    void checkTimers();
    
protected:
    static const int EventLoopInterval = 1;
    static RGlutApplication * mActiveApplication;

    RGlutWindow * mMainWindow;
    bool mFinished;

    RGlutLAF * mLAF;

    int mScreenNumber;
    int mScreenWidth;
    int mScreenHeight;
    
    class TimerEntry
    {
    public:
        RGlutTime mTime;    
        RGlutTimer * mTimer;
    };
    
    typedef std::vector<TimerEntry> TimerSet;
    typedef TimerSet::iterator TimerSetIter;
    
    TimerSet mTimers;
};

#endif
