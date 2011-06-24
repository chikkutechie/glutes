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
 
#if defined(__SYMBIAN32__) || defined(SYMBIAN)
#if !defined(GLES)
#define GLES
#endif
#endif
#if defined(GLES)
#include <glut.h>
#define GLORTHO glOrthof
#else
#include <GL/glut.h>
#define GLORTHO glOrtho
#endif

#include <vector>
#include <math.h>

using namespace std;

enum MenuIds
{
    ZoomMenu = 10,
    ExitMenu = 20
};

char * ZoomInText  = "Zoom In";
char * ZoomOutText = "Zoom Out";
char * PauseText   = "Pause";
char * ResumeText  = "Resume";
char * ExitText    = "Exit";

class TrianglesState
{
public:
    GLfloat xRot;
    GLfloat yRot;
    
    int scaleCount;
    int scaleDir;
    int doRotations;
    int changeAnimState;
    int menuId;
    char * zoomMenuText;
    char * animMenuText;
};

TrianglesState * state = 0;

void init();
void scale();
void display();
void reshape(int w, int h);
void menu(int id);
void scale();
void timeout(int);
void createMenu();

void init()
{
    state = new TrianglesState;
    
    state->xRot        = 0.0f;
    state->yRot        = 0.0f;
    
    state->scaleCount      = 0;
    state->scaleDir        = 1;
    state->doRotations     = 1;
    state->changeAnimState = 0;
    state->menuId          = 0;
    state->zoomMenuText  = ZoomOutText;
    state->animMenuText  = PauseText;
    
    createMenu();
}

void display()
{
    glClearColor(1.0, 1.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glShadeModel(GL_SMOOTH);
    glFrontFace(GL_CW);
    glPushMatrix();
    glRotatef(state->xRot, 1.0f, 0.0f, 0.0f);
    glRotatef(state->yRot, 0.0f, 1.0f, 0.0f);
    
    int pivot = 1;
    
    std::vector<GLfloat> triangleColors;
    std::vector<GLfloat> trianglePaths;
    
    trianglePaths.push_back(0.0f);
    trianglePaths.push_back(0.0f);
    trianglePaths.push_back(75.0f);
    
    triangleColors.push_back(0.0f);
    triangleColors.push_back(1.0f);
    triangleColors.push_back(0.0f);
    triangleColors.push_back(1.0f);
    
    for (GLfloat angle = 0.0f;
            angle <= (2.0f * M_PI) + 1.0f;
            angle += M_PI/38.0f) {
        
        GLfloat x = 50.0f * sin(angle);
        GLfloat y = 50.0f * cos(angle);

        if (pivot % 2 == 0) {
            triangleColors.push_back(0.0f);
            triangleColors.push_back(1.0f);
            triangleColors.push_back(0.0f);
            triangleColors.push_back(1.0f);
        } else {
            triangleColors.push_back(1.0f);
            triangleColors.push_back(0.0f);
            triangleColors.push_back(0.0f);
            triangleColors.push_back(1.0f);
        }
        
        pivot++;
        trianglePaths.push_back(x);
        trianglePaths.push_back(y);
        trianglePaths.push_back(0.0f);
    }
    
    glVertexPointer(3, GL_FLOAT, 0, &trianglePaths[0]);
    glColorPointer(4, GL_FLOAT, 0, &triangleColors[0]);
    
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glDrawArrays(GL_TRIANGLE_FAN, 0, trianglePaths.size() / 3);
    glDisableClientState(GL_VERTEX_ARRAY);
    glPopMatrix();
    
    glutSwapBuffers();
}

void reshape(int w, int h)
{
    GLfloat nRange = 100.0f;
    glViewport(0, 0, w, h);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    if (w <= h) {
        GLORTHO(-nRange, nRange, -nRange*h/w, nRange*h/w, -nRange, nRange);
    } else {
        GLORTHO(-nRange*w/h, nRange*w/h, -nRange, nRange, -nRange, nRange);
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void menu(int id)
{
    switch (id) {
        case ZoomMenu: {
            scale();
            break;
        }
                
        case ExitMenu: {
            exit(0);
            break;
        }
    }
}

void scale()
{
    float scaleUnit = 1.0f;

    if (state->scaleDir < 0) {
        if (state->scaleCount <= 0) {
            state->scaleCount = 0;
            state->scaleDir = state->scaleDir * -1;
            
            state->zoomMenuText = ZoomOutText;

            createMenu();

        } else {                    
            scaleUnit = 1.0f / 0.8f;
            state->scaleCount--;
        }
    } else {
        if (state->scaleCount >= 10) {
            state->scaleCount = 10;
            state->scaleDir = state->scaleDir * -1;
            
            state->zoomMenuText = ZoomInText;
            
            createMenu();

        } else {                    
            scaleUnit = 0.8f;
            state->scaleCount++;
        }
    }
    glScalef(scaleUnit, scaleUnit, scaleUnit);
}

void timeout(int)
{    
    if (state->doRotations) {
        if (state->xRot >= 360.0f) {
            state->xRot = 0.0f;
        }
        
        state->xRot += 5.0f;
        state->yRot = state->xRot;
        
        glutPostRedisplay();
        glutTimerFunc(20, timeout, 0);
    }
}

void createMenu()
{
    if (state->menuId) {
        glutDestroyMenu(state->menuId);
        state->menuId = 0;
    }
        
    state->menuId = glutCreateMenu(menu);
    
    glutAddMenuEntry(state->zoomMenuText, ZoomMenu);
    glutAddMenuEntry(ExitText, ExitMenu);

    glutAttachMenu(GLUT_LEFT_BUTTON);
}

#if defined(__SYMBIAN32__) || defined(SYMBIAN)
GLDEF_C TInt E32Main()
{

    char * argv[] = {"triangle", "-renderer", "gles"};
    int    argc = sizeof(argv) / sizeof(argv[0]);
    
#else
int main(int argc, char ** argv)
{
#endif
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutCreateWindow(argv[0]);
    
    init();
        
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(20, timeout, 0);
        
    glutMainLoop();

    return 0;
}
