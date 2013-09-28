#include "Car.h"
#include "../Timekeeper.h"
#include "../Vect.h"
#include <GL/glut.h>
#include <cmath>
#include <cstdio>
#include "wheel/Wheel.h"      
#include "glm/glm.h"

extern void loadTexture(char*, GLuint *);
extern Timekeeper tkpr;
Vect *X;


Car::Car(char *filename, char *tyreFilename, float widthRatio, float wheelFwdFac, float wheelBkdFac, float x, float y, float z, GLuint l, GLuint r, GLuint s) {
    posX = x;
    posY = y;
    posZ = z;
    
    leftLight = l;
		rightLight = r;
		searchLight = s;

		
		
		lengthBase = 150;
    lengthTop = lengthBase/3;
    heightBottom = lengthBase/6;
    heightTop = heightBottom;
    wsAngle = 35;
    width = lengthBase*widthRatio;
    
		wheelFwdPosFac = wheelFwdFac;
		wheelBkdPosFac = wheelBkdFac;
    
		// Defining the light positions
		left_light_position[0] = lengthBase/2+5;
		left_light_position[1] = 0;
		left_light_position[2] = -width/2;
		left_light_position[3] = 1;
		right_light_position[0] = lengthBase/2+5;
		right_light_position[1] = 0;
		right_light_position[2] = width/2;	
		right_light_position[3] = 1;	
		search_light_position[0] = 0;			//since search light called after translating to the point
		search_light_position[1] = 0;
		search_light_position[2] = 0;
		search_light_position[3] = 1;


		speed=0;
    dir = new Vect(1,0,0);              //facing the +X direction
    fl = new Wheel(tyreFilename);
    fr = new Wheel(tyreFilename);
    bl = new Wheel(tyreFilename);
    br = new Wheel(tyreFilename);
    frontWheelAngle = 0;
    
    heightBox = 2;      
    
    lastDrawTime = tkpr.getTime();
    slideBoxPos = 0;
    searchLightAngle = 0;
    searchRad = heightBox/2;
    searchHeight = 2;
    search = gluNewQuadric();
    gluQuadricNormals(search, GLU_SMOOTH);   // Create Smooth Normals ( NEW )
  	
		pmodel = glmReadOBJ(filename);
		glmUnitize(pmodel);
		glmVertexNormals(pmodel, 90.0, GL_TRUE);
    
    
    steerPosX = lengthTop/2.5;
    steerPosY = heightTop/4;
    steerPosZ = width/10;  
   
		X = new Vect(1,0,0);
               
    locSearchZ = heightTop+heightBox+searchHeight;
		makeCarDispList();
   // getTextures();
}

void Car::makeCarDispList() {

//	glNewList(_displayListCar, GL_COMPILE);
	int mode = GLM_NONE;    

	mode = mode | GLM_SMOOTH;

	mode = mode | GLM_2_SIDED;

	mode = mode | GLM_MATERIAL;

	mode = mode | GLM_TEXTURE;


	/*	glScalef(100,100,100);
			glRotatef(90,0,1,0);
			glmDraw(pmodel, mode);
			glmUnitize(pmodel);
	 */

	_displayListCar = glmList(pmodel, mode);
}

void Car::getTextures() {
    loadTexture("textures/rear.bmp", &_rear);
    loadTexture("textures/metal.bmp", &_metal);
    loadTexture("textures/front.bmp", &_front);
}

GLfloat diff[] = {0.8,0.8,0.8,1};
GLfloat cutoff[] = {45};

void Car::setHeadLights() {
	head_light_direction[0] = 1;
	head_light_direction[1] = 0;
	head_light_direction[2] = 0;
	//set the left headlight
	glLightfv(leftLight, GL_DIFFUSE, diff);
	glLightfv(leftLight, GL_SPOT_CUTOFF, cutoff);
	glLightfv(leftLight, GL_SPOT_DIRECTION, head_light_direction);
	glLightfv(leftLight, GL_POSITION, left_light_position);
	//set the right headlight
	glLightfv(rightLight, GL_DIFFUSE, diff);
	glLightfv(rightLight, GL_SPOT_CUTOFF, cutoff);
	glLightfv(rightLight, GL_SPOT_DIRECTION, head_light_direction);
	glLightfv(rightLight, GL_POSITION, right_light_position);

}

