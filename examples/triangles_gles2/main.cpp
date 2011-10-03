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
#include <string.h>

#if defined(GLUT_ES2)
#include <glut.h>
#else
#include <GL/glut.h>
#endif


using namespace std;

GLfloat xRot = 0.0f;
GLfloat yRot = 0.0f;

static const char *vertxShader = "   uniform mat4 u_mvp_matrix; \n"
        "attribute vec4 a_position;                             \n"
        "attribute vec4 a_color;                                \n"
        "varying mediump vec4 v_color;                                  \n"
        "                                                       \n"
        "void main()                                            \n"
        "{                                                      \n"
        "   v_color = a_color                                   \n;"
        "   gl_Position = u_mvp_matrix * a_position;            \n"
        "}                                                      \n";

static const char *fragShader = "                               \n"
        "varying mediump vec4 v_color;                                  \n"
        "                                                       \n"
        "void main()                                            \n"
        "{                                                      \n"
        "   gl_FragColor = v_color;                             \n"
        "}                                                      \n";

int   scaleCount = 0;
int   scaleDir   = 1;

GLuint program;
GLuint vshader;
GLuint fshader;

float matrix[4*4];
                
std::vector<GLfloat> triangleColors;
std::vector<GLfloat> trianglePaths;

void parallel(float left, float right, float bottom, float top, float near, float far, float matrix[4*4])
{
    matrix[0*4+0]  = 2.0f / (right - left);
    matrix[0*4+1]  = 0.0f;
    matrix[0*4+2]  = 0.0f;
    matrix[0*4+3]  = 0.0f;
    matrix[1*4+0]  = 0.0f;
    matrix[1*4+1]  = 2.0f / (top - bottom);
    matrix[1*4+2]  = 0.0f;
    matrix[1*4+3]  = 0.0f;
    matrix[2*4+0]  = 0.0f;
    matrix[2*4+1]  = 0.0f;
    matrix[2*4+2] = -2.0f / (far - near);
    matrix[2*4+3] = 0.0f;
    matrix[3*4+0] = -(right + left) / (right - left);
    matrix[3*4+1] = -(top + bottom) / (top - bottom);
    matrix[3*4+2] = -(far + near) / (far - near);
    matrix[3*4+3] = 1.0f;
}

void translate(float tx, float ty, float tz, float matrix[4*4])
{
    matrix[3*4+0] += (matrix[0*4+0] * tx + matrix[1*4+0] * ty + matrix[2*4+0] * tz);
    matrix[3*4+1] += (matrix[0*4+1] * tx + matrix[1*4+1] * ty + matrix[2*4+1] * tz);
    matrix[3*4+2] += (matrix[0*4+2] * tx + matrix[1*4+2] * ty + matrix[2*4+2] * tz);
    matrix[3*4+3] += (matrix[0*4+3] * tx + matrix[1*4+3] * ty + matrix[2*4+3] * tz);
}

float * multimatrix4x4(float matrix1[4*4], float matrix2[4*4], float matrix3[4*4])
{
    for (int i=0; i<4; i++) {
        matrix3[i*4+0] = (matrix1[i*4+0] * matrix2[0*4+0]) +
                         (matrix1[i*4+1] * matrix2[1*4+0]) +
                         (matrix1[i*4+2] * matrix2[2*4+0]) +
                         (matrix1[i*4+3] * matrix2[3*4+0]);

        matrix3[i*4+1] = (matrix1[i*4+0] * matrix2[0*4+1]) + 
                         (matrix1[i*4+1] * matrix2[1*4+1]) +
                         (matrix1[i*4+2] * matrix2[2*4+1]) +
                         (matrix1[i*4+3] * matrix2[3*4+1]);

        matrix3[i*4+2] = (matrix1[i*4+0] * matrix2[0*4+2]) + 
                         (matrix1[i*4+1] * matrix2[1*4+2]) +
                         (matrix1[i*4+2] * matrix2[2*4+2]) +
                         (matrix1[i*4+3] * matrix2[3*4+2]);

        matrix3[i*4+3] = (matrix1[i*4+0] * matrix2[0*4+3]) + 
                         (matrix1[i*4+1] * matrix2[1*4+3]) +
                         (matrix1[i*4+2] * matrix2[2*4+3]) +
                         (matrix1[i*4+3] * matrix2[3*4+3]);
    }

    return matrix3;
}

