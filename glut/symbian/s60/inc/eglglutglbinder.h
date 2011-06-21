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
 
#ifndef EGLGLUTGLBINDER_H_
#define EGLGLUTGLBINDER_H_

#include "glutglbinder.h"
#include "EGL/egl.h"

class EGLGlutGLBinder: public GlutGLBinder
{
public:
    struct EGLSurfaceInfo
    {
        enum Type
        {
            TYPE_WINDOW,
            TYPE_PIXMAP,
            TYPE_PBUFFER
        };
        Type   mType;
        void * mData;
    };
    
public:
    EGLGlutGLBinder(API api);
    ~EGLGlutGLBinder();
    
    bool initialize();
    unsigned int createSurface(Surface  surface, int width, int height);
    bool makeCurrent(unsigned int surface);
    void destroySurface(unsigned int surface);
    void swapBuffer(unsigned int surface);
    void terminate();

    int getBufferSize()
    {
        return getValue(EGL_BUFFER_SIZE);
    }
    int getRedSize()
    {
        return getValue(EGL_RED_SIZE);
    }
    int getGreenSize()
    {
        return getValue(EGL_GREEN_SIZE);
    }
    int getBlueSize()
    {
        return getValue(EGL_BLUE_SIZE);
    }
    int getAlphaSize()
    {
        return getValue(EGL_ALPHA_SIZE);
    }
    int getDepthSize()
    {
        return getValue(EGL_DEPTH_SIZE);
    }
    int getStencilSize()
    {
        return getValue(EGL_STENCIL_SIZE);
    }

private:
    bool createContext();
    int getValue(int state);
    
private:
    EGLDisplay mDisplay;
    EGLContext mContext;
    EGLConfig  mConfig;
    EGLSurfaceInfo mSurfaceInfo;
    int        mRendererType;
};

#endif

