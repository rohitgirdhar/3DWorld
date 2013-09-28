#include <GL/glut.h>
#include <cstdio>

void loadTexture(char *filename,GLuint *texture) {
	FILE *image;
	char *data;
	int width,height,j;
	char temp;
	if((image=fopen(filename,"rb"))==NULL)
	{
		fprintf(stderr,"Error: Texture file \"%s\" could not be read\n",filename);
		return;
	}
	fseek(image,18,SEEK_SET);
	if((fread(&width,4,1,image))!=1)
	{
		fprintf(stderr,"Error: Could not read image width\n");
		return;
	}
	if((fread(&height,4,1,image))!=1)
	{
		fprintf(stderr,"Error: Could not read image height\n");
		return;
	}
	if((data=(char *)malloc(3*width*height*sizeof(char)))==NULL)
	{
		fprintf(stderr,"Error: Not enough memory for texture\n");
		return;
	}
	fseek(image,54,SEEK_SET);
	if((fread(data,3*width*height,1,image))!=1)
	{
		fprintf(stderr,"Error: Image format seems not to be valid\n");
		return;
	}
	fclose(image);
	for(j=0; j<3*width*height; j=j+3)
	{
		temp=data[j];
		data[j]=data[j+2];
		data[j+2]=temp;
	}
	glGenTextures(1,texture);
	glBindTexture(GL_TEXTURE_2D,*texture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D,0,3,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,data);
}

