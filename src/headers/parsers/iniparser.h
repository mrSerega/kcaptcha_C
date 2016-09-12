#include <map>
#include <iostream>
#include <fstream>
#include <string>

#define COMMENT '#'
#define SECTION_START '['
#define SECTION_END ']'
#define VAR_SPLIT '='
#define DEFAULT_SECTION_NAME "default"

#ifndef INIPARSER_H
#define INIPARSER_H

using namespace std;

struct IniParser{
	map<string,map<string,string> >container; 
	IniParser(const char* filename){
	//init fstream
	ifstream ini;
	ini.open(filename);
	string str;
	int tmp;
	
	//service
	string section_name = DEFAULT_SECTION_NAME;
	string variable;
	string value;
	
	//file reading
	while((tmp=ini.get())!=EOF){
		//read line
		char tmpch=static_cast<char>(tmp);
		string push="";
		push+=tmpch;
		getline(ini,str);
		push+=str;	
		//handling
		if (push[0]==COMMENT) {//handling of comments
			//cout<<"comment:"<<push<<endl;
			continue; //skip comments
		}
		
		if (push[0]==SECTION_START){//handling of sections
			section_name = push.substr(1,push.size()-2);
			//cout<<"section name:"<<section_name<<endl;
			continue;
		}
		
		//handling of var
		int found;
		found = push.find_first_of(VAR_SPLIT);
		variable = push.substr(0,found);
		value = push.substr(found+1,push.size()-1);
		container[section_name][variable]=value;
		//cout<<"variable:"<<variable<<" value:"<<value<<endl;
		
	}
}
	
	const char* get_value(const char* section_name, const char* variable_name){
		return container[section_name][variable_name].data();
	}
};

#endif
