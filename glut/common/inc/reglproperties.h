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

#ifndef EGLPROPERTIES_H_
#define EGLPROPERTIES_H_

#include <vector>
#include <EGL/egl.h>

#include "rcommon.h"

/**
 * @class REGLProperties
 * @brief Manages properties of EGL.
 */
class REGLProperties
{
private:
    class AttributePair
    {
    public:
        EGLint mName;
        EGLint mValue;

        AttributePair(EGLint n, EGLint v)
            : mName(n),
              mValue(v)
        {}
    };

    typedef  std::vector<AttributePair> Attributes;
    typedef  Attributes::iterator AttributesIter;
    typedef  Attributes::const_iterator AttributesConstIter;

public:
    enum Type {
        ConfigAttribute,
        ContextAttribute,
        SurfaceAttribute,
    };

    REGLProperties()
    {}

    /*!
     * Set the given property name and value.
     * The type of property is automatically determined.
     */
    void addProperty(int name, int value);
    /*!
     * remove the given property.
     */
    void removeProperty(int name);

    /*!
     * Retries the attribute set for the given type
     */
    EGLint *getAttributes(Type type);

private:
    void addProperty(Attributes &attributes, int name, int value);
    void removeProperty(Attributes &attributes, int name);

    Attributes *getAttributesVector(Type type);

private:
    Attributes mConfigAttributes;
    Attributes mContextAttributes;
    Attributes mSurfaceAttributes;
};

#endif

