#include <fstream>

#include "bmp.h"

#ifndef CREATEBITMAP_H_23082016
#define CREATEBITMAP_H_23082016

///создает Bmp файл
/**
/param[in] imageData - байтовое представление изображения
/param[in] filename - имя файла
/param[in] _bmp - класс для получения структурной информации для генерации bmp файла
*/

void createBitmap( byte * imageData, const char * filename, Bmp* _bmp){
    std::ofstream file( filename, std::fstream::binary );

    file.write( reinterpret_cast< char * >( &(_bmp->bitmapFileHeader)), _bmp->bitmapFileHeaderSize );
    file.write( reinterpret_cast< char * >( &(_bmp->bitmapInfoHeader) ), _bmp->bitmapInfoHeaderSize );
    file.write( (const char*)imageData, _bmp->width * _bmp->height * 3 );
}

#endif
