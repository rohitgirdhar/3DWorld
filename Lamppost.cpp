#include "Lamppost.h"
#include <GL/glut.h>
Lamppost::Lamppost(float x, float z, float ht) {
	posX = x;
	posZ = z;
	height = ht;
	rad = ht/4;
	cyl = gluNewQuadric();
	gluQuadricNormals(cyl, GLU_SMOOTH);	
	sph = gluNewQuadric();
	gluQuadricNormals(sph, GLU_SMOOTH);
	shadeModel = GL_SMOOTH;
	reflectModel = GL_SPECULAR;
}

GLfloat shine[] = {100};
void Lamppost::toggleShade() {
	if(shadeModel == GL_SMOOTH) 
		shadeModel = GL_FLAT;
	else
		shadeModel = GL_SMOOTH;
}

void Lamppost::toggleReflect() {
	if(reflectModel == GL_SPECULAR)
		reflectModel = GL_DIFFUSE;
	else
		reflectModel = GL_SPECULAR;

	if(shine[0] == 3)
		shine[0] = 100;
	else
		shine[0] = 3;
}


GLfloat vals[] = {0.6,0.6,0.6,1};
void Lamppost::draw() {
	glPushMatrix();
	glShadeModel(shadeModel);
	glMaterialfv(GL_FRONT_AND_BACK,	reflectModel, vals);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shine);
	glTranslatef(posX, height+rad/2, posZ);
	gluSphere(sph, rad, 32, 32);
	glTranslatef(0, -height-rad/2, 0);
	glRotatef(90,-1,0,0);
	gluCylinder(cyl, 10, 9, height, 32, 32);
	glEnable(GL_COLOR_MATERIAL);
	glPopMatrix();
}
