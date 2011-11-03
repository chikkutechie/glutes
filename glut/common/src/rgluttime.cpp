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

#include "rgluttime.h"

#if defined(WIN32)
#include <windows.h>
#include <locale.h>
#else
#if defined(__SYMBIAN32__) || defined(SYMBIAN)
#include <e32std.h>
#else
#include <time.h>
#include <sys/time.h>
#endif
#endif

static const int MSecPerSec     = 1000;
static const int MSecPerMinute  = 60 * MSecPerSec;
static const int MSecPerHour    = 60 * MSecPerMinute;

RGlutTime::RGlutTime()
    : mMS(0)
{
}

void RGlutTime::start()
{
    *this = currentTime();
}
int RGlutTime::elapsed() const
{
    RGlutTime ct = RGlutTime::currentTime();
    return (ct.mMS - mMS);           // there is a possiblity of the result to be negative
    // need to handle it.
}

#if defined(WIN32)
RGlutTime RGlutTime::currentTime()
{
    SYSTEMTIME st;
    memset(&st, 0, sizeof(SYSTEMTIME));
    GetLocalTime(&st);
    RGlutTime ct;
    ct.mMS = fromTimeComponentToMS(st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
    return ct;
}
#else
#if defined(__SYMBIAN32__) || defined(SYMBIAN)
RGlutTime RGlutTime::currentTime()
{
    RGlutTime ct;
    TTime localTime;
    localTime.HomeTime();
    TDateTime localDateTime = localTime.DateTime();
    ct.mMS = fromTimeComponentToMS((unsigned long)localDateTime.Hour(),
                                   (unsigned long)localDateTime.Minute(),
                                   (unsigned long)localDateTime.Second(),
                                   (unsigned long)(localDateTime.MicroSecond() / 1000));
    return ct;
}
#else
RGlutTime RGlutTime::currentTime()
{
    RGlutTime ct;

    struct timeval tv;
    gettimeofday(&tv, 0);
    time_t ltime = tv.tv_sec;
    struct tm *t = 0;

    tzset();
    struct tm res;
    t = localtime_r(&ltime, &res);

    if (t) {
        ct.mMS = fromTimeComponentToMS(t->tm_hour, t->tm_min, t->tm_sec, tv.tv_usec / 1000);
    }

    return ct;
}
#endif
#endif

unsigned long RGlutTime::fromTimeComponentToMS(unsigned long hour,
        unsigned long minute,
        unsigned long sec,
        unsigned long msec)
{
    return MSecPerHour * hour +
           MSecPerMinute * minute +
           MSecPerSec * sec + msec;
}

