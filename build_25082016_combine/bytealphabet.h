#include <cstring>

#include "readbmp.h"

using namespace std;

#ifndef BYTEALPHABET_H_25082016
#define BYTEALPHABET_H_25082016

struct bytealphabet{
	int _len;
	int _letterlen;
	int _letterheight;
	unsigned char** alphabet;
	
	bytealphabet (const char* dict, const char*  font){
		
		_len = strlen(dict);
		alphabet = new unsigned char*[_len];
		
		for(int i = 0;i<_len;i++){
			
			string letter=font;
			letter+="\\";
			letter+=dict[i];
			letter+=".bmp";
			
			unsigned char* byteData = readBMP(letter.c_str(),&_letterlen,&_letterheight);//TODO: Обработка исколючения не нахождения файла буквы
			
			alphabet[i]= new unsigned char[_letterheight*_letterlen];
			makeLetter(byteData,i);
			
			delete byteData;
		}
	
	}
	
	~bytealphabet(){
		for(int i=0;i<_len;++i){
			delete alphabet[i];
		}
		delete alphabet;
	}
	
	void makeLetter(unsigned char* data, int num){
		for(int x=0;x<_letterlen;x++){
    		for(int y=0;y<_letterheight;y++){
				int pos = (x+y*_letterlen);
				alphabet[num][pos]=(data[pos*3]);	
			}
		}
	}
	
};

#endif
