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
    mConfig(0),
    mLastError(0)
{}

EGLGlutGLBinder::~EGLGlutGLBinder()
{
    terminate();
}

bool EGLGlutGLBinder::initialize()
{
    if (mDisplay == EGL_NO_DISPLAY) {
        mDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
        if (mDisplay == EGL_NO_DISPLAY) {
            mLastError = eglGetError();
            return false;
        }

        if (eglInitialize(mDisplay, 0, 0) == EGL_FALSE) {
            eglTerminate(mDisplay);
            mDisplay = 0;
            mLastError = eglGetError();
            return false;
        }

        switch (getBindApi()) {
            case OPENVG_API: {
                mRendererType = EGL_OPENVG_BIT;
                eglBindAPI(EGL_OPENVG_API);
                break;
            } 
            case OPENGL_API: {
                mRendererType = EGL_OPENGL_BIT;
                eglBindAPI(EGL_OPENGL_API);
                break;
            } 
            case OPENGL_ES_API: {
                mRendererType = EGL_OPENGL_ES_BIT;
                eglBindAPI(EGL_OPENGL_ES_API);
                addProperty(EGL_CONTEXT_CLIENT_VERSION, 1);
                break;
            } 
            case OPENGL_ES2_API: {
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

void EGLGlutGLBinder::terminate()
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

bool EGLGlutGLBinder::createContext()
{
    if (mDisplay == EGL_NO_DISPLAY) {
        if (!initialize()) {
            return false;
        }
    }
    
    if (mContext == EGL_NO_CONTEXT) {
        EGLint numConfigs = 0;
        
        if (eglChooseConfig(mDisplay,
                            getAttributes(mConfigAttributes),
                            &mConfig, 1,
                            &numConfigs) == EGL_FALSE) {
            mLastError = eglGetError();
            return false;
        }
        mContext = eglCreateContext(mDisplay, mConfig,
                                    EGL_NO_CONTEXT,
                                    getAttributes(mContextAttributes));
        if (mContext == EGL_NO_CONTEXT) {
            mLastError = eglGetError();
        }
    }
    
    return (mContext != EGL_NO_CONTEXT);
}

unsigned int  EGLGlutGLBinder::createSurface(Surface surfaceParam, int width, int height)
{
    EGLSurface surface = EGL_NO_SURFACE;
    
    eglMakeCurrent(mDisplay, EGL_NO_SURFACE,
                   EGL_NO_SURFACE, EGL_NO_CONTEXT);
    
    mSurfaceInfo = *((EGLSurfaceInfo*) surfaceParam);
    switch (mSurfaceInfo.mType) {
        case EGLSurfaceInfo::TYPE_WINDOW: {
            if (mSurfaceInfo.mData) {
                addProperty(EGL_SURFACE_TYPE, EGL_WINDOW_BIT |
                                              EGL_VG_ALPHA_FORMAT_PRE_BIT);
    
                if (!createContext()) {
                    return EGL_NO_SURFACE;
                }
    
                RWindow * win= (RWindow *)mSurfaceInfo.mData;
                surface = eglCreateWindowSurface(mDisplay, mConfig, win,
                                            getAttributes(mSurfaceAttributes));
            }
            break;
        }

        case EGLSurfaceInfo::TYPE_PBUFFER: {
            if (width > 0 && height > 0) {
                addProperty(EGL_SURFACE_TYPE, EGL_PBUFFER_BIT |
                                              EGL_VG_ALPHA_FORMAT_PRE_BIT);
    
                if (!createContext()) {
                    return EGL_NO_SURFACE;
                }
    
                addProperty(EGL_WIDTH, width);
                addProperty(EGL_WIDTH, height);
    
                surface = eglCreatePbufferSurface(mDisplay, mConfig,
                                            getAttributes(mSurfaceAttributes));
            }
            break;
        }

        case EGLSurfaceInfo::TYPE_PIXMAP: {
            if (mSurfaceInfo.mData) {
                addProperty(EGL_SURFACE_TYPE, EGL_PIXMAP_BIT |
                                              EGL_VG_ALPHA_FORMAT_PRE_BIT);
                addProperty(EGL_MATCH_NATIVE_PIXMAP, (EGLint)mSurfaceInfo.mData);
    
                if (!createContext()) {
                    return EGL_NO_SURFACE;
                }
    
                surface = eglCreatePixmapSurface(mDisplay, mConfig,
                                                        mSurfaceInfo.mData, 0);
            }
            break;
        }
    }
    mLastError = eglGetError();
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

int EGLGlutGLBinder::getValue(int state) const
{
    EGLint value = 0;
    
    if (mDisplay != EGL_NO_DISPLAY && mConfig) {
        eglGetConfigAttrib(mDisplay, mConfig, state, &value);
    }
    
    return value;
}

EGLint * EGLGlutGLBinder::getAttributes(Attributes & attributes)
{
    EGLint * attr = 0;
    
    if (!attributes.empty()) {

        bool nonepresent = false;
        
        for (AttributesIter iter = attributes.begin();
                iter != attributes.end(); ++iter) {
            if (iter->first == EGL_NONE) {
                nonepresent = true;
                break;
            }
        }
        
        if (!nonepresent) {
            attributes.push_back(AttributePair(EGL_NONE, 0));
        }
    
        attr = (EGLint *)&attributes[0];
    }

    return attr;
}

void EGLGlutGLBinder::addProperty(EGLint name, EGLint value)
{
    switch (name) {
        case EGL_CONTEXT_CLIENT_VERSION:
        case EGL_CONTEXT_CLIENT_TYPE:
            addProperty(mContextAttributes, name, value);
            break;

        case EGL_WIDTH:
        case EGL_HEIGHT:
        case EGL_LARGEST_PBUFFER:
        case EGL_TEXTURE_FORMAT:
        case EGL_MIPMAP_TEXTURE:
        case EGL_RENDER_BUFFER:
        case EGL_VG_COLORSPACE:
        case EGL_VG_ALPHA_FORMAT:
            addProperty(mSurfaceAttributes, name, value);
            break;

        default:
            addProperty(mConfigAttributes, name, value);
            break;
    }
}

void EGLGlutGLBinder::removeProperty(int name)
{
    switch (name) {
        case EGL_CONTEXT_CLIENT_VERSION:
        case EGL_CONTEXT_CLIENT_TYPE:
            removeProperty(mContextAttributes, name);
            break;

        case EGL_WIDTH:
        case EGL_HEIGHT:
        case EGL_LARGEST_PBUFFER:
        case EGL_TEXTURE_FORMAT:
        case EGL_MIPMAP_TEXTURE:
        case EGL_RENDER_BUFFER:
        case EGL_VG_COLORSPACE:
        case EGL_VG_ALPHA_FORMAT:
            removeProperty(mSurfaceAttributes, name);
            break;

        default:
            removeProperty(mConfigAttributes, name);
            break;
    }
}

void EGLGlutGLBinder::addProperty(Attributes & attributes, int name, int value)
{
    int update = false;
    
    for (AttributesIter iter = attributes.begin();
            iter != attributes.end(); ++iter) {
        
        if (iter->first == name) {
            update = true;
            
            switch (iter->first) {
                case EGL_SURFACE_TYPE: {
                    iter->second |= value;
                    break;
                }
                
                default: {
                    iter->second = value;
                    break;
                }
            }

            break;
        }
    }
    
    if (!update) {
        attributes.insert(attributes.begin(), AttributePair(name, value));
    }
}

void EGLGlutGLBinder::removeProperty(Attributes & attributes, int name)
{
    for (AttributesIter iter = attributes.begin();
            iter != attributes.end(); ++iter) {
        if (iter->first == name) {
            attributes.erase(iter);
            break;
        }
    }
}

