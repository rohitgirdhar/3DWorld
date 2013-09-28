/* 
 * File:   Gallery.h
 * Author: rohit
 *
 * Created on February 5, 2012, 6:00 PM
 */

#ifndef GALLERY_H
#define	GALLERY_H

#include "Track.h"

class Gallery {
    Track *tk;
    float height;
    GLuint _gallery_texture;
public:
    Gallery(Track*);
    void draw();
    void getTextures();
};



#endif	/* GALLERY_H */

