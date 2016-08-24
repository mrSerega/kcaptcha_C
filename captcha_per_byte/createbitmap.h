#include <fstream>

#include "bmp.h"

#ifndef CREATEBITMAP_H_23082016
#define CREATEBITMAP_H_23082016

void createBitmap( byte * imageData, const char * filename, bmp* _bmp){
    std::ofstream file( filename, std::fstream::binary );

    file.write( reinterpret_cast< char * >( &(_bmp->bitmapFileHeader)), _bmp->bitmapFileHeaderSize );
    file.write( reinterpret_cast< char * >( &(_bmp->bitmapInfoHeader) ), _bmp->bitmapInfoHeaderSize );
    file.write( (const char*)imageData, _bmp->width * _bmp->height * 3 );
}

#endif
