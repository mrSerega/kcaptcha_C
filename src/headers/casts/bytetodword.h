#include "../defines.h"

#include <iostream>
#include <windows.h>
#include <conio.h>

#ifndef BYTETODWORD_H_23082016
#define BYTETODWORD_H_23082016

dword* byteToDword(unsigned char byteData[],int length){
	int byteDataSize = length;
	int dwordDataSize = byteDataSize/3;
	
	dword* dwordData = new dword[dwordDataSize];
	
	for(register int i =0;i<dwordDataSize;++i){
		register int i3=i*3;
		dwordData[i]  = ((dword)byteData[i3])  <<24;
		dwordData[i] |= ((dword)byteData[i3+1])<<16;
		dwordData[i] |= ((dword)byteData[i3+2])<<8 ;
	}
	
	return dwordData;
}

#endif
