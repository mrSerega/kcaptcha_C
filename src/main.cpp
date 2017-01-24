#include <string>
#include <cstring>
#include <ctime>
#include <cstdlib>
#include <winsock2.h>
#include <windows.h>
#include <direct.h>
#include <stdio.h>

#include "headers/alphabets/dwordalphabet.h"	//alphabet for full color captcha
#include "headers/alphabets/bytealphabet.h"		//alphabet for mobochrome captcha

#include "headers/bitmap/bmp.h"					//bmp helper structure
#include "headers/bitmap/createbitmap.h"		//func for creating bmp file

#include "headers/genword.h"					//func for generating text data

#include "headers/gentext/gentext.h"			//gen full color bitmap of text
#include "headers/gentext/gentextformono.h"		//gen monochrome bitmap of text

#include "headers/waves/gradwave.h"				//different algo for wave
#include "headers/waves/circwave.h"
#include "headers/waves/monowave.h"

#include "headers/casts/dwordtobyte.h"			//cast

#include "headers/parsers/iniparser.h"			//parser of ini files
#include "headers/parsers/messageparser.h"		//pareser of message from socket

#include "headers/network/server.h"				//soket helper
#include "headers/network/sharedmemory.h"		//shm helper

#include "headers/defines.h"

using namespace std;

int main(int argc, char* argv[]){
	//declare service var
	int length;
	int* word_int;
	//declare flags
	enum Mode_flag{CIRC,GRAD,MONO};
		/*
		-CIRC - CAPTCHA with circles
		-GRAD - CPATCHA with gradient
		-MONO - monochrome CPATCHA
		*/
	enum Rand_flag{Y,N};
		/*
		-Y - choose random CAPTCHA mode
		-N - choose adjusted CAPTCHA mode
		*/
	enum Text_flag{GEN,ADJ};
		/*
		-GEN - generate random text for CPTCHA
		-ADJ - use adjusted text for CAPTCHA
		*/
	//init default
	int sleep_time = 100;
	string dict="abcdefghjiklmnopqrstuvwxyz";
	string range = "a-z";
	string font = "rt";
	string word_str="test";
	string filename = "captcha.bmp";
		//flag variables
	Mode_flag mode_flag = MONO;
	Rand_flag rand_flag = Y;
	Text_flag text_flag = GEN;
		//shared memory variables
	string shm_object_name = "default_shm_object_name";
	size_t shm_object_size = 1000;
	//init from args
		//TODO: init from args
	//init from config
	IniParser iniparser(CONFIG_FILE_NAME);
	string use_it = iniparser.get_value(COMMON_SECTION_NAME,USE);
	if(use_it==USE_TRUE){
		sleep_time = strToInt(iniparser.get_value(NETWORK_SECTION_NAME,SLEEP_TIME));
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
		//TODO: shared memory object name from config
	}
	//init from network------------------------------------------------------------------------------------------------------------
	c_server* Server = new c_server();
	char* message = NULL;
	MessageParser* msgparser = NULL;
	//init alphabets--------------------------------------------------------------------------------------------------------------------	
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
	//declare path to exe
	char current_work_dir[FILENAME_MAX];
	_getcwd(current_work_dir, sizeof(current_work_dir));
	cout<<current_work_dir<<endl;
	string file_path = current_work_dir;
	file_path += '\\';
	file_path += "captcha.bmp";
	cout<<"path: "<<file_path<<endl;
	//do the deal++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	while(true){
		//choose the step
		delete msgparser;
		message = Server->listen_network(sleep_time);
		cout<<"msg: "<<message<<endl;
		msgparser=new MessageParser(message);
		string com = msgparser->get_value(COM);
		
		if(com==END_LISTENING){
			printf(MSG_SHUTDOWN);
			break;
		}else{
			if(com!=MAKE_CAPTCHA){
				printf("wrong msg\n");
				continue;
			}
		}	
			
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
		//creation shm object
		cSharedMemory shm(shm_object_name.data());
		//doing...
		switch(mode_flag){
			case CIRC:{
				dword* data = genText(wordlen,&dwordAlphabet,&length,word_int);
				data = circwave(data,pictureWidth,letterHeight,dwordBGC);
				byte* byteData = dwordToByte(data,length);
				createBitmap(byteData,filename.data(), &bmp);//TODO: remove line
				//TODO: put in mem
				cout<<"putting data...\n";//TODO: delete this
				size_t buffer_size = length*3+bmp.bitmapFileHeaderSize;
				shm.createObject(buffer_size);//TODO
				shm.setData(byteData,buffer_size);//TODO
				//---
				delete word_int;
				delete data;
				delete byteData;
				break;
			}
			case GRAD:{
				dword* data = genText(wordlen,&dwordAlphabet,&length,word_int);
				data = gradwave(data,pictureWidth,letterHeight,dwordBGC);
				byte* byteData = dwordToByte(data,length);
				createBitmap(byteData,filename.data(), &bmp);//TODO: remove line
				//TODO: put in mem
				delete word_int;
				delete data;
				delete byteData;
				break;
			}
			case MONO:{
				unsigned char* data = gentextformono(wordlen,&byteAlphabet,word_int);
				data = monowave(data,pictureWidth,letterHeight,ucBGC);
				createBitmap(data,filename.data(),&bmp);//TODO: remove line
				//TODO: put in mem
				delete word_int;
				delete data;
				break;
			}
			default:{
				//TODO: make smth
				break;
			}
		}
		//sending to client
		printf("sending...\n");//TODO: delete this
		Server->send_to_client(shm_object_name.data());//TODO: send a data
	}//ebd deal+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//clear a memory
	//TODO: clear memory
	Server->send_to_client("shutdown");
	delete Server;
	return 0;	
}
