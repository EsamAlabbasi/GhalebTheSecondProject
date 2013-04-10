//
//  main.cpp
//  Ders 6
//
//  Created by Gurel Erceis on 3/17/13.
//  Copyright (c) 2013 Yogurt3D. All rights reserved.
//
// Thing to install and test before writing any code
// glm
// assimp (http://assimp.sourceforge.net/main_downloads.html)

#include <iostream>
#include <GL/glew.h>
#include "Scene.h"
#include <GLUT.h>
#include "ScreenSpaceShaderNode.h"
#include "NormalShaderNode.h"
#include "ColorFillNode.h"
#include "glm/glm.hpp"
#include "glm/ext.hpp"

#define WINDOW_TITLE_PREFIX "Ders 7"

void IdleFunction(void);
void ResizeFunction(int Width, int Height);
void setupScene();

unsigned FrameCount = 0;


int exitCounter;

int CurrentWidth = 800,
CurrentHeight = 600,
WindowHandle = 0, mouseX = 0, mouseY = 0;

SceneNode* pad1;
SceneNode* pad2;
SceneNode* ball;

Camera *camera;

Scene* scene;

float directionX = 1;
float directionY = 0.5;
float speed = 0.07;

static void Draw(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    
    scene->draw();
    
    glutSwapBuffers();
    FrameCount++;
}

static void Key(unsigned char key, int x, int y)
{
    printf("%d", key);
	switch (key) {
		case 27: // ESC
			exit(0);
            break;
		case 'a':
			if((pad1->getPosition().y) > -1.1)
			{
				pad1->translate(0, -35, 0);
			}
            break;
		case 'A': // ESC
			if((pad1->getPosition().y) > -1.1)
			{
				pad1->translate(0, -35, 0);
			}
            break;
		case 'q' :
			if((pad1->getPosition().y) < 1.1)
			{
				pad1->translate(0, 35, 0);
			}
            break;
		case 'Q': // ESC
			if((pad1->getPosition().y) < 1.1)
			{
				pad1->translate(0, 35, 0);
			}
            break;
		case 's':
			if((pad2->getPosition().y) > -1.1)
			{
				pad2->translate(0, -35, 0);
			}
            break;
			case 'S': // ESC
			if((pad2->getPosition().y) > -1.1)
			{
				pad2->translate(0, -35, 0);
			}
            break;
			case 'w':
			if((pad2->getPosition().y) < 1.1)
			{
				pad2->translate(0, 35, 0);
			}
            break;
			case 'W': // ESC
			if((pad2->getPosition().y) < 1.1)
			{
				pad2->translate(0, 35, 0);
			}
            break;
	}
}
void ResizeFunction(int Width, int Height)
{
	CurrentWidth = Width;
	CurrentHeight = Height;
	glViewport(0, 0, CurrentWidth, CurrentHeight);
}

static void timerCallback (int value)
{
    if (0 != value) {
		char* TempString = (char*)
        malloc(512 + strlen(WINDOW_TITLE_PREFIX));
        
		sprintf(
                TempString,
                "%s: %d Frames Per Second @ %d x %d",
                WINDOW_TITLE_PREFIX,
                FrameCount * 4,
                CurrentWidth,
                CurrentHeight
                );
        
		glutSetWindowTitle(TempString);
		free(TempString);
	}
	FrameCount = 0;
	glutTimerFunc(250, timerCallback, 1);
}


void IdleFunction(void)
{
    glm::mat4 ballMatrix = ball->getModelMatrix();
    float x = ball->getPosition().x;
    float y = ball->getPosition().y;
	float object_1_X = pad1->getPosition().x;
	float object_1_Y = pad1->getPosition().y;
	float object_2_X = pad2->getPosition().x;
	float object_2_Y = pad2->getPosition().y;
    
	if((x < (object_1_X+0.05) && x > (object_1_X-0.1)) && (y < (object_1_Y+0.1) && y > (object_1_Y-0.1) ))
	{
		directionX *= -1;
	}

	if((x < (object_2_X+0.1) && x > (object_2_X-0.1)) && (y < (object_2_Y+0.2) && y > (object_2_Y-0.2) ))
	{
		directionX *= -1;
	}

    if( y > 1.20 )
    {
        directionY *= -1;
    }
    if( y < -1.20 )
    {
        directionY *= -1;
    }



    if( x > 1.80 )
    {
			exit(0);
    }
    if( x < -1.80 )
    {
			exit(0);
    }

    //pad1->rotate(1, 0, 1, 0);
    ball->translate(directionX*speed, directionY*speed, 0);
	glutPostRedisplay();
}

void setupScene(){
    scene = new Scene();
    
    pad1 = new ColorFillNode("g.obj");
    
    pad2 = new ColorFillNode("g.obj");
    
    ball = new ColorFillNode("potiron02.obj");
    
    pad1->scale(0.001,0.001,0.001);
    pad2->scale(0.001,0.001,0.001);
	ball->scale(0.01,0.01,0.01);
    
    scene->addNode(pad1);
    scene->addNode(pad2);
    scene->addNode(ball);
    
    camera = new Camera();
    
    camera->translate(0.f, 0.f, 3.f);
    
    pad1->translate(-1600.f, 0.f, 0.f);
    
    pad2->translate( 1600.f, 0.f, 0.f);
    
    scene->setCamera(camera);
}

int main (int argc, char ** argv)
{
    exitCounter = 0;
    GLenum type;
    
	glutInit(&argc, argv);
	glutInitWindowSize(CurrentWidth,CurrentHeight);
	type = GLUT_RGB;
	type |= GLUT_DOUBLE;
	type |= GLUT_DEPTH;
    type |= GLUT_MULTISAMPLE;
	glutInitDisplayMode(type);
	glutCreateWindow("");

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
	  /* Problem: glewInit failed, something is seriously wrong. */
	  fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	}
	fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
    
    glClearColor(0.0, 0.0, 0.0, 0.0);
    // Z-Buffer i aciyoruz
    glEnable(GL_DEPTH_TEST);
    
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE); 
    
    const GLubyte* glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
    
    printf("GLSL Version: %s\n",glslVersion);
    
    const GLubyte* glVersion = glGetString(GL_VERSION);
    
    printf("GL Version: %s\n",glVersion);
       
    setupScene();
    
    timerCallback(0);
    glutReshapeFunc(ResizeFunction);
    glutDisplayFunc(Draw);
    glutKeyboardFunc(Key);
    glutIdleFunc(IdleFunction);
    glutMainLoop();
    
    return 0;
}


