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

#include "rglutlafmanager.h"
#include "rglutlaf.h"

RGlutLAFManager::RGlutLAFManager()
{
    // default look and feel for glut
    std::string glutlaf("rglut");
    doRegisterLAF(glutlaf, new RGlutLAF);
}

RGlutLAFManager::~RGlutLAFManager()
{
    for (LAFsIter iter = mLAFs.begin(); iter != mLAFs.end(); ++iter) {
        delete iter->second;
    }
}

void RGlutLAFManager::doRegisterLAF(std::string const & name, RGlutLAF * laf)
{
    mLAFs.insert(std::pair<std::string, RGlutLAF *>(name, laf));
}

void RGlutLAFManager::doUnregisterLAF(std::string const & name)
{
    LAFsIter iter = mLAFs.find(name);
    if (iter != mLAFs.end()) {
        delete iter->second;
        mLAFs.erase(iter);
    }
}

void RGlutLAFManager::registerLAF(std::string const & name, RGlutLAF * laf)
{
    instance()->doRegisterLAF(name, laf);
}

void RGlutLAFManager::unregisterLAF(std::string const & name)
{
    instance()->doUnregisterLAF(name);
}

RGlutLAF * RGlutLAFManager::getLAF(std::string name)
{
    RGlutLAF * laf = 0;
    LAFsIter iter = instance()->mLAFs.find(name);
    if (iter != instance()->mLAFs.end()) {
        laf = iter->second;
    }

    return laf;
}

RGlutLAFManager * RGlutLAFManager::instance()
{
    static RGlutLAFManager manager;
    return &manager;
}

