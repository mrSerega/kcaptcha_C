#include <string>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iostream>

using namespace std;

#ifndef GENWORD_H_25082016
#define GENWORD_H_25082016

struct Generator{
	const char* dict;
	int dict_len;
	
	///конструктор
	/**
	\param[in] _dict - словарь
	*/

	Generator(const char* _dict);
	//generator(char* _dict, char* _range);
	
	///задать ранг используемых символоав из словаря
	/**
	\param[in] _range - может иместь следующий вид:
		first-last: a-f
		перечесление: a,b,c,d,e,f
	*/
	void set_range(const char* _range);
	int* strToIntArr(const char* word);
	///генерирует слово заданнрй длины
	int* genword(int len);
};

Generator::Generator(const char* _dict):
	dict(_dict)
{
	dict_len = strlen(dict);
}

/*
Generator::generator(char* _dict, char* _range){
	dict = _dict;
	dict_len = strlen(dict);
	set_range(_range);
}
*/

void Generator::set_range(const char* _range){
	int range_str_len = strlen(_range);
	
	if (range_str_len == 3) {//first-last
		if(_range[1]=='-'){
			int first = charToOrder(_range[0]);
			int last = charToOrder(_range[2]);
			int len = last-first+1;
			char tmp[len];
			for(int i=first;i<=last;i++){
				tmp[i-first]=dict[i];
			}
			dict = tmp;
			dict_len = len;
			return;
		}
	}

	dict = _range;
	dict_len  = range_str_len;
	return;
}

int* Generator::strToIntArr(const char* word){
	int wordlen = strlen(word);
	int* intword= new int[wordlen];
	for(int i=0;i<wordlen;i++){
		intword[i]=charToOrder(word[i]);
	}
	return intword;
}

int* Generator::genword(int len){
	srand(time(0));
	int* word = new int[len];
	for(int i=0;i<len;i++){
		word[i]=rand()%dict_len;
	}
	return word;
}

#endif
