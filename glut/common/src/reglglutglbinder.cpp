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

#include "reglglutglbinder.h"

REGLGlutGLBinder::REGLGlutGLBinder(RGlutGLBinder::API api)
    : RGlutGLBinder(api),
      mNativeDisplay(-1),
      mDisplay(EGL_NO_DISPLAY),
      mContext(EGL_NO_CONTEXT),
      mConfig(0)
{}

REGLGlutGLBinder::~REGLGlutGLBinder()
{
    terminate();
}

bool REGLGlutGLBinder::initialize()
{
    if (mDisplay == EGL_NO_DISPLAY) {
        if (mNativeDisplay < 0) {
            mDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
        } else {
            mDisplay = eglGetDisplay((EGLNativeDisplayType)mNativeDisplay);
        }
        if (mDisplay == EGL_NO_DISPLAY) {
            return false;
        }

        if (eglInitialize(mDisplay, 0, 0) == EGL_FALSE) {
            eglTerminate(mDisplay);
            mDisplay = 0;
            return false;
        }

        switch (getBindApi()) {
        case OPENVG_API: {
            GLUTES_DEBUGP1("Bind API is OpenVg");
            mRendererType = EGL_OPENVG_BIT;
            eglBindAPI(EGL_OPENVG_API);
            break;
        }
        case OPENGL_API: {
            GLUTES_DEBUGP1("Bind API is OpenGL");
            mRendererType = EGL_OPENGL_BIT;
            eglBindAPI(EGL_OPENGL_API);
            break;
        }
        case OPENGL_ES_API: {
            GLUTES_DEBUGP1("Bind API is OpenGL/ES");
            mRendererType = EGL_OPENGL_ES_BIT;
            eglBindAPI(EGL_OPENGL_ES_API);
            addProperty(EGL_CONTEXT_CLIENT_VERSION, 1);
            break;
        }
        case OPENGL_ES2_API: {
            GLUTES_DEBUGP1("Bind API is OpenGL/ES2");
            mRendererType = EGL_OPENGL_ES2_BIT;
            eglBindAPI(EGL_OPENGL_ES_API);
            addProperty(EGL_CONTEXT_CLIENT_VERSION, 2);
            break;
        }
        }
    }

    addProperty(EGL_RENDERABLE_TYPE, mRendererType);

    return true;
}

void REGLGlutGLBinder::terminate()
{
    if (mDisplay != EGL_NO_DISPLAY) {
        eglMakeCurrent(mDisplay, EGL_NO_SURFACE,
                       EGL_NO_SURFACE, EGL_NO_CONTEXT);

        if (mContext != EGL_NO_CONTEXT) {
            eglDestroyContext(mDisplay, mContext);
            mContext = EGL_NO_CONTEXT;
        }

        eglTerminate(mDisplay);
        mDisplay = EGL_NO_DISPLAY;
    }
}

bool REGLGlutGLBinder::createContext()
{
    if (mDisplay == EGL_NO_DISPLAY) {
        if (!initialize()) {
            GLUTES_DEBUGP1("EGL Initialization Failed");
            return false;
        }
    }

    if (mContext == EGL_NO_CONTEXT) {
        EGLint numConfigs = 0;

        EGLint *attr =
            mProperties.getAttributes(REGLProperties::ConfigAttribute);

        if (eglChooseConfig(mDisplay,
                            attr,
                            &mConfig, 1,
                            &numConfigs) == EGL_FALSE) {
            GLUTES_DEBUGP2("eglChooseConfig Failed 0x%x", eglGetError());
            return false;
        }
        attr = mProperties.getAttributes(REGLProperties::ContextAttribute);

        mContext = eglCreateContext(mDisplay,
                                    mConfig,
                                    EGL_NO_CONTEXT,
                                    attr);
        if (mContext == EGL_NO_CONTEXT) {
            GLUTES_DEBUGP2("eglCreateContext Failed 0x%x", eglGetError());
        }
    }

    return (mContext != EGL_NO_CONTEXT);
}

