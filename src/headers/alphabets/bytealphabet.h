#include <cstring>

#include "../bitmap/readbmp.h"

using namespace std;

#ifndef BYTEALPHABET_H_25082016
#define BYTEALPHABET_H_25082016

struct bytealphabet{
	int _len;
	int _letterlen;
	int _letterheight;
	unsigned char** _alphabet;
	
	bytealphabet (const char*  fontName){
		//vars
		int letter_width = 0;
		int letter_height= 0;
		int bitmap_width = 0;
		int bitmap_height= 0;
		int letters_num  = 0;
		int* order;
		
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
		
		order = new int[letters_num];
		for(int i=0;i<letters_num;++i){
			font_info>>order[i];
		}
		
		font_info.close();
		
		//recording in structure
		_len = letters_num;
		_alphabet = new unsigned char*[_len];
		_letterlen = letter_width;
		_letterheight = letter_height;
		
		//reading bitmap of font
		unsigned char* data = readBMP(bitmappath.data(), &bitmap_width, &bitmap_height);
		
		//data parsing
		unsigned char** parsed_data = parse(data,letter_width,letter_height,bitmap_width,bitmap_height);
		delete data;
		
		for(int i=0;i<_len;++i){
			_alphabet[i]= new unsigned char[letter_width*letter_height];
			makeLetter(parsed_data[order[i]],i);
			//delete parsed_data[i];
		}
		delete parsed_data;
		
		//
		delete order;
	}
	
	~bytealphabet(){
		for(int i=0;i<_len;++i){
			delete _alphabet[i];
		}
		delete _alphabet;
	}
	
	void makeLetter(unsigned char* data, int num){
		for(int x=0;x<_letterlen;x++){
    		for(int y=0;y<_letterheight;y++){
				int pos = (x+y*_letterlen);
				_alphabet[num][pos]=(data[pos*3]);
			}
		}
	}
	
};

#endif