void Car::setSearchLight() {
	search_light_direction[2] = dir->z*cos(searchLightAngle*M_PI/180) - dir->x*sin(searchLightAngle*M_PI/180);
	search_light_direction[0] = dir->z*sin(searchLightAngle*M_PI/180) + dir->x*cos(searchLightAngle*M_PI/180);
	search_light_direction[1] = dir->y;
	
	glLightfv(searchLight, GL_DIFFUSE, diff);
	glLightfv(searchLight, GL_SPOT_CUTOFF, cutoff);
	glLightfv(searchLight, GL_SPOT_DIRECTION, search_light_direction);
	glLightfv(searchLight, GL_POSITION, search_light_position);

}

void Car::draw() {

		autoShiftBox();
    autoRotateSearch();


//	printf(">>now speed = %f\n", speed);
		updatePosition();
    glPushMatrix();
    glTranslatef(posX, posY, posZ);
		glTranslatef(0,33,0);
		
    float angleBetweenXandDir = dir->getAngle(X);    //with the X axis
    
    
    //a fraud to get the +/- angle wrt X axis
    if(dir->z >  0) {
        angleBetweenXandDir *= -1;
    }
    glRotatef(angleBetweenXandDir,0,1,0);


	if (pmodel) {
    //printf("Car coord: %f %f %f\n", posX, posY, posZ);
    //the car is drawn along the +X direction, so rotate according to its direction
    	int mode = GLM_NONE;    

		mode = mode | GLM_SMOOTH;

		mode = mode | GLM_2_SIDED;

		mode = mode | GLM_MATERIAL;

		mode = mode | GLM_TEXTURE;

    		glPushMatrix();
				
		glEnable(GL_TEXTURE_2D);
	
		setHeadLights();

		glRotatef(90,0,1,0);

		glScalef(100,100,100);
//		glmDraw(pmodel, mode);

	glCallList(_displayListCar);
		glmUnitize(pmodel);


		glDisable(GL_TEXTURE_2D);

		glPopMatrix();
	}



    
    //printf("%f %f %f\n", dir->x, dir->y, dir->z);
    
	/*
        
        //printf(">>>angel = %f\n" , angleBetweenXandDir);
    
    //drawing the roof
    glColor3f(1,1,1);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, _metal);
    glBegin(GL_QUADS);
    glTexCoord2i(0,0);
    glVertex3f(-lengthTop/2, heightTop, -width/2);
    glTexCoord2i(0,1);
    glVertex3f(lengthTop/2, heightTop, -width/2);
    glTexCoord2i(1,1);
    glVertex3f(lengthTop/2, heightTop, width/2);
    glTexCoord2i(1,0);
    glVertex3f(-lengthTop/2, heightTop, width/2);
    glEnd();
    
    glColor3f(0,1,1);
    //drawing the left side
    glBegin(GL_QUADS);
    glVertex3f(lengthTop/2, heightTop, -width/2);
    glVertex3f(-lengthTop/2, heightTop, -width/2);
    glVertex3f(-lengthTop/2 - heightTop*tan(wsAngle*M_PI/180), 0, -width/2);
    glVertex3f(lengthTop/2  + heightTop*tan(wsAngle*M_PI/180), 0, -width/2);
    glEnd();
    
   
    //drawing the right side
    glBegin(GL_QUADS);
    glVertex3f(lengthTop/2, heightTop, width/2);
    glVertex3f(-lengthTop/2, heightTop, width/2);
    glVertex3f(-lengthTop/2 - heightTop*(tan(wsAngle*M_PI/180)), 0, width/2);
    glVertex3f(lengthTop/2 + heightTop*tan(wsAngle*M_PI/180), 0, width/2);
    glEnd();
    
    
    glColor3f(1,1,1);
   //drawing the dickey top
    glBegin(GL_QUADS);
    glVertex3f(-lengthBase/2, 0, -width/2);
    glVertex3f(-lengthBase/2, 0,  width/2);
    glVertex3f(-lengthTop/2 - heightTop*(tan(wsAngle*M_PI/180)), 0, width/2);
    glVertex3f(-lengthTop/2 - heightTop*tan(wsAngle*M_PI/180), 0,  -width/2);
    glEnd();
    
    //drawing the bonnet
    glBindTexture(GL_TEXTURE_2D, _metal);
    glBegin(GL_QUADS);
    glTexCoord2i(0,0);
    glVertex3f(lengthBase/2, 0, -width/2);
    glTexCoord2i(0,1);
    glVertex3f(lengthBase/2, 0,  width/2);
    glTexCoord2i(1,1);
    glVertex3f(lengthTop/2 + heightTop*(tan(wsAngle*M_PI/180)), 0, width/2);
    glTexCoord2i(1,0);
    glVertex3f(lengthTop/2 + heightTop*tan(wsAngle*M_PI/180), 0,  -width/2);
    glEnd();
    
    //drawing the right side
    
    glBegin(GL_QUADS);
    glVertex3f(-lengthBase/2, 0, width/2);
    glVertex3f(-lengthBase/2, -heightBottom, width/2);
    glVertex3f(lengthBase/2, -heightBottom, width/2);
    glVertex3f(lengthBase/2, 0,  width/2);
    glEnd();
    
    glColor3f(0.5,0.5, 0.5);
    //drawing the right side
    glBegin(GL_QUADS);
    glVertex3f(-lengthBase/2, 0, -width/2);
    glVertex3f(-lengthBase/2, -heightBottom, -width/2);
    glVertex3f(lengthBase/2, -heightBottom, -width/2);
    glVertex3f(lengthBase/2, 0,  -width/2);
    glEnd();
    
    glColor3f(1,1,1);
    //drawing the front grille
    glBindTexture(GL_TEXTURE_2D, _front);
    glBegin(GL_QUADS);
    glTexCoord2i(0,1);
    glVertex3f(lengthBase/2, 0, width/2);
    glTexCoord2i(0,0);
    glVertex3f(lengthBase/2, -heightBottom, width/2);
    glTexCoord2i(1,0);
    glVertex3f(lengthBase/2, -heightBottom, -width/2);
    glTexCoord2i(1,1);
    glVertex3f(lengthBase/2, 0,  -width/2);
    glEnd();
    
    //drawing the rear
    glColor3f(1,1,1);
    glBindTexture(GL_TEXTURE_2D, _rear);
    glBegin(GL_QUADS);
    glTexCoord2i(1,1);
    glVertex3f(-lengthBase/2, 0, width/2);
    glTexCoord2i(1,0);
    glVertex3f(-lengthBase/2, -heightBottom, width/2);
    glTexCoord2i(0,0);
    glVertex3f(-lengthBase/2, -heightBottom, -width/2);
    glTexCoord2i(0,1);
    glVertex3f(-lengthBase/2, 0,  -width/2);
    glEnd();
    
    glColor4f(0,0,0,0.2);                               //slightly translucent wind shield 
    //draw the windshield
    glBegin(GL_QUADS);
    glVertex3f(lengthTop/2, heightTop, -width/2);
    glVertex3f(lengthTop/2, heightTop, width/2);
    glVertex3f(lengthTop/2 + heightTop*(tan(wsAngle*M_PI/180)), 0, width/2);
    glVertex3f(lengthTop/2 + heightTop*(tan(wsAngle*M_PI/180)), 0,  -width/2);
    glEnd();
    
    glColor4f(0,0,0,0.2);                               //slightly translucent wind shield 
    //draw the back windshield
    glBegin(GL_QUADS);
    glVertex3f(-lengthTop/2, heightTop, -width/2);
    glVertex3f(-lengthTop/2, heightTop, width/2);
    glVertex3f(-lengthTop/2 - heightTop*(tan(wsAngle*M_PI/180)), 0, width/2);
    glVertex3f(-lengthTop/2 - heightTop*(tan(wsAngle*M_PI/180)), 0,  -width/2);
    glEnd();
    
    
    
    
*/    
    long long now = tkpr.getTime();
		long long tE = (now - lastDrawTime);
    float timeElapsed = tE/1000000.0;
    //to make the wheels, I'll translate to the position
    //making the front right wheel
    glPushMatrix();
    glTranslatef(lengthBase*wheelFwdPosFac, -heightBottom/2, width/2+1);
		glRotatef(90,0,1,0);
    glRotatef(frontWheelAngle,0,1,0);
    fl->draw(timeElapsed, speed);
    glPopMatrix();
    
    //making the front left wheel
    glPushMatrix();
    glTranslatef(lengthBase*wheelFwdPosFac, -heightBottom/2, -width/2+1);
		glRotatef(-90,0,1,0);
    glRotatef(frontWheelAngle,0,1,0);
    
    fr->draw(timeElapsed, speed);
    glPopMatrix();
    
    //making the back right wheel
    glPushMatrix();
    glTranslatef(-lengthBase*wheelBkdPosFac, -heightBottom/2, width/2+1);
		glRotatef(90,0,1,0);
    
    
    br->draw(timeElapsed, speed);
    glPopMatrix();

    
    //making the back left wheel
    glPushMatrix();
    glTranslatef(-lengthBase*wheelBkdPosFac, -heightBottom/2, -width/2+1);
		glRotatef(-90,0,1,0);

    bl->draw(timeElapsed, speed);
    glPopMatrix();
    
    
  /*  
    
    //drawing the steering wheel
    
    glPushMatrix();
    
    glTranslatef(steerPosX, steerPosY, steerPosZ);    //right seat has the wheel
    glRotatef(-frontWheelAngle*2, 1,0,0);          //rotate as per front wheel angle in X axis
    st->draw();
    glPopMatrix();
    
    
    
    glDisable(GL_TEXTURE_2D);
    */
    //making the rails on top
    glColor3f(0,0,0);
    glBegin(GL_LINE);
    glVertex3f(heightBox/2, heightTop+1, -width/2);
    glVertex3f(heightBox/2, heightTop+1, +width/2);
    glVertex3f(-heightBox/2, heightTop+1, -width/2);
    glVertex3f(-heightBox/2, heightTop+1, +width/2);
    glEnd();
    //making the sliding box
    glBegin(GL_QUADS);
    glVertex3f(heightBox/2, heightTop+1, slideBoxPos - heightBox/2);
    glVertex3f(-heightBox/2, heightTop+1, slideBoxPos - heightBox/2);
    glVertex3f(-heightBox/2, heightTop+1, slideBoxPos + heightBox/2);
    glVertex3f(heightBox/2, heightTop+1, slideBoxPos + heightBox/2);
    glEnd();
    glBegin(GL_QUADS);
    glVertex3f(heightBox/2, heightTop+heightBox, slideBoxPos - heightBox/2);
    glVertex3f(-heightBox/2, heightTop+heightBox, slideBoxPos - heightBox/2);
    glVertex3f(-heightBox/2, heightTop+heightBox, slideBoxPos + heightBox/2);
    glVertex3f(heightBox/2, heightTop+heightBox, slideBoxPos + heightBox/2);
    glEnd();

    glBegin(GL_QUADS);
    glVertex3f(heightBox/2, heightTop+heightBox, slideBoxPos - heightBox/2);
    glVertex3f(-heightBox/2, heightTop+heightBox, slideBoxPos - heightBox/2);
    glVertex3f(-heightBox/2, heightTop+1, slideBoxPos - heightBox/2);
    glVertex3f(heightBox/2, heightTop+1, slideBoxPos - heightBox/2);
    glEnd();
    
    glBegin(GL_QUADS);
    glVertex3f(heightBox/2, heightTop+heightBox, slideBoxPos + heightBox/2);
    glVertex3f(-heightBox/2, heightTop+heightBox, slideBoxPos + heightBox/2);
    glVertex3f(-heightBox/2, heightTop+1, slideBoxPos + heightBox/2);
    glVertex3f(heightBox/2, heightTop+1, slideBoxPos + heightBox/2);
    glEnd();
    
    glBegin(GL_QUADS);
    glVertex3f(heightBox/2, heightTop+heightBox, slideBoxPos + heightBox/2);
    glVertex3f(heightBox/2, heightTop+heightBox, slideBoxPos - heightBox/2);
    glVertex3f(heightBox/2, heightTop+1, slideBoxPos - heightBox/2);
    glVertex3f(heightBox/2, heightTop+1, slideBoxPos + heightBox/2);
    glEnd();
    
    glBegin(GL_QUADS);
    glVertex3f(-heightBox/2, heightTop+heightBox, slideBoxPos + heightBox/2);
    glVertex3f(-heightBox/2, heightTop+heightBox, slideBoxPos - heightBox/2);
    glVertex3f(-heightBox/2, heightTop+1, slideBoxPos - heightBox/2);
    glVertex3f(-heightBox/2, heightTop+1, slideBoxPos + heightBox/2);
    glEnd();
    
    //make the search light
    //force the light to be at angle from the look vector
    
    glPushMatrix();
    glColor3f(1,0,0);
    
    glTranslatef(0, locSearchZ, slideBoxPos);
		setSearchLight();
    //glRotatef(90,0,0,1);
    //printf(">>%f\n", searchLightAngle);
    glRotatef(searchLightAngle,0,1,0);
    
    gluCylinder(search, searchRad/2, searchRad, searchHeight, 32, 32);
    glPopMatrix();
   	 
    
    glPopMatrix();
    
    
}

