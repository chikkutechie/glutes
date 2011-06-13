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
 
#include "eglglutglbinder.h"
#include <w32std.h>

EGLGlutGLBinder::EGLGlutGLBinder(GlutGLBinder::API api)
  : GlutGLBinder(api),
    mDisplay(EGL_NO_DISPLAY),
    mContext(EGL_NO_CONTEXT),
    mConfig(0)
{
    switch (api) {
        case OPENGL_API:
            mRendererType = EGL_OPENGL_BIT;
            break;
        case OPENGL_ES2_API:
            mRendererType = EGL_OPENGL_ES2_BIT;
            break;
        case OPENVG_API:
            mRendererType = EGL_OPENVG_BIT;
            break;
        case OPENGL_ES_API:
        default:
            mRendererType = EGL_OPENGL_ES_BIT;
            break;            
    }
}

EGLGlutGLBinder::~EGLGlutGLBinder()
{
    terminate();
}

bool EGLGlutGLBinder::initialize()
{
    if (mDisplay == EGL_NO_DISPLAY) {
        mDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
        if (mDisplay == EGL_NO_DISPLAY) {
            return false;
        }

        if (eglInitialize(mDisplay, 0, 0) == EGL_FALSE) {
            eglTerminate(mDisplay);
            mDisplay = 0;
            return false;
        }

        if (getBindApi() == OPENVG_API) {
            eglBindAPI(EGL_OPENVG_API);
        } else if (getBindApi() == OPENGL_API) {
            eglBindAPI(EGL_OPENGL_API);
        } else if (getBindApi() == OPENGL_ES_API || getBindApi() == OPENGL_ES2_API) {
            eglBindAPI(EGL_OPENGL_ES_API);
        }
    }
    
    return createContext();
}

void EGLGlutGLBinder::terminate()
{
    if (mDisplay != EGL_NO_DISPLAY) {
        eglMakeCurrent(mDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        
        if (mContext != EGL_NO_CONTEXT) {
            eglDestroyContext(mDisplay, mContext);
            mContext = EGL_NO_CONTEXT;
        }
                
        eglTerminate(mDisplay);
        mDisplay = EGL_NO_DISPLAY;
    }
}

bool EGLGlutGLBinder::createContext()
{
    if (mDisplay == EGL_NO_DISPLAY) {
        return false;
    }
    if (mContext == EGL_NO_CONTEXT) {
        
        const EGLint KConfigAttributes[] =
            {
            EGL_SURFACE_TYPE,    EGL_WINDOW_BIT |
            EGL_VG_ALPHA_FORMAT_PRE_BIT,
            EGL_RENDERABLE_TYPE, mRendererType,
            EGL_RED_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_BLUE_SIZE, 8,
            EGL_DEPTH_SIZE, 8,
            EGL_NONE
            };
    
        EGLint numConfigs = 0;
        if (eglChooseConfig(mDisplay, KConfigAttributes, &mConfig, 1, &numConfigs) == EGL_FALSE) {
            return false;
        }
        
        EGLint * ContextAttribList = 0;
        EGLint KContextAttribList1[] =
                    {
                    EGL_CONTEXT_CLIENT_VERSION, 1,
                    EGL_NONE
                    };
        EGLint KContextAttribList2[] =
                    {
                    EGL_CONTEXT_CLIENT_VERSION, 2,
                    EGL_NONE
                    };
        
        if (mAPI == OPENGL_ES_API) {
            ContextAttribList = &KContextAttribList1[0];
        } else if (mAPI == OPENGL_ES2_API) {
            ContextAttribList = &KContextAttribList2[0];            
        }

        mContext = eglCreateContext(mDisplay, mConfig, EGL_NO_CONTEXT, ContextAttribList);
        if ( mContext == EGL_NO_CONTEXT ) {
            return false;
        }
    }
    
    return (mContext != EGL_NO_CONTEXT);
}

unsigned int  EGLGlutGLBinder::createSurface(Surface surfaceParam, int, int)
{
    if (mContext == EGL_NO_CONTEXT) {
        return 0;
    }
    
    EGLSurface surface = EGL_NO_SURFACE;
    
    eglMakeCurrent(mDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    if (surface != EGL_NO_SURFACE) {
        eglDestroySurface(mDisplay, surface);
        surface = EGL_NO_SURFACE;
    }
    
    mSurfaceInfo = *((EGLSurfaceInfo*) surfaceParam);
    switch (mSurfaceInfo.mType) {
        case EGLSurfaceInfo::TYPE_WINDOW: {        
        RWindow * win= (RWindow *)mSurfaceInfo.mData;
        surface = eglCreateWindowSurface(mDisplay, mConfig, win, 0);
        break;
        }
        
        case EGLSurfaceInfo::TYPE_PBUFFER: {
        break;
        }
        
        case EGLSurfaceInfo::TYPE_PIXMAP: {
        break;
        }
    }
    
    return (unsigned int)surface;
}

bool EGLGlutGLBinder::makeCurrent(unsigned int surface)
{
    return eglMakeCurrent(mDisplay,
                          (EGLSurface)surface,
                          (EGLSurface)surface,
                          mContext) == EGL_TRUE;
}

void EGLGlutGLBinder::destroySurface(unsigned int surface)
{
    eglMakeCurrent(mDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    eglDestroySurface(mDisplay, (EGLSurface)surface);
}

void EGLGlutGLBinder::swapBuffer(unsigned int surface)
{
    if (mDisplay != EGL_NO_DISPLAY && (EGLSurface)surface != EGL_NO_SURFACE) {
        eglSwapBuffers(mDisplay, (EGLSurface)surface);
    }
}
