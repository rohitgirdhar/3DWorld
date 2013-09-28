

#include "Vect.h"

float Vect::getMagnitude() {
        return sqrt(x*x + y*y + z*z);
    }
    
float Vect::dotProduct(Vect *other) {
        return (this->x*other->x + this->y*other->y + this->z*other->z);
    }
Vect::Vect(float x, float y, float z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }
Vect::Vect() {
        x=0;
        y=0;
        z=0;
    }

float Vect::getAngle(Vect *other) {
    float rads = acos(this->dotProduct(other)/((this->getMagnitude())*(other->getMagnitude())));
    return rads*180/M_PI;
}


void Vect::rotate(float angle) {
    //rotate the vector anticlock by this angle in degrees, about the Y axis
    float tempZ = this->z*cos(angle*M_PI/180) - this->x*sin(angle*M_PI/180);
    float tempX = this->z*sin(angle*M_PI/180) + this->x*cos(angle*M_PI/180);
    
    this->z = tempZ;
    this->x = tempX;
}