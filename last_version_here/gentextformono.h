#include "bytealphabet.h"

#ifndef GENTEXTFORMONO_H_25082016
#define GENTEXTFORMONO_H_25082016

unsigned char* gentextformono(int wordlen,bytealphabet* alphabet,int* word){
	int width = alphabet->_letterlen*wordlen;
	unsigned char* imageData = new unsigned char[alphabet->_letterheight*width];
	for(register int c=0;c<wordlen;c++){
		int xlwc=alphabet->_letterlen*c;
		int lwcpl1 = alphabet->_letterlen*(c+1);	
		for(register int x=xlwc;x<lwcpl1;x++){
			for(register int y=0;y<alphabet->_letterheight;y++){
					
				int pos =  (x + y * width);
					
				unsigned char color = alphabet->_alphabet[word[c]][x%alphabet->_letterlen+y*alphabet->_letterlen];
					
				imageData[pos]=color;
						
			}
		}
	}
	return imageData;
}

#endif
