#include "libBMP.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const Color blackColor = {0,0,0}; 
const Color whiteColor = {255,255,255}; 
const Color greyColor = {100,100,100}; 
const Color redColor = {255,0,0}; 
const Color blueColor = {0,0,255}; 
const Color greenColor = {0,255,0};
const Color yellowColor = {255,255,0};

/* Prototype declartions for input functions.  GetByte() returns a 1-byte sequence as an int,
   GetWord() returns a 2-byte sequence as an int, GetDoubleWord() returns a 4-byte sequence as an int.
   These functions assume little-endianness of file
*/
static int GetByte(FILE *fp);
static int GetWord(FILE *fp);
static int GetDoubleWord(FILE *fp);

boolean equalsColor(const Color c1,const Color c2) {
	return c1.red==c2.red && c1.green==c2.green && c1.blue==c2.blue;
}

void afficheImageInfos(Image *image) {
	printf("Image infos:\n");
	printf("   -  width = %i\n",GetWidth(image));
	printf("   -  height = %i\n",GetHeight(image));
	printf("\n");
}

Image *CreateImage(int width,int height) {
	int i;
	Image *image = (Image *) malloc(sizeof(Image));
	image->width = width;
	image->height = height;
	image->pixels = (Color**) calloc(width,sizeof(Color *));
	for(i=0;i<image->width;i++){
		image->pixels[i] = (Color *) calloc(image->height,sizeof(Color));
		memset(image->pixels[i],0,height*sizeof(Color));	
	}
	return image;
}

void DestroyImage(Image *image) {
	int i;
	for(i=0;i<image->width;i++)
		free(image->pixels[i]);
	free(image->pixels);
	free(image);
}

Image *LoadImage(char *filename)
// void LoadColourArray(char *filename, Image image, int *width, int *height)
{
	int width;
	int height;
	Image *image;
	
	/* For reading data from the file */
	int colourDepth, compression, fileSize;
	int x, y;

	/* Open BMP file */
	FILE *fptr;
	fptr = fopen(filename, "rb");
	if (fptr == NULL) {
		printf("Could not open input file: \"%s\"\n", filename);
		exit(EXIT_FAILURE);
	}

	/* Read file size */
	(void)fseek(fptr, 2, SEEK_SET);
	fileSize = GetDoubleWord(fptr);

	/* Read width and height of image */
	(void)fseek(fptr, 18, SEEK_SET);
	width = GetDoubleWord(fptr);
	(void)fseek(fptr, 22, SEEK_SET);
	height = GetDoubleWord(fptr);

	/* Read colour depth */
	(void)fseek(fptr, 28, SEEK_SET);
	colourDepth = GetWord(fptr);

	/* Read compression */
	(void)fseek(fptr, 30, SEEK_SET);
	compression = GetDoubleWord(fptr);

	/* Ensure a 24-bit, and therefore uncompressed BMP file */
	if ((colourDepth != 24) || (compression != 0)) {
		printf("Not a valid 24-bit BMP file.\n");
		printf("Colour depth should be 24 - it is %d.\n", colourDepth);
		printf("Compression should be 0 - it is %d.\n", compression);
		exit(EXIT_FAILURE);
	}

	/* Read the pixel information */
	(void)fseek(fptr, 54, SEEK_SET);

	image = CreateImage(width,height);

	for(y = height-1; y >= 0; y--) {
		for(x = 0; x < width; x++) {
			image->pixels[y][x].blue = (byte)GetByte(fptr);
			image->pixels[y][x].green = (byte)GetByte(fptr);
			image->pixels[y][x].red = (byte)GetByte(fptr);
		}
		/* Realign the file pointer, scan lines are word aliged */
		(void)fseek(fptr, width % 4, SEEK_CUR);
	}

	printf("Successfully loaded bitmap information:\n");
	printf("  - Filename: \"%s\"\n", filename);
	printf("  - Width: %d\n", width);
	printf("  - Height: %d\n", height);
	printf("  - Bytes read: %d\n\n", fileSize);
	
	return image;
}

/* set the pixel (x,y) to c */
void SetPixel(Image *image,int x,int y,Color c) {
	if(x>=0 && x<GetWidth(image) && y>=0 && y<GetHeight(image))
		image->pixels[y][x]=c;
}

