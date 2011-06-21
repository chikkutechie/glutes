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
 
#ifndef GLUTAPPUI_H_
#define GLUTAPPUI_H_

#include <aknappui.h>

class GlutEventHandler;

class GlutAppUi : public CAknAppUi
{
public:
    GlutAppUi();
    virtual ~GlutAppUi();
    virtual void ConstructL();
    void HandleCommandL(TInt command);
    void setEventHandler(GlutEventHandler * eh);
    void Exit();
    void SetFullScreen(bool v);
    
protected:
    void HandleResourceChangeL(TInt type);
    void HandleWsEventL(const TWsEvent &event, CCoeControl *destination);
    virtual void ProcessCommandL(TInt aCommand);
    virtual void HandleScreenDeviceChangedL();
    void HandleStatusPaneSizeChange();
    
private:
    GlutEventHandler * mEH;
};

#endif

