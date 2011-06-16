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

#include <vector>
#include <math.h>

using namespace std;

#if defined(__SYMBIAN32__) || defined(SYMBIAN)
#include <glut.h>
#else
#include <GL/glut.h>
#endif

VGPath path;

VGPaint paint;

VGfloat rotation = 0.0f;

int   scaleCount = 0;
int   scaleDir   = 1;

int width;
int height;

void init()
{
    path = vgCreatePath(VG_PATH_FORMAT_STANDARD,
                           VG_PATH_DATATYPE_F, 1, 0, 4, 6,
                           VG_PATH_CAPABILITY_APPEND_TO);
        
    const VGubyte pathSegments[] = {
        VG_MOVE_TO_ABS,
        VG_LINE_TO_ABS,
        VG_LINE_TO_ABS,
        VG_CLOSE_PATH
    };

    const VGfloat pathCoords[] = {
         0.0f, 1.0f,
        -1.0f, 0.0f,
         1.0f, 0.0f
    };

    vgAppendPathData(path, 4,
                        pathSegments, pathCoords);

    vgSeti(VG_STROKE_CAP_STYLE, VG_CAP_ROUND);
    vgSeti(VG_STROKE_JOIN_STYLE, VG_JOIN_MITER);
    
    paint = vgCreatePaint();
    
    vgSetPaint(paint, VG_FILL_PATH);
    vgSetParameteri(paint, VG_PAINT_TYPE, VG_PAINT_TYPE_LINEAR_GRADIENT);
    
    const VGfloat gradientParam[] = {-1.0, 0.0f, 1.0f, 1.0f};
    vgSetParameterfv(paint, VG_PAINT_LINEAR_GRADIENT, 4, gradientParam);
    
    vgSetParameteri(paint, VG_PAINT_COLOR_RAMP_SPREAD_MODE,
                              VG_COLOR_RAMP_SPREAD_REPEAT);

    const VGfloat fillStops[] = {
        0.0, 0.0, 1.0, 0.0, 1.0,
        1.0, 1.0, 0.0, 0.0, 1.0 
    };
    
    vgSetParameterfv(paint, VG_PAINT_COLOR_RAMP_STOPS,   
                                      5*2, fillStops);
    
    VGfloat bgColor[] = {1.0, 1.0, 0.0, 1.0};
    vgSetfv(VG_CLEAR_COLOR, 4, bgColor);
}

void display()
{
    vgClear(0, 0, width, height);
    
    VGfloat tx = (VGfloat)width  / 2.0f;
    VGfloat ty = (VGfloat)height / 2.0f;
    
    VGfloat sx = (VGfloat)width  / 4.0f;
    VGfloat sy = (VGfloat)height / 4.0f;
    
    vgTranslate(tx, ty);
    vgRotate(rotation);
    vgScale(sx, sy);
    
    vgDrawPath(path, VG_FILL_PATH);
    
    vgScale(1.0f/sx, 1.0f/sy);
    vgRotate(-rotation);
    vgTranslate(-tx, -ty);
    
    glutSwapBuffers();
}

void reshape(int w, int h)
{
    width = w;
    height = h;

    vgSeti(VG_MATRIX_MODE, VG_MATRIX_PATH_USER_TO_SURFACE);
    vgLoadIdentity();
}

void mouseFunction(int button, int , int, int)
{
    switch (button) {
        case GLUT_LEFT_BUTTON: {
            float scaleUnit = 1.0f;
            
            if (scaleDir < 0) {
                if (scaleCount <= 0) {
                    scaleCount = 0;
                    scaleDir = scaleDir * -1;
                } else {                    
                    scaleUnit = 1.0f / 0.8f;
                    scaleCount--;
                }
            } else {
                if (scaleCount >= 10) {
                    scaleCount = 10;
                    scaleDir = scaleDir * -1;
                } else {                    
                    scaleUnit = 0.8f;
                    scaleCount++;
                }
            }

            vgScale(scaleUnit, scaleUnit);
            break;
        }
        case GLUT_MIDDLE_BUTTON: {
            break;
        }
        case GLUT_RIGHT_BUTTON: {
            break;
        }
            
    }
}
void timeout(int)
{
    if (rotation >= 360.0f) {
        rotation = 0.0f;
    }
    
    rotation += 5.0f;
    
    glutPostRedisplay();
    glutTimerFunc(20, timeout, 0);
}

#if defined(__SYMBIAN32__) || defined(SYMBIAN)
GLDEF_C TInt E32Main()
{

    char * argv[] = {"triangle", "-renderer", "vg"};
    int    argc = sizeof(argv) / sizeof(argv[0]);
    
#else
int main(int argc, char ** argv)
{
#endif
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutCreateWindow(argv[0]);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(20, timeout, 0);
    glutMouseFunc(mouseFunction);
    
    init();
    
    glutMainLoop();

    return 0;
}
