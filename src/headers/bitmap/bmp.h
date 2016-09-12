#include <windows.h>

#ifndef BMP_23082016
#define BMP_23082016

struct Bmp{
	BITMAPFILEHEADER bitmapFileHeader;
	BITMAPINFOHEADER bitmapInfoHeader;
	
	int bitmapFileHeaderSize;
	int bitmapInfoHeaderSize;
	
	int width;
	int height;
	
	Bmp(int _width,int _height){
		
		width=_width;
		height=_height;
		
		bitmapFileHeaderSize = sizeof(bitmapFileHeader);
		bitmapInfoHeaderSize = sizeof(bitmapInfoHeader);
    
		memset( &bitmapFileHeader, 0, bitmapFileHeaderSize );
    	bitmapFileHeader.bfType = ( 'B' | 'M' << 8 );
    	bitmapFileHeader.bfOffBits = bitmapFileHeaderSize +bitmapInfoHeaderSize;
    	bitmapFileHeader.bfSize = bitmapFileHeader.bfOffBits + width * height * 3;
    
		memset( &bitmapInfoHeader, 0, bitmapInfoHeaderSize );
    	bitmapInfoHeader.biSize = bitmapInfoHeaderSize;
    	bitmapInfoHeader.biWidth = width;
    	bitmapInfoHeader.biHeight = height;
    	bitmapInfoHeader.biPlanes = 1;
    	bitmapInfoHeader.biBitCount = 24;
	}
};

#endif
