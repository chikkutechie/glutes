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

#ifndef GLUTGLBINDER_H_
#define GLUTGLBINDER_H_


class GlutGLBinder
{
public:
    enum API
    {
        OPENGL_ES_API,
        OPENGL_ES2_API,
        OPENVG_API,
        OPENGL_API
    };
    
    typedef int * Surface;
    
public:
    GlutGLBinder(API a = OPENGL_ES_API) : mAPI(a) {}
    virtual ~GlutGLBinder() {}
    
    virtual bool initialize() = 0;
    virtual unsigned int createSurface(Surface surface, int width, int height) = 0;
    virtual bool makeCurrent(unsigned int surface) = 0;
    virtual void destroySurface(unsigned int surface) = 0;
    virtual void swapBuffer(unsigned int surface) = 0;
    virtual void terminate() = 0;
    
    virtual void addProperty(int, int) {}
    virtual void removeProperty(int) {}
    
    virtual void setStencil(bool enable, int size = 8) = 0;
    virtual void setDepth(bool enable, int size = 8) = 0;
    virtual void setSingleBuffer(bool enable) = 0;
    virtual void setDoubleBuffer(bool enable) = 0;
    
    void setBindApi(API api)
    {
        this->mAPI = api;
    }
    
    API getBindApi() const
    {
        return mAPI;
    }

    virtual int getBufferSize()
    {
        return 0;
    }
    virtual void setBufferSize(int)
    {}    

    virtual int getRedSize()
    {
        return 0;
    }
    virtual void setRedSize(int)
    {}

    virtual int getGreenSize()
    {
        return 0;
    }
    virtual void setGreenSize(int)
    {}

    virtual int getBlueSize()
    {
        return 0;
    }
    virtual void setBlueSize(int)
    {}

    virtual int getAlphaSize()
    {
        return 0;
    }
    virtual void setAlphaSize(int)
    {}

    virtual int getDepthSize()
    {
        return 0;
    }
    virtual void setDepthSize(int)
    {}

    virtual int getStencilSize()
    {
        return 0;
    }
    virtual void setStencilSize(int)
    {}

    virtual int getLuminanceSize()
    {
        return 0;
    }
    virtual void setLuminanceSize(int)
    {}
    
protected:
    API mAPI;
};

#endif
