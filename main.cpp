/* 
 * File:   main.cpp
 * Author: rohit
 *
 * Created on January 22, 2012, 1:33 AM
 */

#include <cstdlib>
#include <GL/glut.h>
#include <cmath>
#include <cstdio>
#include "car/Car.h"
#include "Track.h"
#include "WorldConstants.h"
#include "Timekeeper.h"
#include "Camera.h"
#include "Gallery.h"
#include "Lamppost.h"
#include "house/House.h"
#include <AL/al.h>
#include <AL/alut.h>
using namespace std;

/*
 * 
 */
Car *c;
Car *c2;
House *hs;
Track *tk;
Timekeeper *tkpr;
Camera *cam;
Gallery *gl;
Lamppost *lp;
//Lamppost *lp1;

GLfloat ambientColor[] = {1, 1, 1, 1.0f}; //Color(0.2, 0.2, 0.2) for ambient light
GLfloat lightColor0[] = {1.0f, 1.0f, 1.0f, 1.0f}; //Color (0.5, 0.5, 0.5)
GLfloat lightPos0[] = {100, 100, -200, 1.0f}; //Positioned at (4, 0, 8)

void checkTrackLimit(Car *c) {
//    printf(">>%f %f %f %f %f\n", c->posX, c->posY, c->getHeight(), tk->getHeight(c->posX, c->posZ));
    if(abs(c->getHeight() - tk->getHeight(c->posX, c->posZ)) > 5) {     //assuming the car is kept on track initially
        c->speed = -c->speed;
    }
}

int keyState[4] = {0};     //up down left right
int wasdKeyState[4] = {0};
void keyPress() {
    if(keyState[0] == 1) {
        c->speedUp(10);
    }
    if(keyState[1] == 1) {
        c->speedUp(-10);
    }
    if(keyState[2] == 1) {
        c->turn(6);
    }
    if(keyState[3] == 1) {
        c->turn(-6);
    }
		if(wasdKeyState[0] == 1) {
        c2->speedUp(10);
    }
		if(wasdKeyState[2] == 1) {
        c2->speedUp(-10);
    }
		if(wasdKeyState[1] == 1) {
        c2->turn(6);
    }
		if(wasdKeyState[3] == 1) {
        c2->turn(-6);
    }
}
/* Lighting Variables */
//GLfloat light_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
//GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat light_position_sun[] = {0, 0, 0, 1.0};
GLfloat light_specular_sun[] = { 1.0, 1.0, 1.0, 1.0 };

GLfloat light_ambient[] = {0.5,0.5,0.5,1.0};

GLfloat mat_ambient[] = { 0.7, 0.7, 0.7, 1.0 };
GLfloat mat_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat high_shininess[] = { 100.0 };

GLUquadricObj *sun;
void initSun() {
	sun = gluNewQuadric();
  gluQuadricNormals(sun, GLU_SMOOTH);
}

void drawSun() {
	glPushMatrix();
	glTranslatef(light_position_sun[0], light_position_sun[1], light_position_sun[2]);
//	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, light_specular_sun);
//	printf(">>%f %f %f\n", light_position_sun[0]);
	glColor3f(1,1,0);
	gluSphere(sun, 50, 32, 32);	
	glPopMatrix();
}

float sunX=0, sunY=0, sunZ=0, sunAngle = 0, sunRad;
void initLights() {
	//for the sun
	sunX = tk->scaleFac*tk->getTerrainWidth()/2;
	sunY = 0;
	sunZ = 0;
	sunAngle = 0;
	sunRad = 2*tk->scaleFac*tk->getTerrainLength()/2;
	initSun();
}

void moveSun(float d) {
	//the sun goes from 0,0 to terrain->width, terrain->length
	//The sun moves in a semicircle 
	sunAngle += d;
	if(sunAngle > 180) {
		sunAngle = 180;
	}
	if(sunAngle < 0) {
		sunAngle = 0;
	}
	//sunX is const
	sunY =	sunRad*sin(sunAngle*M_PI/180);
	sunZ = sunRad*(1-cos(sunAngle*M_PI/180));

	
}

void changeAmbient(float x) {
	light_ambient[0] += x;
	if(light_ambient[0] > 1) light_ambient[0] = 1;
	light_ambient[1] += x;
	if(light_ambient[1] > 1) light_ambient[1] = 1;
	light_ambient[2] += x;
	if(light_ambient[2] > 1) light_ambient[2] = 1;
	light_ambient[3] += x;
	if(light_ambient[3] > 1) light_ambient[3] = 1;

}

void placeSun() {
//	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
//glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	light_position_sun[0] = sunX;
	light_position_sun[1] = sunY;
	light_position_sun[2] = sunZ;
	drawSun();
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular_sun);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position_sun);

}

void placeAmbientLight() {
	glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_ambient);
	glLightfv(GL_LIGHT1, GL_POSITION, light_position_sun);
}


void display() {

   
//	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, light_ambient);
    
    //Add ambient light
//   glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
//	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
//	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
//	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
//		glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
//		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
//		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
//		glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
 
    
    //function to check car does not go beyond the track
    keyPress();
		placeAmbientLight();
    checkTrackLimit(c);
		checkTrackLimit(c2);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.9,1,1,1);
    glLoadIdentity();
    //gluLookAt( 0, 500, -400, ORIGIN_X, ORIGIN_Y,ORIGIN_Z,0,0,1);
    cam->placeCam();
    glPushMatrix();
    //glLoadIdentity();
    glTranslatef(ORIGIN_X,ORIGIN_Y,ORIGIN_Z);
		placeSun();
    gl->draw();
    tk->draw();
    c->draw();
    c2->draw();
		hs->draw();
		lp->draw();
	//	lp1->draw();
		
    glPopMatrix();
    glutSwapBuffers();
}



