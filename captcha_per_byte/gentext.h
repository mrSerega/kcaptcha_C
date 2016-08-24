#include <cstdlib>
#include <ctime>

#include "alphabet.h"

#include "defines.h"

#include <iostream>

#ifndef GEN_H_23082016
#define GEN_H_23082016

dword* genText(const int lenOfWord, const Alphabet* alphabet, int* length){
	int width = lenOfWord*alphabet->_lenOfLetter;
	int height = alphabet->_heightOfLetter;
	dword* data = new dword[width*height];
	srand(time(0));
	int word[lenOfWord];
	for(int i=0;i<lenOfWord;i++){
		word[i]=rand()%alphabet->_len;
	}
	for(register int l=0;l<lenOfWord;l++){
		for(register int x=alphabet->_lenOfLetter*l;x<alphabet->_lenOfLetter*(l+1);x++){
			for(register int y=0;y<alphabet->_heightOfLetter;y++){
				int pos=(x+y*width);
				dword color = alphabet->_alphabet[word[l]][x%alphabet->_lenOfLetter+y*alphabet->_lenOfLetter];
				data[pos]=color;
			}
		}
	}
	*length = width*height;
	return data;
}

#endif