Color GetPixel(Image *image,int x,int y) {
	return image->pixels[y][x];
}

int GetWidth(Image *image) {
	return image->width;
}

int GetHeight(Image *image) {
	return image->height;
}


void SaveImage(char *filename, Image *image)
{
	int width = image->width;
	int height = image->height;
	Color **bitmap = image->pixels;
	
	/* BMP file format header, for uncompressed 24-bit colour */
	byte header[54] = {	0x42,0x4D,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x36,0x00,0x00,0x00,0x28,0x00,
						0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x18,0x00,0x00,0x00,
						0x00,0x00,0x00,0x00,0x00,0x00,0xC2,0x1E,0x00,0x00,0xC2,0x1E,0x00,0x00,0x00,0x00,
						0x00,0x00,0x00,0x00,0x00,0x00 };

	FILE *fptr;
	byte* fileData;

	int aligned, padding, totalSize;
	int i;
	int x, y;
	int writeIndex = 54;
				
	/* Calculate total size of picture, taking into account padding */
	aligned = 4 - (width * 3) % 4;
	padding = (aligned == 4) ? 0 : aligned;
	totalSize = 54 + (((width * 3) + padding) * height);
	
	/* Allocate room to store filedata */
	fileData = (byte*)malloc((size_t)totalSize);
	if (fileData == NULL) {
		printf("Could not allocate sufficient memory.\n");
		exit(EXIT_FAILURE);
	}

	/* Initialise first 54 bytes, set total size and width and height correctly */
	for (i = 0; i < 54; i++) {
		fileData[i] = header[i];
	}
	/* File size */
	fileData[2] = (byte)(totalSize & 0xFF);
	fileData[3] = (byte)((totalSize >> 0x08) & 0xFF);
	fileData[4] = (byte)((totalSize >> 0x10) & 0xFF);
	fileData[5] = (byte)((totalSize >> 0x18) & 0xFF);
	/* Width */
	fileData[18] = (byte)(width & 0xFF);
	fileData[19] = (byte)((width >> 0x08) & 0xFF);
	fileData[20] = (byte)((width >> 0x10) & 0xFF);
	fileData[21] = (byte)((width >> 0x18) & 0xFF);
	/* Height */
	fileData[22] = (byte)(height & 0xFF);
	fileData[23] = (byte)((height >> 0x08) & 0xFF);
	fileData[24] = (byte)((height >> 0x10) & 0xFF);
	fileData[25] = (byte)((height >> 0x18) & 0xFF);
	
	/* Copy bitmap data */
	for(y=height-1; y >=0; y--) {
		for(x=0; x < width; x++) {
			fileData[writeIndex++] = bitmap[y][x].blue;
			fileData[writeIndex++] = bitmap[y][x].green;
			fileData[writeIndex++] = bitmap[y][x].red;
		}
		writeIndex += width%4;
	}

	/* Write to file */
	fptr = fopen(filename, "wb");
	if (fptr == NULL) {
		printf("Could not open output file: \"%s\"\n", filename);
		exit(EXIT_FAILURE);
	}

	/* Write data */
	(void)fwrite(fileData, 1, (size_t)totalSize, fptr);

	/* Close files */
	(void)fclose(fptr);

	/* Release memory */
	free(fileData);

	printf("Successfully saved bitmap information:\n");
	printf("  - Filename: \"%s\"\n", filename);
	printf("  - Width: %d\n", width);
	printf("  - Height: %d\n", height);
	printf("  - Bytes written: %d\n\n", totalSize);
}


static int GetByte(FILE *fp)
{
	int w;
	w =  (int) (fgetc(fp) & 0xFF);
	return(w);
}

static int GetWord(FILE *fp)
{
	int w;
	w =  (int) (fgetc(fp) & 0xFF);
	w |= ((int) (fgetc(fp) & 0xFF) << 0x08);
	return(w);
}

static int GetDoubleWord(FILE *fp)
{
	int dw;
	dw =  (int) (fgetc(fp) & 0xFF);
	dw |= ((int) (fgetc(fp) & 0xFF) << 0x08);
	dw |= ((int) (fgetc(fp) & 0xFF) << 0x10);
	dw |= ((int) (fgetc(fp) & 0xFF) << 0x18);
	return(dw);
}

