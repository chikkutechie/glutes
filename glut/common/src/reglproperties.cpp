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

#include "reglproperties.h"

REGLProperties::Attributes * REGLProperties::getAttributesVector(Type type)
{
    Attributes * attr = 0;
    
    switch (type) {
        case ConfigAttribute:
            attr = &mConfigAttributes;
            break;
        case ContextAttribute:
            attr = &mContextAttributes;
            break;
        case SurfaceAttribute:
            attr = &mSurfaceAttributes;
            break;
    }
    
    return attr;
}

EGLint * REGLProperties::getAttributes(Type type)
{
    Attributes * attributes = getAttributesVector(type);
    if (!attributes) {
        return 0;
    }

    EGLint * attr = 0;

    if (!attributes->empty()) {

        bool nonepresent = false;

        for (AttributesIter iter = attributes->begin();
                iter != attributes->end(); ++iter) {
            if (iter->mName == EGL_NONE) {
                nonepresent = true;
                break;
            }
        }

        if (!nonepresent) {
            attributes->push_back(AttributePair(EGL_NONE, 0));
        }

        attr = (EGLint *)(&((*attributes)[0]));
    }

    return attr;
}

void REGLProperties::addProperty(EGLint name, EGLint value)
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

void REGLProperties::removeProperty(int name)
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

void REGLProperties::addProperty(Attributes & attributes,
                                int name, int value)
{
    int update = false;

    for (AttributesIter iter = attributes.begin();
            iter != attributes.end(); ++iter) {

        if (iter->mName == name) {
            update = true;

            switch (iter->mName) {
                case EGL_SURFACE_TYPE: {
                    iter->mValue |= value;
                    break;
                }

                default: {
                    iter->mValue = value;
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

void REGLProperties::removeProperty(Attributes & attributes, int name)
{
    for (AttributesIter iter = attributes.begin();
            iter != attributes.end(); ++iter) {
        if (iter->mName == name) {
            attributes.erase(iter);
            break;
        }
    }
}

