/* 
 * File:   SteeringWheel.h
 * Author: rohit
 *
 * Created on January 25, 2012, 1:34 AM
 */

#ifndef STEERINGWHEEL_H
#define	STEERINGWHEEL_H

#include <GL/glut.h>

class SteeringWheel {
    float rad;
    GLuint _steer;
public:
    SteeringWheel();
    void draw();
    void getTextures();
};


#endif	/* STEERINGWHEEL_H */

