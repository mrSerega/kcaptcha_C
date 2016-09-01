#include <string>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iostream>

using namespace std;

#ifndef GENWORD_H_25082016
#define GENWORD_H_25082016

int* strToIntArr(const char* word){
	int wordlen = strlen(word);
	int* intword= new int[wordlen];
	for(int i=0;i<wordlen;i++){
		intword[i]=int(word[i])-97;
	}
	return intword;
}

int* genword(int len, char* dict){
	srand(time(0));
	int dictlen = strlen(dict);
	int* word = new int[len];
	for(int i=0;i<len;i++){
		word[i]=rand()%26;
	}
	return word;
}

#endif
