#include "Wheel.h"
#include <cmath>
#include "../../Timekeeper.h"
#include <cstdio>
#include "../glm/glm.h"
extern void loadTexture(char*, GLuint *);

extern long long tkpr;
Wheel::Wheel(char *tyreFile) {
	rad = 7;
	//getTextures();
	wheel_model = glmReadOBJ(tyreFile);
	glmUnitize(wheel_model);
	glmVertexNormals(wheel_model, 90.0, GL_TRUE);
	angle = 0;
    
    
    
}

void Wheel::getTextures() {
    loadTexture("textures/wheel.bmp", &_wheel);
}

void Wheel::draw(float timeElapsed, float speed) {
    
    //glPushMatrix();
    //assuming the current position where to make the wheel
    //since it is always drawn with the car, so we can have the angle to turn the wheel from the time elapsed and Velocity
    int i;
		

    float angleRotated = (10*speed*timeElapsed*180)/(M_PI*rad); //making the circle in XY plane

		angle += angleRotated;
		if(angle > 360) {
			angle = angle-360;
		}
		if(angle < 0) {
			angle += 360;
		}

//	 printf(">>angle = %f\n", timeElapsed);
   
    
    
    glPushMatrix();
		glColor3f(1,1,1);
		glRotatef(angle,1,0,0);

		int mode = GLM_NONE;    /* reset mode */

		mode = mode | GLM_SMOOTH;

		mode = mode | GLM_2_SIDED;

		mode = mode | GLM_MATERIAL;

		mode = mode | GLM_TEXTURE;

//		glTranslatef(0,20,0);
		glPushMatrix();
		glmUnitize(wheel_model);
		glScalef(18, 18,18);
		if (wheel_model) {
			glEnable(GL_TEXTURE_2D);
			glmDraw(wheel_model, mode);
			glDisable(GL_TEXTURE_2D);

		}
		glPopMatrix();


	


		/*
    glBindTexture(GL_TEXTURE_2D, _wheel);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glBegin(GL_POLYGON);
    
    for(i=0; i<360; ++i) {
        glTexCoord2f((cos((i)*M_PI/180)+1)/2, (sin((i)*M_PI/180)+1)/2);
        glVertex2f(rad*cos((i)*M_PI/180), rad*sin((i)*M_PI/180));
    }
    glEnd();*/
    glPopMatrix();
    //glRotatef(angleRotated,0,0,1);
    //glPopMatrix();
}
