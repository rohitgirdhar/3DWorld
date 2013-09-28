#include "Camera.h"
#include "car/Car.h"
#include "WorldConstants.h"
#include <GL/glut.h>
#include <cstdio>

Camera::Camera(Car *cr) {
    c = cr;
    state = DRIVER_VIEW;
    
    heliX = c->posX + ORIGIN_X;
    heliY = c->posY + ORIGIN_Y + 1000;
    heliZ = c->posZ + ORIGIN_Z + 50;
    focus = CAR;                //default focus
    
}

void Camera::changeState() {
    state += 1;
    state %= TOTAL_STATES;
}

void Camera::placeCam() {
    
    if(focus == CAR) {
    //these need to be revised, as the car moves
        heliLookX = c->posX + ORIGIN_X;
        heliLookY = c->posY + ORIGIN_Y;
        heliLookZ = c->posZ + ORIGIN_Z;
    }
    
    switch(state) {
        case DRIVER_VIEW: setDriverView(); break;
        case ROOF_TOP_VIEW: setRoofTopView(); break;
        case SLIDE_BOX_VIEW: setSlideBoxView(); break;
        case SEARCH_LIGHT_VIEW: setSearchLightView(); break;
        case WHEEL_VIEW: setWheelView(); break;
        case OVERHEAD_VIEW: setOverheadView(); break;
        case HELI_VIEW: setHeliView(); break;
        case TOP_VIEW: topView(); break;
 
               
    }
}

void Camera::setRoofTopView() {
    
    float camX = c->posX + ORIGIN_X;
    float camY = c->posY + ORIGIN_Y + c->heightBottom + c->heightTop + c->bl->rad;
    float camZ = c->posZ + ORIGIN_Z;
    // the vector perpendicular to the look vector in clockwise dir can be given by: (in YZ plane)

    //now a point at distance slideBoxPos in the negative direction of this vector
    
    float mod = c->dir->getMagnitude();
    
    camX += c->dir->x*5/mod;            //shift the camera slightly in front of the box
    camY += c->dir->y*5/mod;
    camZ += c->dir->z*5/mod;
    
    //now look at a point in front
    float lookX = camX + c->dir->x*c->lengthBase/mod;
    float lookY = camY + c->dir->y*c->lengthBase/mod;
    float lookZ = camZ + c->dir->z*c->lengthBase/mod;
    
    gluLookAt(camX, camY, camZ, lookX, lookY, lookZ, 0, 1, 0);
}


void Camera::setSearchLightView() {
    //the slider is exactly above the center of the car
    float camX = c->posX + ORIGIN_X;
    float camY = c->posY + ORIGIN_Y + c->heightBottom + c->heightTop + c->bl->rad + c->heightBox + c->searchHeight;
    float camZ = c->posZ + ORIGIN_Z;
    // the vector perpendicular to the look vector in clockwise dir can be given by: (in YZ plane)
    Vect *oppV = new Vect(-c->dir->z, c->dir->y, c->dir->x);
    //now a point at distance slideBoxPos in the negative direction of this vector
    
    float mod = oppV->getMagnitude();
    
    camX += oppV->x*c->slideBoxPos/mod;
    camY += oppV->y*c->slideBoxPos/mod;
    camZ += oppV->z*c->slideBoxPos/mod;
    
    //now to find the look vector, just rotate the look vector of car by the angle of the search light
    Vect temp(0,0,0);
    //just had to negate angle to match the light....
    temp.z = c->dir->z*cos((c->searchLightAngle-90)*M_PI/180) - c->dir->x*sin((c->searchLightAngle-90)*M_PI/180);
    temp.x = c->dir->z*sin((c->searchLightAngle-90)*M_PI/180) + c->dir->x*cos((c->searchLightAngle-90)*M_PI/180);
    temp.y = c->dir->y;
    //printf(">>looking vector: %f %f %f\n", temp.x, temp.y, temp.z);
    mod = temp.getMagnitude();
    float lookX = camX + temp.x*c->lengthBase/mod;
    float lookY = camY + temp.y*c->lengthBase/mod;
    float lookZ = camZ + temp.z*c->lengthBase/mod;
		delete oppV;
    gluLookAt(camX, camY, camZ, lookX, lookY, lookZ, 0, 1, 0);
    
    
}

void Camera::setSlideBoxView() {
    //the slider is exactly above the center of the car
    float camX = c->posX + ORIGIN_X;
    float camY = c->posY + ORIGIN_Y + c->heightBottom + c->heightTop + c->bl->rad + c->heightBox/2;
    float camZ = c->posZ + ORIGIN_Z;
    // the vector perpendicular to the look vector in clockwise dir can be given by: (in YZ plane)
    Vect *oppV = new Vect(-c->dir->z, c->dir->y, c->dir->x);
    //now a point at distance slideBoxPos in the negative direction of this vector
    
    float mod = oppV->getMagnitude();
    
    camX += oppV->x*c->slideBoxPos/mod;
    camY += oppV->y*c->slideBoxPos/mod;
    camZ += oppV->z*c->slideBoxPos/mod;
    
    //move the camera slightly in front ahead of the box
    
    mod = c->dir->getMagnitude();
    float camX1 = camX + 2*c->heightBox*c->dir->x/mod;
    float camY1 = camY + 2*c->heightBox*c->dir->y/mod;
    float camZ1 = camZ + 2*c->heightBox*c->dir->z/mod;
    //now look at a point in front
    float d = c->lengthBase;
    
    float lookX = camX + c->dir->x*d/mod;
    float lookY = camY + c->dir->y*d/mod;
    float lookZ = camZ + c->dir->z*d/mod;
    
		delete oppV;
    gluLookAt(camX1, camY1, camZ1, lookX, lookY, lookZ, 0, 1, 0);
    
    
}

