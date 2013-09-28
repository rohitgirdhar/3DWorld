/* 
 * File:   Camera.h
 * Author: rohit
 *
 * Created on January 24, 2012, 12:18 AM
 */

#ifndef CAMERA_H
#define	CAMERA_H

#include "car/Car.h"
//defining camera states
#define TOTAL_STATES 8  //currently only 3 are available
#define DRIVER_VIEW 0
#define ROOF_TOP_VIEW 1
#define SLIDE_BOX_VIEW 2
#define SEARCH_LIGHT_VIEW 3
#define WHEEL_VIEW 4
#define OVERHEAD_VIEW 5
#define HELI_VIEW 6
#define TOP_VIEW 7

//for the focus
#define CAR 1
#define NONE 0  

#define MAX_HELI_HEIGHT 1500



class Camera {
    Car *c;                     //to have the location of the car
    
    
    float heliX;
    float heliY;
    float heliZ;
    float heliLookX;
    float heliLookY;
    float heliLookZ;
    int clickX;
    int clickY;
    int clickButton;
    int clickState;
    int focus;          //=CAR(1), if focus on car, =NONE(0) if not change focus by pressing 'f'
    
    
public:
    
    int state;                  //the camera view currently in
    Camera(Car*);
    void changeState();         //change view state (cyclic order)
    void placeCam();            //main camera click function
    
    //set the position of camera depending on the view
    
    void setDriverView();
    void setRoofTopView();
    void setSlideBoxView();
    void setWheelView();
    void setSearchLightView();
    void setHeliView();
    void setOverheadView();
    void topView();
    void updateHeliView(int, int);
    void recordClick(int, int, int, int);
    void changeFocus();
};

#endif	/* CAMERA_H */