int baseRad;
float tempX;
float tempZ;
float tempAngle;
float turningRad;
float angle;
float mag;			//some global declarations to make the next function fast
void Car::updatePosition() {
    
    
    
    long long tE = (tkpr.getTime() - lastDrawTime);
    float timeElapsed = tE/1000000.0;
    
    lastDrawTime += tE;
    
    //apply friction, reduce velocity
    speed -= timeElapsed*speed*0.3;
    //move the car along its direction of motion
  //  float mag = dir->getMagnitude();
    
    
    //rotate the car if the wheel is rotated assuming car on XZ plane, Y axis going up
    //check
    
    //find the angle the car turns by in circle
    
    //turning radius depends on the angle of wheels
    
    if(abs(frontWheelAngle) <= 5) {
        frontWheelAngle = 0;
    }

		tempX = dir->x;
		tempZ = dir->z;

//		if(frontWheelAngle != 0) {
		baseRad = lengthBase*30;
    
    turningRad = (baseRad/frontWheelAngle);
    angle = 180*timeElapsed*speed/(M_PI*turningRad);
      
    tempX = dir->x*cos(angle*M_PI/180) + dir->z*sin(angle*M_PI/180);
    tempZ = -dir->x*sin(angle*M_PI/180) + dir->z*cos(angle*M_PI/180);
//		}
    mag = sqrt(tempX*tempX + dir->y*dir->y + tempZ*tempZ);
    posX += (speed*tempX/mag)*timeElapsed;
    posY += (speed*dir->y/mag)*timeElapsed;
    posZ += (speed*tempZ/mag)*timeElapsed;
    
    
    
    tempAngle = frontWheelAngle;
    
    
    if(speed > 0) {
        frontWheelAngle = frontWheelAngle-angle;             //so that the angle slowly changes restores
    }
    else {
        frontWheelAngle += angle;
    }
    
    if(frontWheelAngle*tempAngle < 0) {
        //the angle went beyond
        frontWheelAngle = 0;
    }
    
    
    dir->x = tempX;
    dir->z = tempZ;
    
    
    
}

void Car::speedUp(float val) {
    speed += val;
    if(speed >= MAX_SPEED) {
        speed = MAX_SPEED;
    }
    if(speed < -MAX_SPEED) {
        speed = -MAX_SPEED;
    }
}


void Car::turn(float val) {
    frontWheelAngle += val;
    if(frontWheelAngle > 45) {
        frontWheelAngle = 45;
    }
    if(frontWheelAngle < -45) {
        frontWheelAngle = -45;
    }
}

int Car::shiftBox(float val) {
    slideBoxPos += val;
    if(slideBoxPos < -width/2) {
        slideBoxPos = -width/2;
        return 0;
        
    }
    if(slideBoxPos > width/2) {
        slideBoxPos = width/2;
        return 0;
    }
    return 1;
}

void Car::autoShiftBox() {
    static float val = 2;
    if(!shiftBox(val)) 
        val = -val;
    
    
}

void Car::rotateSearch(float val) {
    searchLightAngle += val;
    if(searchLightAngle > 360) {
        searchLightAngle -= 360;
    }
}

void Car::autoRotateSearch() {
    rotateSearch(1);
    
}

float Car::getHeight() {             //to return the height of the lowermost point of car
    
    return 0;           //since the car is defined to be touching ground
}
