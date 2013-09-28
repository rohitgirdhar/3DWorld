/* 
 * File:   Lamppost.h
 * Author: rohit
 *
 */

#ifndef LAMPPOST_H
#define	LAMPPOST_H

#include <GL/glut.h>
class Lamppost {
	float posX;
	float posZ;
	float height;
	float rad;
	GLuint shadeModel;
	GLuint reflectModel;

	GLUquadricObj *cyl;
	GLUquadricObj *sph;
	public:
	Lamppost(float, float, float);
	void draw();
	void toggleShade();
	void toggleReflect();
	
};


#endif	/* LAMPPOST_H */

