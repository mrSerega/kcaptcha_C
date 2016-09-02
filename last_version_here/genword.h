#include <string>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iostream>

using namespace std;

#ifndef GENWORD_H_25082016
#define GENWORD_H_25082016

struct range{
	
};

struct generator{
	char* dict;
	generator(char* _dict);
	void set_range(char* _range);
	int* strToIntArr(const char* word);
	int* genword(int len, char* dict);
};

generator::generator(char* _dict){
	dict = _dict
}

void generator::set_range(char* _range){
	
}

int* generator::strToIntArr(const char* word){
	int wordlen = strlen(word);
	int* intword= new int[wordlen];
	for(int i=0;i<wordlen;i++){
		intword[i]=int(word[i])-97;
	}
	return intword;
}

int* generator::genword(int len, char* dict){
	srand(time(0));
	int dictlen = strlen(dict);
	int* word = new int[len];
	for(int i=0;i<len;i++){
		word[i]=rand()%dictlen;
	}
	return word;
}

#endif
