#include <GL/glut.h>
#include "House.h"      
#include "../car/glm/glm.h"



House::House(char *filename, float x, float z, float scal) {
    posX = x;
    posZ = z;
    
  	
		hmodel = glmReadOBJ(filename);
		glmUnitize(hmodel);
		glmVertexNormals(hmodel, 90.0, GL_TRUE);
		makeHouseDispList();
		scalef = scal;
}

void House::makeHouseDispList() {

//	glNewList(_displayListCar, GL_COMPILE);
	int mode = GLM_NONE;    

	mode = mode | GLM_SMOOTH;

	mode = mode | GLM_2_SIDED;

	mode = mode | GLM_MATERIAL;

	mode = mode | GLM_TEXTURE;



	_displayListHouse = glmList(hmodel, mode);
}



void House::draw() {

	glPushMatrix();
	
    glTranslatef(posX, 0, posZ);
		glTranslatef(0,150,0);
		
	if (hmodel) {
    //printf("Car coord: %f %f %f\n", posX, posY, posZ);
    //the car is drawn along the +X direction, so rotate according to its direction
    		glPushMatrix();
				
		glEnable(GL_TEXTURE_2D);
	

		glRotatef(90,0,1,0);

		glScalef(scalef,scalef,scalef);
//		glmDraw(pmodel, mode);

	glCallList(_displayListHouse);


		glDisable(GL_TEXTURE_2D);

		glPopMatrix();
	}

	glPopMatrix();
}



    
