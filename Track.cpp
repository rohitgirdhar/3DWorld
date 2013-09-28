#include <GL/glut.h>
#include <cmath>
#include "Track.h"
#include <cstdio>
#include "terrain/Terrain.h"
#include "terrain/vec3f.h"
extern void loadTexture(char*, GLuint*);
extern Terrain* loadTerrain(const char*, float);

Track::Track(float x, float y) {
    centerX = x;
    centerY = y;
    
    innerA = 800;
    innerB = 600;
    outerA = 1000;
    outerB = 700;
    
    scaleFac = 6;
    
    _terrain = loadTerrain("terrain/heightmap.bmp", 500);           //the height of the terrain
    _displayListTerrain = glGenLists(1);
   
    getTextures();
    
    
    
    makeTerrainList();
    
    
}

void Track::getTextures() {
   /* loadTexture("textures/road.bmp", &_road);
    loadTexture("textures/grass.bmp", &_grass);*/
    loadTexture("terrain/colormap.bmp", &_texture_terrain);
}

void Track::makeTerrainList() {
        glNewList(_displayListTerrain, GL_COMPILE);
    
    	//glTranslatef(-(float)(_terrain->width() - 1) / 2, 0.0f,-(float)(_terrain->length() - 1) / 2);
        glTranslatef(centerX, 0, centerY);
	glEnable(GL_TEXTURE_2D);

	glColor3f(1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, _texture_terrain);
	for(int z = 0; z < _terrain->length() - 1; z++) {
		//Makes OpenGL draw a triangle at every three consecutive vertices
		glBegin(GL_TRIANGLE_STRIP);
		for(int x = 0; x < _terrain->width(); x++) {
			Vec3f normal = _terrain->getNormal(x, z);
			glNormal3f(normal[0], normal[1], normal[2]);

			glTexCoord2f(x*1.0/_terrain->width(), z*1.0/_terrain->length());
			glVertex3f(x, _terrain->getHeight(x, z), z);
			normal = _terrain->getNormal(x, z + 1);
			glNormal3f(normal[0], normal[1], normal[2]);
			glTexCoord2f(x*1.0/_terrain->width(), (z+1)*1.0/_terrain->length());
			glVertex3f(x, _terrain->getHeight(x, z + 1), z + 1);
		}
		glEnd();
		
	}
	
	glDisable(GL_TEXTURE_2D);
    
        glEndList();
}

void Track::draw() {
    
    
    

    glPushMatrix();
    glScalef(scaleFac,1,scaleFac);
    glCallList(_displayListTerrain);
    glPopMatrix();
    
}

float Track::getHeight(float x, float z) {
    float temp = _terrain->getHeight((x-centerX)/scaleFac,(z-centerY)/scaleFac);
    //printf("%f %f %f\n", x, z, temp);
    return temp;
}

float Track::getTerrainWidth() {
    //returns the width of the terrain in X direction
    return _terrain->width();
}

float Track::getTerrainLength() {
    //returns the length of the terrain in Z direction
    return _terrain->length();
}
