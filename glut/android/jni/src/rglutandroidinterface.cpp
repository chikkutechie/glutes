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

#include "rglutandroidinterface.h"
#include "reglglutglbinder.h"

static RGlutInterface *interface = 0;

RGlutInterface *RGlutInterface::getInterface()
{
    if (!interface) {
        interface = new  RGlutAndroidInterface();
    }
    return interface;
}

bool RGlutInterface::destroyInterface()
{
    delete interface;
    interface = 0;
    return true;
}

RGlutAndroidInterface::RGlutAndroidInterface()
    : KParamRenderer("-renderer"),
      KParamRendererVG("vg"),
      KParamRendererGL("gl"),
      KParamRendererGLES("gles"),
      KParamRendererGLES2("gles2"),
      KParamOrientation("-orientation"),
      KParamOrientationPortrait("portrait"),
      KParamOrientationLandscape("landscape"),
      KParamOrientationAutomatic("automatic")
{
    setRenderer(GLES);
}

RGlutAndroidInterface::~RGlutAndroidInterface()
{}

void RGlutAndroidInterface::parseArguments(int argc, char **argv)
{
    for (int i = 0; i < argc; ++i) {

        std::string name(argv[i]);

        if (name == KParamRenderer) {

            if (i + 1 >= argc) {
                break;
            }

            std::string value(argv[i + 1]);
            if (value == KParamRendererVG) {
                setRenderer(VG);
            } else if (value == KParamRendererGLES2) {
                setRenderer(GLES2);
            } else if (value == KParamRendererGLES) {
                setRenderer(GLES);
            } else if (value == KParamRendererGL) {
                setRenderer(GL);
            }
            ++i;
        }

        if (name == KParamOrientation) {

            if (i + 1 >= argc) {
                break;
            }

            std::string value(argv[i + 1]);

            if (value == KParamOrientationPortrait) {
                setOrientation(Portrait);
            } else if (value == KParamOrientationLandscape) {
                setOrientation(Landscape);
            } else if (value == KParamOrientationAutomatic) {
                setOrientation(Automatic);
            }
            ++i;
        }
    }
}

void RGlutAndroidInterface::createBinder()
{
    /*
     * currently egl is used to bind native window system
     * with opengl/openvg/opengles
     */
    RGlutGLBinder::API api;
    switch (getRenderer()) {
    case GL:
        api = RGlutGLBinder::OPENGL_API;
        break;
    case GLES2:
        api = RGlutGLBinder::OPENGL_ES2_API;
        break;
    case VG:
        api = RGlutGLBinder::OPENVG_API;
        break;
    case GLES:
    default:
        api = RGlutGLBinder::OPENGL_ES_API;
        break;
    }

    mBinder = new REGLGlutGLBinder(api);
    mBinder->initialize();
}


void RGlutAndroidInterface::intialize(int argc, char **argv)
{
    parseArguments(argc, argv);

    createBinder();
}

void RGlutAndroidInterface::terminate()
{
    if (mBinder) {
        mBinder->terminate();
        delete mBinder;
        mBinder = 0;
    }
}

void RGlutAndroidInterface::initDisplayMode(unsigned int mode)
{
   mDisplayMode = mode;

    int rgb = mDisplayMode & 1;
    if (rgb == 0) {
        // GLUT_RGBA or GLUT_RGB
        mBinder->setRedSize(8);
        mBinder->setGreenSize(8);
        mBinder->setBlueSize(8);
        mBinder->setAlphaSize(8);
    }

    if (mDisplayMode & GLUT_DEPTH) {
        mBinder->setDepth(true);
    }

    if (mDisplayMode & GLUT_STENCIL) {
        mBinder->setStencil(true);
    }
}

int RGlutAndroidInterface::createWindow()
{}
void RGlutAndroidInterface::destroyWindow(int win)
{}
int RGlutAndroidInterface::getWindow()
{}
void RGlutAndroidInterface::setWindow(int win)
{}
void RGlutAndroidInterface::setWindowTitle(const char *title)
{}
void RGlutAndroidInterface::showWindow()
{}
void RGlutAndroidInterface::fullScreen()
{}
void RGlutAndroidInterface::hideWindow()
{}
void RGlutAndroidInterface::positionWindow(int x, int y)
{}
void RGlutAndroidInterface::reshapeWindow(int width, int height)
{}
void RGlutAndroidInterface::popWindow()
{}
void RGlutAndroidInterface::pushWindow()
{}

void RGlutAndroidInterface::timerFunc(unsigned int millis, void (*func)(int), int value)
{}

void RGlutAndroidInterface::exec()
{}
void RGlutAndroidInterface::redraw(int win)
{}
void RGlutAndroidInterface::flush()
{}

int RGlutAndroidInterface::createMenu(void ( *)(int menu))
{}
void RGlutAndroidInterface::destroyMenu(int menu)
{}
int RGlutAndroidInterface::getMenu()
{}
void RGlutAndroidInterface::setMenu(int menu)
{}
void RGlutAndroidInterface::addMenuEntry(const char *label, int value)
{}
void RGlutAndroidInterface::removeMenuItem(int item)
{}
void RGlutAndroidInterface::attachMenu(int button)
{}
void RGlutAndroidInterface::detachMenu(int button)
{}

int RGlutAndroidInterface::getModifiers()
{}
int RGlutAndroidInterface::getValue(unsigned int state)
{}

void RGlutAndroidInterface::draw()
{}
void RGlutAndroidInterface::reshape(int w, int h)
{}
void RGlutAndroidInterface::rerect(int x, int y, int w, int h)
{}
void RGlutAndroidInterface::repos(int x, int y)
{}
void RGlutAndroidInterface::keyboard(unsigned char key, unsigned int modifier, int x, int y)
{}
void RGlutAndroidInterface::mouse(int button, int modifier, int x, int y, bool pressed)
{}
void RGlutAndroidInterface::motion(int button, int modifier, int x, int y)
{}
void RGlutAndroidInterface::removeTimer(RGlutTimer *timer)
{}


