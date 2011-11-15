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

#ifndef _RGLUTSTYLE_H_
#define _RGLUTSTYLE_H_

#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>

#include "rglutcolor.h"
#include "rglutfont.h"

/**
 * @class RGlutLAF
 * @brief Defines basic FW properties.
 */
class RGlutLAF
{
public:
    RGlutLAF();
    virtual ~RGlutLAF();

    /**
     * Retrieves the default font for the FW.
     */
    virtual RGlutFont defaultFont();
    /**
     * Retrieves the menu font for the FW.
     */
    virtual RGlutFont menuItemFont();

    /**
     * Retrieves the maximum menu width for the FW.
     */
    virtual int maximumMenuWidth();
    /**
     * Retrieves the maximum menu item height for the FW.
     */
    virtual int maximumMenuItemHeight();
    /**
     * Retrieves the minimum menu width for the FW.
     */
    virtual int minimumMenuWidth();
    /**
     * Retrieves the minimum menu item height for the FW.
     */
    virtual int minimumMenuItemHeight();

    /**
     * Retrieves the menu item gap for the FW.
     */
    virtual int menuItemGap();

    /**
     * Retrieves the menu background color for the FW.
     * @see RGlutColor
     */
    virtual RGlutColor getMenuBackgroundColor();
    /**
     * Retrieves the menu text color for the FW.
     * @see RGlutColor
     */
    virtual RGlutColor getMenuTextColor();

    /**
     * Creates the normal menu item background.
     */
    virtual Pixmap createMenuItemBackgroundNormal(int w, int h);
    /**
     * Creates the pressed menu item background.
     */
    virtual Pixmap createMenuItemBackgroundPressed(int w, int h);

private:

};

#endif

