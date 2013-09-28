/* 
 * File:   Car.h
 * Author: rohit
 *
 * Created on January 22, 2012, 1:34 AM
 */

#ifndef CAR_H
#define	CAR_H

#include <GL/glut.h>
#include "wheel/Wheel.h"
#include "../Vect.h"
#include "glm/glm.h"
#define MAX_SPEED 10000

class Car {
    public:
    float posX;
    float posY;
    float posZ;
    
		GLMmodel *pmodel;

    long long lastDrawTime;
    
    float lengthBase;
    float lengthTop;
    float heightBottom;
    float heightTop;
    float width;
    float wsAngle;              //windshield angle
    
    float speed;                //speed of the car
    Vect *dir;                   //direction of motion, as given by a vector
    float frontWheelAngle;          //the angle the car faces wrt +X axis in 2D plane, direction of motion
                                        //assumed to be positive, if car turning right, else negative

		float wheelFwdPosFac;
		float wheelBkdPosFac;
    Wheel *fl;
    Wheel *fr;
    Wheel *bl;
    Wheel *br;
    
		GLuint _displayListCar;
    
    float steerPosX;
    float steerPosY;
    float steerPosZ;
    
    float slideBoxPos;
    float heightBox;
    
    float searchLightAngle;
    float searchRad;            //the radius of the search light
    float searchHeight;
    float locSearchZ;            //the height at which to draw the light
    GLUquadricObj *search;
   
		//headlights, searchlight

		GLuint leftLight;
		GLuint rightLight;
		GLuint searchLight;

		GLfloat left_light_position[4];
		GLfloat right_light_position[4];
		GLfloat search_light_position[4];
		GLfloat head_light_direction[3];
		GLfloat search_light_direction[3];

		//textures
    
    GLuint _metal;
    GLuint _rear;
    GLuint _front;

    Car(char *, char *, float, float, float, float, float, float, GLuint, GLuint, GLuint);
    void draw();
    void getTextures();
    void updatePosition();
    
    void speedUp(float);
    void turn(float);
    int shiftBox(float);
    void autoShiftBox();
    void rotateSearch(float);
    void autoRotateSearch();
    float getHeight();

		void makeCarDispList();
		void setHeadLights();
		void setSearchLight();
    
};

#endif	/* CAR_H */

