/**
 * Copyright (c) 2010-2015 Ranjith TV.            
 * All rights reserved.  
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *    This product includes software developed by the <organization>.
 * 4. Neither the name of the <organization> nor the
 *    names of its contributors may be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY Ranjith TV ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL Ranjith TV BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
 
#include <glut.h>
#include <vector>
#include <math.h>

using namespace std;

GLfloat xRot = 0.0f;
GLfloat yRot = 0.0f;

void display()
{
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glShadeModel(GL_SMOOTH);
    glFrontFace(GL_CW);
    glPushMatrix();
    glRotatef(xRot, 1.0f, 0.0f, 0.0f);
    glRotatef(yRot, 0.0f, 1.0f, 0.0f);
    
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
    
    for (GLfloat angle = 0.0f; angle <= (2.0f * M_PI) + 1.0f; angle += M_PI/38.0f)
        {
        GLfloat x = 50.0f * sin(angle);
        GLfloat y = 50.0f * cos(angle);

        if (pivot % 2 == 0)
        {
            triangleColors.push_back(0.0f);
            triangleColors.push_back(1.0f);
            triangleColors.push_back(0.0f);
            triangleColors.push_back(1.0f);
        }
        else
        {
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
    if (w <= h) 
    {
        glOrthof(-nRange, nRange, -nRange*h/w, nRange*h/w, -nRange, nRange);
    }
    else 
    {
        glOrthof(-nRange*w/h, nRange*w/h, -nRange, nRange, -nRange, nRange);
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void mouseFunction(int button, int , int, int)
{
    switch (button) {
        case GLUT_LEFT_BUTTON: {
            glTranslatef(0.0f, 0.0f, 5.0f);
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
    if (xRot >= 360.0f) {
        xRot = 0.0f;
    }
    
    xRot += 5.0f;
    yRot = xRot;
    
    glutPostRedisplay();
    glutTimerFunc(20, timeout, 0);
}

#if defined(__SYMBIAN32__) || defined(SYMBIAN)
GLDEF_C TInt E32Main()
{
    int    argc = 1;
    char * argv[2];

    argv[0] = "g1";
    
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
    glutMainLoop();

    return 0;
}

