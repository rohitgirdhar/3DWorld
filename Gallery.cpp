#include "Gallery.h"
#include "Track.h"

extern void loadTexture(char*, GLuint *);

Gallery::Gallery(Track *t) {
    tk = t;
    height = 1000;
    getTextures();
}

void Gallery::getTextures() {
    loadTexture("textures/gallery.bmp", &_gallery_texture);
}

void Gallery::draw() {
    //assuming at the ORIGIN point, drawing around the Track
    float len = tk->getTerrainLength();         //length along the +Z axis
    float wid = tk->getTerrainWidth();  //width along the X axis
    
    //draw the four sqaures as gallery
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, _gallery_texture);
    glPushMatrix();
    glScalef(tk->scaleFac,1,tk->scaleFac);
    glColor3f(1,1,1);
    glBegin(GL_QUADS);
    glTexCoord2i(0,0);
    glVertex3f(0,0,0);
    glTexCoord2i(1,0);
    glVertex3f(wid, 0, 0);
    glTexCoord2i(1,1);
    glVertex3f(wid,height,0);
    glTexCoord2i(0,1);
    glVertex3f(0,height, 0);
    glEnd();
    
    glBegin(GL_QUADS);
    glTexCoord2i(0,0);
    glVertex3f(0,0,0);
    glTexCoord2i(1,0);
    glVertex3f(0, 0, len);
    glTexCoord2i(1,1);
    glVertex3f(0,height,len);
    glTexCoord2i(0,1);
    glVertex3f(0,height, 0);
    glEnd();
    
    glBegin(GL_QUADS);
    glTexCoord2i(0,0);
    glVertex3f(wid,0,0);
    glTexCoord2i(1,0);
    glVertex3f(wid, 0, len);
    glTexCoord2i(1,1);
    glVertex3f(wid,height,len);
    glTexCoord2i(0,1);
    glVertex3f(wid,height, 0);
    glEnd();
    
    
    glBegin(GL_QUADS);
    glTexCoord2i(0,0);
    glVertex3f(0,0,len);
    glTexCoord2i(1,0);
    glVertex3f(wid, 0, len);
    glTexCoord2i(1,1);
    glVertex3f(wid,height,len);
    glTexCoord2i(0,1);
    glVertex3f(0,height, len);
    glEnd();
    glPopMatrix();
}
