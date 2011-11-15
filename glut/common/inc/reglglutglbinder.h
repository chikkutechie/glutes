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

#ifndef EGLGLUTGLBINDER_H_
#define EGLGLUTGLBINDER_H_

#include <EGL/egl.h>

#include "rcommon.h"
#include "rglutglbinder.h"
#include "reglproperties.h"

/**
 * @class REGLGlutGLBinder
 * @brief EGL functionality wrapper, and an implementation for RGlutGLBinder
 */
class REGLGlutGLBinder: public RGlutGLBinder
{
public:
    struct EGLSurfaceInfo {
        enum Type {
            TYPE_WINDOW,
            TYPE_PIXMAP,
            TYPE_PBUFFER
        };

        Type   mType;
        void *mData;
    };

public:
    REGLGlutGLBinder(API api);
    ~REGLGlutGLBinder();

    /*!
     * Sets the native display. This will be passed to eglGetDisplay.
     * @apram display display to set as native display
     */
    void setNativeDisplay(int display) {
        mNativeDisplay = display;
    }

    /*!
     * Does the EGL initialization
     */
    bool initialize();
    
    /*!
     * Creates the EGLSurface
     */
    unsigned int createSurface(Surface  surface, int width, int height);
    
    /*!
     * Makes the given surface as the current surface.
     */
    bool makeCurrent(unsigned int surface);
    
    /*!
     * Destroys the passed surface. Current surface and context will to set to null.
     */
    void destroySurface(unsigned int surface);
    
    /*!
     * Calls the eglSwapBuffers
     */
    void swapBuffer(unsigned int surface);
    
    /*!
     * Terminate the egl display, context.
     */
    void terminate();

    void addProperty(int name, int value) {
        mProperties.addProperty(name, value);
    }
    void removeProperty(int name) {
        mProperties.removeProperty(name);
    }

    void setStencil(bool enable, int size = 8) {
        if (enable) {
            addProperty(EGL_STENCIL_SIZE, size);
        } else {
            removeProperty(EGL_STENCIL_SIZE);
        }
    }

    void setDepth(bool enable, int size = 8) {
        if (enable) {
            addProperty(EGL_DEPTH_SIZE, size);
        } else {
            removeProperty(EGL_DEPTH_SIZE);
        }
    }

    void setSingleBuffer(bool enable) {
        if (enable) {
            addProperty(EGL_RENDER_BUFFER, EGL_SINGLE_BUFFER);
        } else {
            removeProperty(EGL_RENDER_BUFFER);
        }
    }

    void setDoubleBuffer(bool enable) {
        if (enable) {
            addProperty(EGL_RENDER_BUFFER, EGL_BACK_BUFFER);
        } else {
            removeProperty(EGL_RENDER_BUFFER);
        }
    }

    int getBufferSize() {
        return getValue(EGL_BUFFER_SIZE);
    }
    void setBufferSize(int size) {
        addProperty(EGL_BUFFER_SIZE, size);
    }

    int getRedSize() {
        return getValue(EGL_RED_SIZE);
    }
    void setRedSize(int size) {
        addProperty(EGL_RED_SIZE, size);
    }

    int getGreenSize() {
        return getValue(EGL_GREEN_SIZE);
    }
    void setGreenSize(int size) {
        addProperty(EGL_GREEN_SIZE, size);
    }

    int getBlueSize() {
        return getValue(EGL_BLUE_SIZE);
    }
    void setBlueSize(int size) {
        addProperty(EGL_BLUE_SIZE, size);
    }

    int getAlphaSize() {
        return getValue(EGL_ALPHA_SIZE);
    }
    void setAlphaSize(int size) {
        addProperty(EGL_ALPHA_SIZE, size);
    }

    int getDepthSize() {
        return getValue(EGL_DEPTH_SIZE);
    }
    void setDepthSize(int size) {
        addProperty(EGL_DEPTH_SIZE, size);
    }

    int getStencilSize() {
        return getValue(EGL_STENCIL_SIZE);
    }
    void setStencilSize(int size) {
        addProperty(EGL_STENCIL_SIZE, size);
    }

    int getLuminanceSize() {
        return getValue(EGL_LUMINANCE_SIZE);;
    }
    void setLuminanceSize(int size) {
        addProperty(EGL_LUMINANCE_SIZE, size);
    }

private:
    bool createContext();
    int getValue(int state) const;

private:
    int mNativeDisplay;
    EGLDisplay mDisplay;
    EGLContext mContext;
    EGLConfig  mConfig;
    EGLSurfaceInfo mSurfaceInfo;
    int mRendererType;
    REGLProperties mProperties;
};

#endif