void rotate(float angle, float x, float y, float z, float matrix[4*4])
{
   float mag = sqrtf(x * x + y * y + z * z);

   if (mag > 0.0f) {
       float sinAngle = sinf(angle * (float)M_PI / 180.0f);
       float cosAngle = cosf(angle * (float)M_PI / 180.0f);

       float xx, yy, zz, xy, yz, zx, xs, ys, zs;
       float oneMinusCos;
       float rotMat[4*4];

       x /= mag;
       y /= mag;
       z /= mag;

       xx = x * x;
       yy = y * y;
       zz = z * z;
       xy = x * y;
       yz = y * z;
       zx = z * x;
       xs = x * sinAngle;
       ys = y * sinAngle;
       zs = z * sinAngle;
       oneMinusCos = 1.0f - cosAngle;

       rotMat[0*4+0] = (oneMinusCos * xx) + cosAngle;
       rotMat[0*4+1] = (oneMinusCos * xy) - zs;
       rotMat[0*4+2] = (oneMinusCos * zx) + ys;
       rotMat[0*4+3] = 0.0F; 

       rotMat[1*4+0] = (oneMinusCos * xy) + zs;
       rotMat[1*4+1] = (oneMinusCos * yy) + cosAngle;
       rotMat[1*4+2] = (oneMinusCos * yz) - xs;
       rotMat[1*4+3] = 0.0F;

       rotMat[2*4+0] = (oneMinusCos * zx) - ys;
       rotMat[2*4+1] = (oneMinusCos * yz) + xs;
       rotMat[2*4+2] = (oneMinusCos * zz) + cosAngle;
       rotMat[2*4+3] = 0.0F; 

       rotMat[3*4+0] = 0.0F;
       rotMat[3*4+1] = 0.0F;
       rotMat[3*4+2] = 0.0F;
       rotMat[3*4+3] = 1.0F;

       float resultMat[4*4];
       multimatrix4x4(rotMat, matrix, resultMat);
       
       memcpy(matrix, resultMat, 4 * 4 * sizeof(float));
   }
}

void scale(float sx, float sy, float sz, float matrix[4*4])
{
    matrix[0*4+0] *= sx;
    matrix[0*4+1] *= sx;
    matrix[0*4+2] *= sx;
    matrix[0*4+3] *= sx;

    matrix[1*4+0] *= sy;
    matrix[1*4+1] *= sy;
    matrix[1*4+2] *= sy;
    matrix[1*4+3] *= sy;

    matrix[2*4+0] *= sz;
    matrix[2*4+1] *= sz;
    matrix[2*4+2] *= sz;
    matrix[2*4+3] *= sz;
}

