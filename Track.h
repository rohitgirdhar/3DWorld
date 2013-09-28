/* 
 * File:   Track.h
 * Author: rohit
 *
 * Created on January 23, 2012, 7:03 PM
 */

#ifndef TRACK_H
#define	TRACK_H

#include <GL/glut.h>
#include "terrain/Terrain.h"

class Track {
    float centerX;
    float centerY;
    //assuming a general ellipse as the inner and outer boundaries
    float innerA;
    float innerB;
    float outerA;
    float outerB;
    
    GLuint _road;
    GLuint _grass;
    GLuint _texture_terrain;
    GLuint _displayListTerrain;
    
    Terrain *_terrain;
    
public:
    float scaleFac;
    Track(float, float);
    void draw();
    void getTextures();
    void makeTerrainList();
    float getHeight(float, float);              //return the terrain height at a given x, z point
    float getTerrainWidth();
    float getTerrainLength();
    
};


#endif	/* TRACK_H */

