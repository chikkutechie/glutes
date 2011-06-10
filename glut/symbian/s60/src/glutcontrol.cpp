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
 
#include "glutcontrol.h"
#include "gluteventhandler.h"

GlutControl::GlutControl()
  : CCoeControl(),
    mEH(0)
{
}

void GlutControl::ConstructL(const TRect& aRect)
{
    CreateWindowL();
    SetRect(aRect);
    ActivateL();
}

GlutControl::~GlutControl()
{
}

void GlutControl::setEventHandler(GlutEventHandler * eh)
{
    this->mEH = eh;
}

void GlutControl::Draw(const TRect &) const
{        
    if (mEH) {
        mEH->draw();
    }
}

void GlutControl::SizeChanged()
{
    CCoeControl::SizeChanged();
}

void GlutControl::PositionChanged()
{
    CCoeControl::PositionChanged();    
}

RWindow& GlutControl::nativeWindow()
{
    return Window();
}

void GlutControl::HandlePointerEvent(const TPointerEvent& aPointerEvent)
{
    if (mEH) {
        mEH->mouse(aPointerEvent.iType, aPointerEvent.iModifiers,
                  aPointerEvent.iPosition.iX, aPointerEvent.iPosition.iY);
    }
}

TKeyResponse GlutControl::OfferKeyEvent(const TKeyEvent& aKeyEvent, TEventCode)
{
    if (mEH) {
        mEH->keyboard(aKeyEvent.iCode, 0, 0);
    }
    
    return EKeyWasConsumed;
}

