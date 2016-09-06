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
#include "iniparser.h"

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
	string dict="abcdefghjiklmnopqrstuvwxyz";
	string range = "a-z";
	string font = "rt";
	string word_str="test";
	Mode_flag mode_flag = MONO;
	Rand_flag rand_flag = Y;
	Text_flag text_flag = GEN;
	string filename = "captcha.bmp";
	//init from args
		//TODO
	//init from config
	IniParser iniparser(CONFIG_FILE_NAME);
	string use_it = iniparser.get_value(COMMON_SECTION_NAME,USE);
	if(use_it==USE_TRUE){
		dict=iniparser.get_value(DEMON_SECTION_NAME,DICT_VARIABLE_NAME);
		range=iniparser.get_value(DEMON_SECTION_NAME,RANGE_VARIABLE_NAME);
		font=iniparser.get_value(COMMON_SECTION_NAME,FONT_VARIABLE_NAME);
		word_str=iniparser.get_value(STANDALONE_SECTION_NAME,WORD_VARIABLE_NAME);
	
		string tmp_mode_flag = iniparser.get_value(STANDALONE_SECTION_NAME,MODE_FLAG);//mode
		if(tmp_mode_flag==CIRC_FLAG) mode_flag=CIRC;
		if(tmp_mode_flag==GRAD_FLAG) mode_flag=GRAD;
		if(tmp_mode_flag==MONO_FLAG) mode_flag=MONO;

		string tmp_random_flag = iniparser.get_value(STANDALONE_SECTION_NAME,RANDOM_FLAG);//mode
		if(tmp_random_flag==Y_FLAG) rand_flag=Y;
		if(tmp_random_flag==N_FLAG) rand_flag=N;
	
		string tmp_text_flag = iniparser.get_value(STANDALONE_SECTION_NAME,TEXT_FLAG);//mode
		if(tmp_text_flag==GEN_FLAG) text_flag=GEN;
		if(tmp_text_flag==ADJ_FLAG) text_flag=ADJ;
	
		filename=iniparser.get_value(COMMON_SECTION_NAME,FILE_NAME_VARIABLE_NAME);
	}
	//init alphabets	
	dwordalphabet dwordAlphabet(font.data());
	bytealphabet  byteAlphabet(font.data()); 
	//make generator
	Generator generator(dict.data());
	generator.set_range(range.data());
	//calculated vars
	word_int = generator.strToIntArr(word_str.data());
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
		word_int = generator.genword(wordlen);
		
		//test outpt###########################################
		for(int i=0;i<wordlen;++i){
			cout<<dict[word_int[i]];
		}
		//test output##########################################
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
			//data = go3(data,pictureWidth,letterHeight);
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
