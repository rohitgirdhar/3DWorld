#include "SteeringWheel.h"
#include <GL/glut.h>
#include <cmath>

extern void loadTexture(char*, GLuint*);
SteeringWheel::SteeringWheel() {
    rad = 1;
    getTextures();
}

void  SteeringWheel::getTextures() {
    loadTexture("textures/steer.bmp", &_steer);
}

void SteeringWheel::draw() {
    //asuming already translated to the position
    //to draw in the YZ plane
    //glPushMatrix();
    
    int i;
    float costheta, sintheta;
    glColor3f(1,1,1);
    glBindTexture(GL_TEXTURE_2D, _steer);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glBegin(GL_POLYGON);
    for(i=0; i<360; ++i) {
        
        costheta = cos(i*M_PI/180);
        sintheta = sin(i*M_PI/180);
        glTexCoord2f((costheta+1)/2, (sintheta+1)/2);
        glVertex3f(0,rad*costheta, rad*sintheta);
    }
    
    glEnd();
    
    //glPopMatrix();
}


