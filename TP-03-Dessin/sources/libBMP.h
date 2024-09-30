/*  LibBMP - a library for reading and writing bitmap data for uncompressed 24-bit BMP images.
    - Created by Paul Denny (August 2007) www.cs.auckland.ac.nz/enggen131s2c/lectures/
    - Deeply modifed and adapted by Luc Fabresse (November 2008)
*/
// SHOULD BE INTRODUCED:
// typedef struct {
	// short x, y;
// } Coordonnee;

#ifndef _LIBBMP_H
#define _LIBBMP_H

// typedef enum {FALSE, TRUE} boolean;
#include "mybasiclibrary.h"

/* Color values in the range 0-255 */
typedef unsigned char byte;

/* Color data for a single pixel */
typedef struct {
	byte red;
	byte green;
	byte blue;
} Color;

typedef struct {
	int width;
	int height;
	Color **pixels;
} Image;

/****
 * Gestion des couleurs
 ****/ 

boolean equalsColor(const Color c1,const Color c2);

extern const Color blackColor; 
extern const Color whiteColor; 
extern const Color redColor; 
extern const Color greyColor; 
extern const Color blueColor;
extern const Color greenColor;
extern const Color yellowColor;

Color createColorFromHSB(const double hue, const double saturation, const double brightness);
	
/****
 * Gestion des images
 ****/ 

/* return an allocated structure for an image of size width*height */
Image *CreateImage(int width,int height);

/* free all allocated space of image */
void DestroyImage(Image *image);

/* Load the image stored in the file named filename and return an image structure with all the data */
Image *LoadImage(char *filename);

/* set the pixel (x,y) to c */
void SetPixel(Image *image,int x,int y,Color c);

/* get the pixel value (x,y) */
Color GetPixel(Image *image,int x,int y);

int GetWidth(Image *image);
int GetHeight(Image *image);

/* Load the image stored in the file named filename and return an image structure with all the data */
void SaveImage(char *filename, Image *image);

/* A debug function that prints the image's characteristics */
void afficheImageInfos(Image *image);

/****
 * Primitives de dessin évoluées
 ****/

/* Draw a line with color c in image from (xi,yi) to (xf,yf). only use integer operations */
void ligneBresenham(Image *image,int xi,int yi,int xf,int yf,Color c);

/* Draw an ellispe in image */
void traceEllipse(Image *image,int xi,int yi,int a,int b,Color c);

#endif