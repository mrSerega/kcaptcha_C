#include <iostream>

using namespace std;


#ifndef PARSE_H_30082016
#define PARSE_H_30082016

unsigned char** parse(unsigned char* data, int letter_width, int letter_height, int width, int height){
	int lines = height/letter_height;
	int columns = width/letter_width;
	
	cout<<lines<<" "<<columns<<endl;
	
	int num_of_letters = lines*columns;
	
	unsigned char** return_data = new unsigned char*[num_of_letters];
	
	for(int i=0;i<columns;++i){
		for(int j=0;j<lines;++j){
			return_data[i+j*columns] = new unsigned char[letter_width*letter_height*3];
			for(int x=0;x<letter_width;++x){
				for(int y=0;y<letter_height;++y){
					int bitmap_global_pos =3*(i*letter_width+x + (j*letter_height+y)*width);
					int bitmap_local_pos = 3*(x+y*letter_width);
					int array_pos = i+j*columns;
					return_data[array_pos][bitmap_local_pos]=data[bitmap_global_pos];
					return_data[array_pos][bitmap_local_pos+1]=data[bitmap_global_pos+1];
					return_data[array_pos][bitmap_local_pos+2]=data[bitmap_global_pos+2];
				}
			}
		}
	}
	
	return return_data;
}

#endif