void Camera::setOverheadView() {
    float carX = c->posX + ORIGIN_X;
    float carY = c->posY + ORIGIN_Y;
    float carZ = c->posZ + ORIGIN_Z;
    
    //the camera must be some distance behind in dir vector
    float mod = c->dir->getMagnitude();
    float camX = carX - 2*c->dir->x*c->lengthBase/mod;
    float camY = carY - 2*c->dir->y*c->lengthBase/mod;
    float camZ = carZ - 2*c->dir->z*c->lengthBase/mod;
    
    //raise the camera
    camY += c->heightBottom + c->heightTop + 25 ;
    
    gluLookAt(camX, camY, camZ, carX, carY, carZ, 0, 1, 0);
}

void Camera::setDriverView() {
    //get the driver position from the car, looking in the direction of motion of car
    //up vector always +Y axis
    
    //the x, y, z coord of the car are the center of the car, I need to translate it appropriately
    //find the look point, a point in the direction of the dir vector of the car
    
    float camX = c->posX + ORIGIN_X;
    float camY = c->posY + ORIGIN_Y + c->heightBottom + c->fl->rad+5;
    float camZ = c->posZ + ORIGIN_Z;
    
    //the camera must see some position to its front
    float mod = c->dir->getMagnitude();
    float lookX = camX + c->dir->x*c->lengthBase/mod;
    float lookY = camY + c->dir->y*c->lengthBase/mod;
    float lookZ = camZ + c->dir->z*c->lengthBase/mod;
    
    
    
    
    gluLookAt(camX, camY, camZ, lookX, lookY, lookZ, 0, 1, 0);
    
    
   
}

void Camera::topView() {
    gluLookAt( ORIGIN_X+200, 1800, ORIGIN_Z+400, ORIGIN_X, ORIGIN_Y,ORIGIN_Z,0,0,1);
}


void Camera::setHeliView() {
    gluLookAt(heliX, heliY, heliZ, heliLookX, heliLookY, heliLookZ, 0, 1, 0);
}
void Camera::updateHeliView(int x, int y) {
    
    if(clickButton == GLUT_LEFT_BUTTON && clickState == GLUT_DOWN) {
        //move the camera left/right/up/down as per the value
        //to move on horizontal plane, I need the vector perpendicular to the current look vector
        Vect *perp = new Vect(-(heliLookZ- heliZ), heliLookY - heliY, heliLookX - heliX);
        int dx = (x-clickX)*perp->x/perp->getMagnitude();              //positive if moving right
        int dz = (x-clickX)*perp->z/perp->getMagnitude();
        int dy = y-clickY;              //positive if moving down
        heliX += dx;
        heliY -= dy;
        heliZ += dz;
        if(heliY < 0) {
            heliY = 0;
        }
        if(heliY > MAX_HELI_HEIGHT) {
            heliY = MAX_HELI_HEIGHT;
        }
				delete perp;
    }
    if(clickButton == GLUT_RIGHT_BUTTON && clickState == GLUT_DOWN) {
        
        //move the camera left/right/up/down as per the value
        //to move on horizontal plane, I need the vector perpendicular to the current look vector
        Vect *perp = new Vect(-(heliLookZ- heliZ), heliLookY - heliY, heliLookX - heliX);
        int dx = (x-clickX)*perp->x/perp->getMagnitude();              //positive if moving right
        int dz = (x-clickX)*perp->z/perp->getMagnitude();
        int dy = y-clickY;              //positive if moving down
        heliLookX += dx;
        heliLookY -= dy;
        heliLookZ += dz;
        if(heliY < 0) {
            heliY = 0;
        }
        if(heliY > MAX_HELI_HEIGHT) {
            heliY = MAX_HELI_HEIGHT;
        }

				delete perp;
    }
}

void Camera::recordClick(int button, int state, int x, int y) {
    clickButton = button;
    clickState = state;
    clickX = x;
    clickY = y;
    
   
    
    //in case of mouse scroll up/down, just zoom in, ie move camera in direction of the look vector
    if (clickButton == 3 || clickButton == 4) {
        Vect *look = new Vect(heliLookX - heliX, heliLookY - heliY, heliLookZ - heliZ);
        float mag = 10;
        if (clickButton == 4) {//down scroll 
            mag = -mag;
        }

        float mod = look->getMagnitude();
        heliX += mag * look->x / mod;
        heliY += mag * look->y / mod;
        heliZ += mag * look->z / mod;

				delete look;
    }
}

void Camera::changeFocus() {
    focus +=1;
    focus %= 2;
}


void Camera::setWheelView() {
    float camX = c->posX + ORIGIN_X;
    float camY = c->posY + ORIGIN_Y;
    float camZ = c->posZ + ORIGIN_Z;
    
    //now rotate the look vector to get the vector about the wheel direction
    Vect *temp = new Vect(c->dir->x, c->dir->y, c->dir->z);
    temp->rotate(c->frontWheelAngle);    //rotate the camera looking vector by this angle
    float d = c->lengthBase;
    float mod = temp->getMagnitude();
    float lookX = camX + temp->x*d/mod;
    float lookY = camY + temp->y*d/mod;
    float lookZ = camZ + temp->z*d/mod;
    
    delete temp;
    gluLookAt(camX, camY, camZ, lookX, lookY, lookZ, 0, 1, 0);

}
