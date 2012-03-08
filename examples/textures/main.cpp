
#ifdef _GLES_EMU_
#include "esUtil.h"
ESContext esContext;
#else
#ifdef GLUT_ES2
#include <glut.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#else
#include <GL/glut.h>
#include <GL/glext.h>
#endif
#endif

#include <iostream>
#include <fstream>

typedef struct
{
} UserData;

#ifdef _GLES_EMU_
const char * texFile = "c:\\data\\base.bmp";
const char * texLFile = "c:\\data\\light.bmp";
#else
const char * texFile = "base.bmp";
const char * texLFile = "light.bmp";
#endif

GLuint programID;
GLuint pictureID;
GLuint lightID;

struct FileHeader
{
    unsigned short  mType;
    unsigned long   mSize;
    unsigned short  mReserved1;
    unsigned short  mReserved2;
    unsigned long   mOffsetbits;
}; 

struct InfoHeader
{
    unsigned long   mSize;
    unsigned long   mWidth;
    unsigned long   mHeight;
    unsigned short  mPlanes;
    unsigned short  mBitcount;
    unsigned long   mCompression;
    unsigned long   mSizeimage;
    long            mXpelspermeter;
    long            mYpelspermeter;
    unsigned long   mColorsused;
    unsigned long   mColorsimportant;
}; 

struct Pixel{
    unsigned char   mBlue;
    unsigned char   mGreen;
    unsigned char   mRed;
}; 


unsigned char * loadBMPData(const unsigned char * data, unsigned int size)
{
    FileHeader mFileHeader;
    InfoHeader mInfoHeader;
    unsigned char * mPixels;

    if (size < sizeof(mFileHeader) + sizeof(mInfoHeader)) {
        return 0;
    }

    if (data[0] != 'B' ||
        data[1] != 'M') {
        return 0;
    }

    int offset = 0;
    std::memcpy(&mFileHeader.mType, data + offset, sizeof(mFileHeader.mType));
    offset += sizeof(mFileHeader.mType);

    std::memcpy(&mFileHeader.mSize, data + offset, sizeof(mFileHeader.mSize));
    offset += sizeof(mFileHeader.mSize);

    std::memcpy(&mFileHeader.mReserved1, data + offset, sizeof(mFileHeader.mReserved1));
    offset += sizeof(mFileHeader.mReserved1);
    
    std::memcpy(&mFileHeader.mReserved2, data + offset, sizeof(mFileHeader.mReserved2));
    offset += sizeof(mFileHeader.mReserved2);
    
    std::memcpy(&mFileHeader.mOffsetbits, data + offset, sizeof(mFileHeader.mOffsetbits));
    offset += sizeof(mFileHeader.mOffsetbits);
    
    std::memcpy(&mInfoHeader.mSize, data + offset, sizeof(mInfoHeader.mSize));
    offset += sizeof(mInfoHeader.mSize);
    
    std::memcpy(&mInfoHeader.mWidth, data + offset, sizeof(mInfoHeader.mWidth));
    offset += sizeof(mInfoHeader.mWidth);
    
    std::memcpy(&mInfoHeader.mHeight, data + offset, sizeof(mInfoHeader.mHeight));
    offset += sizeof(mInfoHeader.mHeight);
    
    std::memcpy(&mInfoHeader.mPlanes, data + offset, sizeof(mInfoHeader.mPlanes));
	offset += sizeof(mInfoHeader.mPlanes);

    std::memcpy(&mInfoHeader.mBitcount, data + offset, sizeof(mInfoHeader.mBitcount));
	offset += sizeof(mInfoHeader.mBitcount);

    std::memcpy(&mInfoHeader.mCompression, data + offset, sizeof(mInfoHeader.mCompression));
	offset += sizeof(mInfoHeader.mCompression);

    std::memcpy(&mInfoHeader.mSizeimage, data + offset, sizeof(mInfoHeader.mSizeimage));
	offset += sizeof(mInfoHeader.mSizeimage);

    std::memcpy(&mInfoHeader.mXpelspermeter, data + offset, sizeof(mInfoHeader.mXpelspermeter));
	offset += sizeof(mInfoHeader.mXpelspermeter);

    std::memcpy(&mInfoHeader.mYpelspermeter, data + offset, sizeof(mInfoHeader.mYpelspermeter));
	offset += sizeof(mInfoHeader.mYpelspermeter);

    std::memcpy(&mInfoHeader.mColorsused, data + offset, sizeof(mInfoHeader.mColorsused));
	offset += sizeof(mInfoHeader.mColorsused);

    std::memcpy(&mInfoHeader.mColorsimportant, data + offset, sizeof(mInfoHeader.mColorsimportant));
	offset += sizeof(mInfoHeader.mColorsimportant);

    if (mInfoHeader.mCompression) {
        return false;
    }

    unsigned int dataSize = size - mFileHeader.mOffsetbits;
    mPixels = new unsigned char[dataSize];

    memcpy(mPixels, data + mFileHeader.mOffsetbits, dataSize);

    return mPixels;
}