void ligneBresenham(Image *image,int xi,int yi,int xf,int yf,Color c) {
	int dx,dy,i,xinc,yinc,cumul,x,y ;
	x = xi ;
	y = yi ;
	dx = xf - xi ;
	dy = yf - yi ;
	xinc = ( dx > 0 ) ? 1 : -1 ;
	yinc = ( dy > 0 ) ? 1 : -1 ;
	dx = abs(dx) ;
	dy = abs(dy) ;
	SetPixel(image,x,y,c);
	if ( dx > dy ) {
		cumul = dx / 2 ;
		for ( i = 1 ; i <= dx ; i++ ) {
			x += xinc ;
			cumul += dy ;
			if (cumul >= dx) {
				cumul -= dx ;
				y += yinc ;
			}
			SetPixel(image,x,y,c);
		} 
	}
	else {
		cumul = dy / 2 ;
		for ( i = 1 ; i <= dy ; i++ ) {
			y += yinc ;
			cumul += dx ;
			if ( cumul >= dy ) {
				cumul -= dy ;
				x += xinc ; 
			}
			SetPixel(image,x,y,c);
		} 
	}
}

void traceEllipse(Image *image,int xi,int yi,int a,int b,Color c) {
  int x,y ;
  double d1,d2 ;
  x = 0 ;
  y = b ;
  d1 = b*b - a*a*b + a*a/4 ;
  SetPixel(image,xi+x,yi+y,c) ;
  SetPixel(image,xi+x,yi-y,c) ;
  SetPixel(image,xi-x,yi-y,c) ;
  SetPixel(image,xi-x,yi+y,c) ;
	
  while ( a*a*(y-.5) > b*b*(x+1) ) {
    if ( d1 < 0 ) {
      d1 += b*b*(2*x+3) ;
      x++ ; }
      else {
      d1 += b*b*(2*x+3) + a*a*(-2*y+2) ;
      x++ ;
      y-- ; }
  SetPixel(image,xi+x,yi+y,c) ;
  SetPixel(image,xi+x,yi-y,c) ;
  SetPixel(image,xi-x,yi-y,c) ;
  SetPixel(image,xi-x,yi+y,c) ;
 }
  d2 = b*b*(x+.5)*(x+.5) + a*a*(y-1)*(y-1) - a*a*b*b ;
  while ( y > 0 ) {
    if ( d2 < 0 ) {
      d2 += b*b*(2*x+2) + a*a*(-2*y+3) ;
      y-- ;
      x++ ; }
      else {
      d2 += a*a*(-2*y+3) ;
      y-- ; }
  SetPixel(image,xi+x,yi+y,c) ;
  SetPixel(image,xi+x,yi-y,c) ;
  SetPixel(image,xi-x,yi-y,c) ;
  SetPixel(image,xi-x,yi+y,c) ;
 }
}



Color createColorFromHSB(double hue, double saturation, double brightness)
{   
	
	// int res.red, int res.green, int res.blue;
	// Color * res = calloc(1,sizeof(Color));
	Color res;
	 
    if (saturation == 0)
    {
        res.red = res.green = res.blue = brightness;
    }
    else
    {
        // the color wheel consists of 6 sectors. Figure out which sector you're in.
        double sectorPos = hue / 60.0;
        int sectorNumber = (int)(floor(sectorPos));
        // get the fractional part of the sector
        double fractionalSector = sectorPos - sectorNumber;

        // calculate values for the three axes of the color. 
        double p = brightness * (1.0 - saturation);
        double q = brightness * (1.0 - (saturation * fractionalSector));
        double t = brightness * (1.0 - (saturation * (1 - fractionalSector)));

        // assign the fractional colors to r, g, and b based on the sector the angle is in.
        switch (sectorNumber)
        {
            case 0:
                res.red = brightness;
                res.green = t;
                res.blue = p;
                break;
            case 1:
                res.red = q;
                res.green = brightness;
                res.blue = p;
                break;
            case 2:
                res.red = p;
                res.green = brightness;
                res.blue = t;
                break;
            case 3:
                res.red = p;
                res.green = q;
                res.blue = brightness;
                break;
            case 4:
                res.red = t;
                res.green = p;
                res.blue = brightness;
                break;
            case 5:
                res.red = brightness;
                res.green = p;
                res.blue = q;
                break;
        }
    }
	 return res;
}