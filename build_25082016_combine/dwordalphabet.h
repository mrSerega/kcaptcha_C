#include <string>

#include "bytetodword.h"
#include "strtoint.h"
#include "readbmp.h"

#include "defines.h"

using namespace std;

#ifndef ALPHABET_23082016
#define ALPHABET_23082016

struct dwordalphabet{
public:
	int _len;
	int _lenOfLetter;
	int _heightOfLetter;
	dword** _alphabet;//TODO гуглить
	
	dwordalphabet(const char* dict, const char*  fontName){
		_len = strlen(dict);
		_alphabet = new dword*[_len];
		for(register int i=0;i<_len;i++){
			
			string letter=fontName;
			letter+="\\";
			letter+=dict[i];
			letter+=".bmp";
			
			
			unsigned char* byteData = readBMP(letter.c_str(),&_lenOfLetter,&_heightOfLetter);//TODO: Обработка исколючения не нахождения файла буквы	
			_alphabet[i] = byteToDword(byteData,_lenOfLetter*_heightOfLetter*3);	
			delete byteData;
		}
	}
	
	~dwordalphabet(){
		for(register int i=0;i<_len;i++){
			delete _alphabet[i];
		}
		delete _alphabet;
	}
	
};

#endif 
