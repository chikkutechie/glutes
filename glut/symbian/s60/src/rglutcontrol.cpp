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
 
#include "rglutcontrol.h"
#include "rgluteventhandler.h"

RGlutControl::RGlutControl()
  : CCoeControl(),
    mEH(0)
{
}

void RGlutControl::ConstructL(const TRect& aRect)
{
    CreateWindowL();
    SetRect(aRect);
    ActivateL();
}

RGlutControl::~RGlutControl()
{
}

void RGlutControl::setEventHandler(RGlutEventHandler * eh)
{
    this->mEH = eh;
}

void RGlutControl::Draw(const TRect &) const
{        
    if (mEH) {
        mEH->draw();
    }
}

void RGlutControl::SizeChanged()
{
    CCoeControl::SizeChanged();
}

void RGlutControl::PositionChanged()
{
    CCoeControl::PositionChanged();
}

void RGlutControl::HandlePointerEventL(const TPointerEvent & aPointerEvent)
{
    if (Rect().Contains(aPointerEvent.iPosition)) {
        if (mEH) {
            mEH->mouse(aPointerEvent.iType,
                aPointerEvent.iModifiers,
                aPointerEvent.iPosition.iX, aPointerEvent.iPosition.iY);
        }
        CCoeControl::HandlePointerEventL(aPointerEvent);
    }
    
}

TKeyResponse RGlutControl::OfferKeyEventL(const TKeyEvent& aKeyEvent, TEventCode aType)
{
    if (mEH) {
        if (aType == EEventKeyUp) {
            mEH->keyboard(aKeyEvent.iScanCode, aKeyEvent.iModifiers, 0, 0);
        }
    }
    
    return EKeyWasNotConsumed;
}

RWindow& RGlutControl::nativeWindow()
{
    return Window();
}