unsigned char * loadBMPFile(const char * fileName)
{
    std::ifstream file(fileName, std::ios::in | std::ios::binary);    
    if (!file.is_open()) {
        return 0;
    }
    file.seekg (0, std::ios::end);
    unsigned int size = (unsigned int)file.tellg();
    unsigned char * data = new unsigned char[size];

    file.seekg (0, std::ios::beg);
    file.read ((char *)data, size);
    file.close();

    unsigned char * pixels = loadBMPData(data, size);

    delete [] data;

    return pixels;
}

GLuint loadShader(const char * src, GLenum type)
{
	GLuint sh = glCreateShader(type);

	if (sh) {
		glShaderSource(sh, 1, &src, 0);
		glCompileShader(sh);
		GLint compiled = 0;
		glGetShaderiv(sh, GL_COMPILE_STATUS, &compiled);
		if (!compiled) {
			std::cerr << "Shader Compilation Failed" << std::endl;
			GLint infoLen = 0;
			glGetShaderiv(sh, GL_INFO_LOG_LENGTH, &infoLen);
			if (infoLen > 1) {
				char * infoLog = new char[infoLen];
				glGetShaderInfoLog(sh, infoLen, NULL, infoLog);
                std::cerr << infoLog << std::endl;
				delete [] infoLog;
			}

			glDeleteShader(sh);
			sh = 0;
		}
	} else {
		std::cerr << "Shader Creation Failed" << std::endl;
	}

	return sh;
}

bool initShadders()
{
	bool status = true;
	const char * vshcode =  "attribute vec4 aPosition;   		                \n"
                            "attribute vec2 aTexCoords;                         \n"
                            "varying vec2 vTexCoords;                           \n"
				            "void main()                 		                \n"
                            "{                           		                \n"
                            "  gl_Position = aPosition;                         \n"
                            "  vTexCoords =  aTexCoords;                        \n"
                            "}                           		                \n";

	const char * fshcode =  "precision mediump float;    		                \n"
                            "uniform vec4 uColor;       	 	                \n"
                            "varying vec2 vTexCoords;                           \n"
                            "uniform sampler2D sTexture;                        \n"
                            "uniform sampler2D sLTexture;                       \n"
                            "void main()                 		                \n"
                            "{                           		                \n"
                            "  vec4 base = texture2D(sTexture, vTexCoords);     \n"
                            "  vec4 light = texture2D(sLTexture, vTexCoords);   \n"
                            "  gl_FragColor =  (uColor * 0.01) + base * (light+0.25);\n"
                            "                            		                \n"
                            "}                           		                \n";

	GLuint vsh = loadShader(vshcode, GL_VERTEX_SHADER);
	if (!vsh) {
		return false;
	}
	GLuint fsh = loadShader(fshcode, GL_FRAGMENT_SHADER);
	if (!fsh) {
		return false;
	}

	programID = glCreateProgram();
	if (programID) {
		glAttachShader(programID, vsh);
		glAttachShader(programID, fsh);
		glLinkProgram(programID);
		
		GLint linked = 0;
		glGetProgramiv(programID, GL_LINK_STATUS, &linked);
		if (!linked) {
			std::cerr << "Program Linking Failed" << std::endl;
			GLint infoLen = 0;
			glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLen);
			if (infoLen > 1) {
				char * infoLog = new char[infoLen];
				glGetProgramInfoLog(programID, infoLen, 0, infoLog);
				std::cerr << infoLog << std::endl;
				delete [] infoLog;
			}
			glDeleteProgram(programID);
			programID = 0;
			status = false;
		}

	} else {
		status = false;
	}

	glDeleteShader(vsh);
	glDeleteShader(fsh);

	return status;
}

bool cleanupShadders()
{
	return true;
}