void initRendering(int width, int height) {
//    glClearColor(0,0,0,1);
	ALuint Buffer, Source;
	alutInit (0, NULL);
	Buffer = alutCreateBufferFromFile("audio/music.wav");
	alGenSources (1, &Source);
	alSourcei(Source, AL_BUFFER, Buffer);
	// alSourcei(Source, AL_LOOPING, AL_TRUE);
	alSourcePlay (Source);
    glEnable(GL_DEPTH_TEST);                    //important to enable depth
    
    //important to enable transparency
    glEnable(GL_BLEND);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
		glEnable(GL_LIGHT1);
		glEnable(GL_LIGHT2);
	
		// for car headlights and search lights
		glEnable(GL_LIGHT3);
		glEnable(GL_LIGHT4);
		glEnable(GL_LIGHT5);
		glEnable(GL_LIGHT6);
		glEnable(GL_LIGHT7);
		glEnable(GL_LIGHT0+8);
		/////////////////////
    glEnable(GL_NORMALIZE);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glMatrixMode(GL_PROJECTION);
    
    glLoadIdentity();
    
    gluPerspective(45,width*1.0/height, 0.5,5000);
    glMatrixMode(GL_MODELVIEW);

	   
    
}




void keyboard(int key, int, int) {
    if(key == GLUT_KEY_UP) {
        keyState[0] = 1;
    }
    if(key == GLUT_KEY_DOWN) {
        keyState[1] = 1;
    }
    if(key == GLUT_KEY_LEFT) {
        keyState[2] = 1;
    }
    if(key == GLUT_KEY_RIGHT) {
        keyState[3] = 1;
    }
}

void keyboardUp(int key, int, int) {
    if(key == GLUT_KEY_UP) {
        keyState[0] = 0;
    }
    if(key == GLUT_KEY_DOWN) {
        keyState[1] = 0;
    }
    if(key == GLUT_KEY_LEFT) {
        keyState[2] = 0;
    }
    if(key == GLUT_KEY_RIGHT) {
        keyState[3] = 0;
    }
}

void kbd(unsigned char key, int, int) {
    
    if(key == 'c') {
        cam->changeState();
    }
/*    if(key == 'a') {
        c->shiftBox(-1);
        
    }
    if(key == 'd') {
        c->shiftBox(1);
        
    }*/  
    if(key == 'f') {
        cam->changeFocus();
    }
		if(key == 'w') {
			wasdKeyState[0] = 1;
		}
		if(key == 'a') {
			wasdKeyState[1] = 1;
		}
		if(key == 's') {
			wasdKeyState[2] = 1;
		}
		if(key == 'd') {
			wasdKeyState[3] = 1;
		}
		//for the sun
		if(key == '1') {
			moveSun(5);
		}
		if(key == '2') {
			moveSun(-5);
		}
		if(key == '+') {
			changeAmbient(0.1);
		}
		if(key == '-') {
			changeAmbient(-0.1);
		}
		if(key == 't') {
			lp->toggleShade();
		}
		if(key == 'g') {
			lp->toggleReflect();
		}


}

void kbdUp(unsigned char key, int, int) {
		if(key == 'w') {
			wasdKeyState[0] = 0;
		}
		if(key == 'a') {
			wasdKeyState[1] = 0;
		}
		if(key == 's') {
			wasdKeyState[2] = 0;
		}
		if(key == 'd') {
			wasdKeyState[3] = 0;
		}

}

void mouse(int a, int b, int c, int d) {
    if(cam->state == HELI_VIEW) {
        cam->recordClick(a,b,c,d);
    }
}

void mouseMotion(int x, int y) {
    if(cam->state == HELI_VIEW) {
        cam->updateHeliView(x,y);
    }
}

int main(int argc, char** argv) {
    
    
    
	glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
    
    glutInitWindowSize(WINDOW_WIDTH,WINDOW_HEIGHT);
    glutInitWindowPosition(0,0);
    glutCreateWindow("3D World");
    glutDisplayFunc(display);
    glutIdleFunc(display);
    glutMouseFunc(mouse);
    glutMotionFunc(mouseMotion);
    glutSpecialFunc(keyboard);
    glutSpecialUpFunc(keyboardUp);
    glutKeyboardFunc(kbd);
		glutKeyboardUpFunc(kbdUp);
    initRendering(WINDOW_WIDTH,WINDOW_HEIGHT);
    
    
    
    tk = new Track(0,0);
    gl = new Gallery(tk);
		lp = new Lamppost(500,200,400);
//		lp1 = new Lamppost(5000, 200, 400);
    //note car x,y,z are the coordinates of the base, I translate up and then start building the car
    c2 = new Car("car/rapideNoTyre.obj", "car/wheel/Tyre.obj", 0.3, 0.42, 0.42, 500,0,330, GL_LIGHT3, GL_LIGHT4, GL_LIGHT7);
    c = new Car("car/ferrari/ferrariNoTyre.obj", "car/ferrari/tyre.obj", 0.38, 0.38, 0.45, 500,0,400, GL_LIGHT5, GL_LIGHT6, GL_LIGHT2);
    hs = new House("house/house.obj",400,2000,300);
    tkpr = new Timekeeper();
    cam = new Camera(c);

		initLights();

    glutMainLoop();
    

    return 0;
}



