#include <string>
#include <fstream>

#include "bytetodword.h"
#include "strtoint.h"
#include "readbmp.h"
#include "parse.h"

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
	
	dwordalphabet(const char*  fontName){
		//vars
		int letter_width = 0;
		int letter_height= 0;
		int bitmap_width = 0;
		int bitmap_height= 0;
		int letters_num  = 0;
		int* order;//order of letters in bitmap
		
		//create path to font bitmap
		string bitmappath = FONTS_FOLDER_STR;
		bitmappath+=SLASH_STR;
		bitmappath+=fontName;
		bitmappath+=BITMAP_FILE_TYPE_STR;
		
		//create path to font info
		string infopath = FONTS_FOLDER_STR;
		infopath+=SLASH_STR;
		infopath+=fontName;
		infopath+=INFO_FILE_TYPE_STR;
		
		//ifstream job
		ifstream font_info;
		font_info.open(infopath.data());
		
		font_info>>letter_width;
		font_info>>letter_height;
		font_info>>letters_num;
	
		order =new int[letters_num];
		for(int i=0;i<letters_num;++i){
			font_info>>order[i];
		}
		
		font_info.close();
		
		
		//recording in structure
		_len = letters_num;
		_alphabet = new dword*[_len];
		_lenOfLetter= letter_width;
		_heightOfLetter = letter_height;
		
		//reading bitmap of font
		unsigned char* data = readBMP(bitmappath.data(), &bitmap_width, &bitmap_height);
		
		//data parsing
		unsigned char** parsed_data = parse(data,letter_width,letter_height,bitmap_width,bitmap_height);
		delete data;
		
		//upload into structure
		int letter_arr_len = letter_width*letter_height*3;
		for(int i=0;i<_len;++i){
			_alphabet[i]=byteToDword(parsed_data[order[i]],letter_arr_len);
			delete parsed_data[i];
		}
		delete parsed_data;	
		
		//
		delete order;	
	}
	
	~dwordalphabet(){
		for(register int i=0;i<_len;i++){
			delete _alphabet[i];
		}
		delete _alphabet;
	}
	
};

#endif 
