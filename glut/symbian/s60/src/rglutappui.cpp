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
 
#include "rglutappui.h"
#include "rgluteventhandler.h"
#include "rglutinterface.h"

#include <eikmenub.h>
#include <eikmenup.h>

#include <aknappui.h>
#include <eikbtgpc.h>
#include <avkon.hrh>
#include <avkon.rsg>

RGlutAppUi::RGlutAppUi()
  : mEH(0)
{
}

RGlutAppUi::~RGlutAppUi()
{
}

void RGlutAppUi::ConstructL()
{
    iEikonEnv->DisableExitChecks(ETrue);
    
    TInt flags = CEikAppUi::ENoScreenFurniture |
                 CEikAppUi::ENonStandardResourceFile |
                 CAknAppUi::EAknEnableSkin;
    
    switch (RGlutInterface::getInterface()->getOrientation()) {
        case RGlutInterface::Portrait:
            flags |= EAppOrientationPortrait;
            break;
        case RGlutInterface::Landscape:
            flags |= EAppOrientationLandscape;
            break;
        case RGlutInterface::Automatic:
            flags |= EAppOrientationAutomatic;
            break;
    }
    
    BaseConstructL(flags);
    
    iEikonEnv->AppUiFactory()->CreateResourceIndependentFurnitureL(this);
    CEikButtonGroupContainer *cba = CEikButtonGroupContainer::NewL(
                                      CEikButtonGroupContainer::ECba,
                                      CEikButtonGroupContainer::EHorizontal,
                                      this,
                                      R_AVKON_SOFTKEYS_OPTIONS_EXIT);
    CEikonEnv::Static()->AppUiFactory()->SwapButtonGroup(cba);

    CEikMenuBar * menuBar = new(ELeave) CEikMenuBar;
    menuBar->ConstructL(this, 0, R_AVKON_MENUPANE_EMPTY);
    menuBar->SetMenuType(CEikMenuBar::EMenuOptions);
    AddToStackL(menuBar, ECoeStackPriorityMenu, ECoeStackFlagRefusesFocus);
    CEikonEnv::Static()->AppUiFactory()->SwapMenuBar(menuBar);
    cba->ActivateL();
    
}

void RGlutAppUi::setEventHandler(RGlutEventHandler * eh)
{
    this->mEH = eh;
}

void RGlutAppUi::SetFullScreen(bool v)
{
    if (v) {
        SetFullScreenApp(ETrue);
    } else {
        SetFullScreenApp(EFalse);
    }
}

void RGlutAppUi::Exit()
{
    CAknAppUi::Exit();
}

void RGlutAppUi::HandleCommandL(TInt command)
{
    switch (command) {
        case EEikCmdExit:
        case EAknSoftkeyExit: {
            Exit();
        }
        break;
    }
}

void RGlutAppUi::HandleStatusPaneSizeChange()
{
    CAknAppUi::HandleStatusPaneSizeChange();
    if (mEH) {
        TRect rect = ClientRect();
        mEH->rerect(rect.iTl.iX, rect.iTl.iY, rect.Width(), rect.Height());
    }
}

void RGlutAppUi::HandleResourceChangeL(TInt type)
{
    CAknAppUi::HandleResourceChangeL(type);
}

void RGlutAppUi::HandleWsEventL(const TWsEvent &event,
                                CCoeControl *destination)
{
    CAknAppUi::HandleWsEventL(event, destination);
}

void RGlutAppUi::ProcessCommandL(TInt command)
{
    CAknAppUi::ProcessCommandL(command);
}

void RGlutAppUi::HandleScreenDeviceChangedL()
{
    CAknAppUi::HandleScreenDeviceChangedL();
    if (mEH) {
        TRect rect = ClientRect();
        mEH->rerect(rect.iTl.iX, rect.iTl.iY, rect.Width(), rect.Height());
    }
}