bool initGL()
{
    unsigned char * pixels  = loadBMPFile(texFile);
    if (!pixels) {
        return false;
    }

    glGenTextures(1, &lightID);

    glGenTextures(1, &pictureID);
    glBindTexture(GL_TEXTURE_2D, pictureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1024, 1024, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    delete [] pixels;


    unsigned char * pixelsLight  = loadBMPFile(texLFile);
    if (!pixelsLight) {
        return false;
    }

    glGenTextures(1, &lightID);
    glBindTexture(GL_TEXTURE_2D, lightID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 512, 1024, 0, GL_RGB, GL_UNSIGNED_BYTE, pixelsLight);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    delete [] pixelsLight;

	if (!initShadders()) {
		std::cerr << "Shader Initialization Failed" << std::endl;
		return false;
	}

	glClearColor(1.0, 1.0, 1.0, 1.0);

	return true;
}

bool terminateGL()
{
	cleanupShadders();
	return true;
}

#ifdef _GLES_EMU_
void display(ESContext * esContext)
#else
void display()
#endif
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glUseProgram(programID);

	GLfloat rectangle[] =
			{
			-1.0f,  1.0f, 0.0f,
 			-1.0f, -1.0f, 0.0f,
			 1.0f, -1.0f, 0.0f,
			-1.0f,  1.0f, 0.0f,
			 1.0f, -1.0f, 0.0f,
			 1.0f,  1.0f, 0.0f,
			};

	GLfloat texCoords[] =
			{
			0.0f, 1.0f,
			0.0f, 0.0f,
			1.0f, 0.0f,
			0.0f, 1.0f,
			1.0f, 0.0f,
			1.0f, 1.0f
			};

	const char * posVar = "aPosition";
	const char * clrVar = "uColor";
    const char * texCoordVar = "aTexCoords";
    const char * samplerVar = "sTexture";
    const char * samplerLVar = "sLTexture";

	GLint posAddr =	glGetAttribLocation(programID, posVar);
	if (posAddr < 0) {
		std::cerr << "Retrieving Location Failed for " << posVar << std::endl;
	}

    GLint texCoordAddr = glGetAttribLocation(programID, texCoordVar);
	if (texCoordAddr < 0) {
		std::cerr << "Retrieving Location Failed for " <<  texCoordVar << std::endl;
	}

	GLint clrAddr = glGetUniformLocation(programID, clrVar);
	if (clrAddr < 0) {
		std::cerr << "Retrieving Location Failed for " <<  clrVar << std::endl;
	}

    GLint samplerAddr = glGetUniformLocation(programID, samplerVar);
	if (samplerAddr < 0) {
		std::cerr << "Retrieving Location Failed for " <<  samplerVar << std::endl;
	}

    GLint samplerLAddr = glGetUniformLocation(programID, samplerLVar);
	if (samplerLAddr < 0) {
		std::cerr << "Retrieving Location Failed for " <<  samplerLVar << std::endl;
	}

	glUniform4f(clrAddr, 1.0f, 0.0f, 1.0f, 1.0f);

    glVertexAttribPointer(posAddr, 3, GL_FLOAT, GL_FALSE, 0, rectangle);
	glEnableVertexAttribArray(posAddr);

	glVertexAttribPointer(texCoordAddr, 2, GL_FLOAT, GL_FALSE, 0, texCoords);
	glEnableVertexAttribArray(texCoordAddr);

    glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, pictureID);
	glUniform1i(samplerAddr, 0);

    glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, lightID);
	glUniform1i(samplerLAddr, 1);

    glDrawArrays(GL_TRIANGLES, 0, 6);

	glUseProgram(0);

#ifdef _GLES_EMU_
    eglSwapBuffers(esContext->eglDisplay, esContext->eglSurface);
#else
	glutSwapBuffers();
#endif
}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}

int main(int argc, char ** argv)
{
	const int WindowWidth = 480*2;
	const int WindowHeight = 360*2;

#ifndef _GLES_EMU_

#ifdef GLUT_ES2
    char *v[] = {"texture", "-renderer", "gles2"};
    int    c = sizeof(v) / sizeof(v[0]);
    glutInit(&c, v);
#else
    glutInit(&argc, argv);
#endif

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);

	glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH)-WindowWidth)/2,
			       (glutGet(GLUT_SCREEN_HEIGHT)-WindowHeight)/2);
	glutInitWindowSize(WindowWidth, WindowHeight);
	glutCreateWindow(argv[0]);

	//glutFullScreen();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	
	if (initGL()) {
		glutMainLoop();
        	terminateGL();
	} else {
		std::cerr << "Initialization Failed" << std::endl;
	}

#else

    UserData  userData;

    esInitContext ( &esContext );
    esContext.userData = &userData;

    esCreateWindow (&esContext, "Textures", WindowWidth, 
                   WindowHeight, ES_WINDOW_RGB | ES_WINDOW_ALPHA | ES_WINDOW_DEPTH);
   
    esRegisterDrawFunc(&esContext, display);

    if (!initGL()) {
        std::cerr << "Initialization Failed" << std::endl;
        return 1;
    }
 
    esMainLoop(&esContext);

    terminateGL();
#endif

	return 0;
}

