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

#ifndef _RGLUTSTYLEMANAGER_H_
#define _RGLUTSTYLEMANAGER_H_

#include <map>
#include <string>

#include "rglutlaf.h"

/**
 * @class RGlutLAFManager
 *        Look and feel container
 */
class RGlutLAFManager
{
public:
    static void registerLAF(std::string const &name, RGlutLAF *laf);
    static void unregisterLAF(std::string const &name);

    static RGlutLAF *getLAF(std::string name);

private:
    RGlutLAFManager();
    ~RGlutLAFManager();

    RGlutLAFManager(RGlutLAFManager const &);
    RGlutLAFManager &operator=(RGlutLAFManager const &);

    static RGlutLAFManager *instance();

    void doRegisterLAF(std::string const &name, RGlutLAF *laf);
    void doUnregisterLAF(std::string const &name);

private:
    typedef std::map<std::string, RGlutLAF *> LAFs;
    typedef LAFs::iterator LAFsIter;
    typedef LAFs::const_iterator LAFsConstIter;

    LAFs mLAFs;
};

#endif

