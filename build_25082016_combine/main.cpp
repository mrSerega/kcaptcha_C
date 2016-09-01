#include <string>
#include <cstring>
#include <ctime>
#include <cstdlib>
#include <windows.h>

#include "dwordalphabet.h"
#include "bytealphabet.h"
#include "bmp.h"
#include "genword.h"
#include "gentext.h"
#include "gradwave.h"
#include "dwordtobyte.h"
#include "createbitmap.h"
#include "circwave.h"
#include "gentextformono.h"
#include "monowave.h"

#include "defines.h"

using namespace std;

int main(int argc, char* argv[]){
	//declare service var
	int length;
	int* word_int;
	//declare flags
	enum Mode_flag{CIRC,GRAD,MONO};
		/*
		-CIRC - CAPTCHA with circles(rus:����� � �������)
		-GRAD - CPATCHA with gradient(rus:����� � ����������)
		-MONO - monochrome CPATCHA(rus:����������� �����)
		*/
	enum Rand_flag{Y,N};
		/*
		-Y - choose random CAPTCHA mode(rus:�������� ��������� ��� �����)
		-N - choose adjusted CAPTCHA mode(rus: �������� �������� ��� �����)
		*/
	enum Text_flag{GEN,ADJ};
		/*
		-GEN - generate random text for CPTCHA(rus:������������ ��������� ����� ��� �����)
		-ADJ - use adjusted text for CAPTCHA(rus: ������������ �������� �����)
		*/
	//init default
	char dict[] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};
	string font = "font";
	string word_str="test";
	Mode_flag mode_flag = GRAD;
	Rand_flag rand_flag = Y;
	Text_flag text_flag = ADJ;
	string filename = "captcha.bmp";
	//init from args
		//TODO
	//init from config
		//TODO
	//init alphabets	
	dwordalphabet dwordAlphabet(dict,font.data());
	bytealphabet  byteAlphabet(dict,font.data()); 
	//calculated vars
	word_int = strToIntArr(word_str.data());
	int wordlen = strlen(word_str.data());
	int letterHeight = dwordAlphabet._heightOfLetter;
	int letterWidth = dwordAlphabet._lenOfLetter;
	int pictureWidth = letterWidth*wordlen;
	//init bmp
	Bmp bmp(pictureWidth,letterHeight);
	//init backgroun colors
	dword dwordBGC = 0xffffffff;
	unsigned char ucBGC = 0xff;
	//gen word if necessary
	if(text_flag==GEN){
		delete word_int;
		word_int = genword(wordlen,dict);
	}
	//choose mode
	if(rand_flag==Y){
		srand(time(0));
		mode_flag = static_cast<Mode_flag>(rand()%3);
	}
	//do the deal
	switch(mode_flag){
		case CIRC:{
			dword* data = genText(wordlen,&dwordAlphabet,&length,word_int);
			data = circwave(data,pictureWidth,letterHeight,dwordBGC);
			byte* byteData = dwordToByte(data,length);
			createBitmap(byteData,filename.data(), &bmp);
			delete word_int;
			delete data;
			delete byteData;
			break;
		}
		case GRAD:{
			dword* data = genText(wordlen,&dwordAlphabet,&length,word_int);
			data = gradwave(data,pictureWidth,letterHeight,dwordBGC);
			byte* byteData = dwordToByte(data,length);
			createBitmap(byteData,filename.data(), &bmp);
			delete word_int;
			delete data;
			delete byteData;
			break;
		}
		case MONO:{
			unsigned char* data = gentextformono(wordlen,&byteAlphabet,word_int);
			data = monowave(data,pictureWidth,letterHeight,ucBGC);
			createBitmap(data,filename.data(),&bmp);
			delete word_int;
			delete data;
			break;
		}
		default:{
			//TODO
			break;
		}
	}
	//clear a memory
	return 0;	
}