unsigned int  REGLGlutGLBinder::createSurface(Surface surfaceParam,
        int width, int height)
{
    EGLSurface surface = EGL_NO_SURFACE;

    eglMakeCurrent(mDisplay, EGL_NO_SURFACE,
                   EGL_NO_SURFACE, EGL_NO_CONTEXT);

    mSurfaceInfo = *((EGLSurfaceInfo *) surfaceParam);
    switch (mSurfaceInfo.mType) {
    case EGLSurfaceInfo::TYPE_WINDOW: {
        if (mSurfaceInfo.mData) {
            GLUTES_DEBUGP1("Window Surface");
            addProperty(EGL_SURFACE_TYPE, EGL_WINDOW_BIT);

            if (!createContext()) {
                GLUTES_DEBUGP1("EGL Context Creation Failed");
                return (unsigned int)EGL_NO_SURFACE;
            }

            EGLint *attr =
                mProperties.getAttributes(REGLProperties::SurfaceAttribute);

            surface = eglCreateWindowSurface(mDisplay, mConfig,
                                             (EGLNativeWindowType)mSurfaceInfo.mData,
                                             attr);
            if (surface == EGL_NO_SURFACE) {
                GLUTES_DEBUGP2("eglCreateSurface Failed 0x%x", eglGetError());
            }
        }
        break;
    }

    case EGLSurfaceInfo::TYPE_PBUFFER: {
        GLUTES_DEBUGP1("PBuffer Surface");
        if (width > 0 && height > 0) {
            addProperty(EGL_SURFACE_TYPE, EGL_PBUFFER_BIT |
                        EGL_VG_ALPHA_FORMAT_PRE_BIT);

            if (!createContext()) {
                return (unsigned int)EGL_NO_SURFACE;
            }

            addProperty(EGL_WIDTH, width);
            addProperty(EGL_HEIGHT, height);

            EGLint *attr =
                mProperties.getAttributes(REGLProperties::SurfaceAttribute);

            surface = eglCreatePbufferSurface(mDisplay,
                                              mConfig,
                                              attr);
        }
        break;
    }

    case EGLSurfaceInfo::TYPE_PIXMAP: {
        GLUTES_DEBUGP1("Pixmap Surface");
        if (mSurfaceInfo.mData) {
            addProperty(EGL_SURFACE_TYPE, EGL_PIXMAP_BIT |
                        EGL_VG_ALPHA_FORMAT_PRE_BIT);
            addProperty(EGL_MATCH_NATIVE_PIXMAP,
                        (EGLint)mSurfaceInfo.mData);

            if (!createContext()) {
                return (unsigned int)EGL_NO_SURFACE;
            }

            EGLint *attr =
                mProperties.getAttributes(REGLProperties::SurfaceAttribute);

            surface = eglCreatePixmapSurface(mDisplay, mConfig,
                                             (EGLNativePixmapType)mSurfaceInfo.mData,
                                             attr);
        }
        break;
    }
    }
    return (unsigned int)surface;
}

bool REGLGlutGLBinder::makeCurrent(unsigned int surface)
{
    return eglMakeCurrent(mDisplay,
                          (EGLSurface)surface,
                          (EGLSurface)surface,
                          mContext) == EGL_TRUE;
}

void REGLGlutGLBinder::destroySurface(unsigned int surface)
{
    eglMakeCurrent(mDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    eglDestroySurface(mDisplay, (EGLSurface)surface);
}

void REGLGlutGLBinder::swapBuffer(unsigned int surface)
{
    if (mDisplay != EGL_NO_DISPLAY && (EGLSurface)surface != EGL_NO_SURFACE) {
        eglSwapBuffers(mDisplay, (EGLSurface)surface);
    }
}

int REGLGlutGLBinder::getValue(int state) const
{
    EGLint value = 0;

    if (mDisplay != EGL_NO_DISPLAY && mConfig) {
        eglGetConfigAttrib(mDisplay, mConfig, state, &value);
    }

    return value;
}
