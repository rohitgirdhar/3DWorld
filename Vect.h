/* 
 * File:   Vect.h
 * Author: rohit
 *
 * Created on January 24, 2012, 1:16 AM
 */

#ifndef VECT_H
#define	VECT_H
#include <cmath>
//a class to define Direction vectors

class Vect {
public:
    float x;
    float y;
    float z;
    float getMagnitude();
    float getAngle(Vect*);
    float dotProduct(Vect *other);
    Vect(float x, float y, float z);
    Vect();
    void rotate(float angle);           //rotate the vector anticlock by this angle in degrees, about the Y axis
};


#endif	/* VECT_H */