int init()
{
    program = glCreateProgram();
    
    vshader = glCreateShader(GL_VERTEX_SHADER);
    fshader = glCreateShader(GL_FRAGMENT_SHADER);
    
    GLint lengths[2] = {strlen(vertxShader), 0};
    glShaderSource(vshader, 1, &vertxShader, lengths);
    glCompileShader(vshader);

    GLint result;
    glGetShaderiv(vshader, GL_COMPILE_STATUS, &result);

    if (result != GL_TRUE) {
        return 0;
    }

    lengths[0] = strlen(fragShader);
    lengths[1] = 0;
    glShaderSource(fshader, 1, &fragShader, lengths);
    glCompileShader(fshader);
    
    glGetShaderiv(fshader, GL_COMPILE_STATUS, &result);

    if (result != GL_TRUE) {
        return 0;
    }

    glAttachShader(program, vshader);
    glAttachShader(program, fshader);
    
    glLinkProgram(program);
    GLint lr;
    glGetProgramiv(program, GL_LINK_STATUS, &lr);
    if (lr != GL_TRUE)
        {
        return 0;
        }
    
    glClearColor(1.0, 1.0, 0.0, 1.0);
    glFrontFace(GL_CW);

    int pivot = 1;

    trianglePaths.push_back(0.0f);
    trianglePaths.push_back(0.0f);
    trianglePaths.push_back(75.0f);
    
    triangleColors.push_back(0.0f);
    triangleColors.push_back(1.0f);
    triangleColors.push_back(0.0f);
    triangleColors.push_back(1.0f);
    
    for (GLfloat angle = 0.0f; angle <= (2.0f * M_PI) + 1.0f; angle += M_PI/38.0f) {
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
    
    return 1;
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(program);

    rotate(yRot, 0.0f, 1.0f, 0.0f, matrix);
    translate(0, 0, -5, matrix);
    rotate(xRot, 1.0f, 0.0f, 0.0f, matrix);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    GLint clrLoc = glGetAttribLocation(program, "a_color");
    GLint posLoc = glGetAttribLocation(program, "a_position");
    GLint matLoc = glGetUniformLocation(program, "u_mvp_matrix");


    glUniformMatrix4fv(matLoc, 1, GL_FALSE, (float *)matrix);
    glVertexAttribPointer(clrLoc, 4, GL_FLOAT, GL_FALSE, 0, &triangleColors[0]);
    glVertexAttribPointer(posLoc, 3, GL_FLOAT, GL_FALSE, 0, &trianglePaths[0]);
    glEnableVertexAttribArray(clrLoc);
    glEnableVertexAttribArray(posLoc);
    glDrawArrays(GL_TRIANGLE_FAN, 0, trianglePaths.size()/3);
    
    rotate(-xRot, 1.0f, 0.0f, 0.0f, matrix);
    translate(0, 0, 5, matrix);
    rotate(-yRot, 0.0f, 1.0f, 0.0f, matrix);

    glUseProgram(0);
    
    glutSwapBuffers();
}

void reshape(int w, int h)
{
    GLfloat nRange = 100.0f;
    glViewport(0, 0, w, h);
    
    if (w <= h) {
        parallel(-nRange, nRange, -nRange*h/w, nRange*h/w, -nRange, nRange, matrix);
    } else {
        parallel(-nRange*w/h, nRange*w/h, -nRange, nRange, -nRange, nRange, matrix);
    }
}

void mouseFunction(int button, int state, int, int)
{
    switch (button) {
        case GLUT_LEFT_BUTTON: {
            if (state == GLUT_DOWN) {
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
                scale(scaleUnit, scaleUnit, scaleUnit, matrix);
            }
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

void keyboard(unsigned char key, int, int)
{
    switch (key) {
        case 'q':
        case 'Q': {
            exit(0);
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
    glutTimerFunc(30, timeout, 0);
}

#if defined(__SYMBIAN32__) || defined(SYMBIAN)
GLDEF_C TInt E32Main()
{
    char * argv[] = {"triangle_2", "-renderer", "gles2"};
    int    argc = sizeof(argv) / sizeof(argv[0]);
    glutInit(&argc, argv);
    
#else
int main(int argc, char ** argv)
{
#ifdef GLUT_ES2
    char * v[] = {"triangle_2", "-renderer", "gles2"};
    int    c = sizeof(v) / sizeof(v[0]);
    glutInit(&c, v);
#else
    glutInit(&argc, argv);
#endif
#endif
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutCreateWindow(argv[0]);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(30, timeout, 0);
    glutMouseFunc(mouseFunction);
    glutKeyboardFunc(keyboard);
    
    if (!init()) {
        exit(1);
    }
    
    glutMainLoop();

    return 0;
}
