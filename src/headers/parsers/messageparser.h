#ifndef MSGPAR_H
#define MSGPAR_H

#include <string>
#include <map>
#include <stdexcept>

#define MSG_START '['
#define MSG_END ']'
#define MSG_SPLIT ';'
#define MSG_EQ '='
#define MSG_MAX_LEN MAX_PATH

//commands
#define COM "com"
//com values
#define END_LISTENING "end_listening"
#define MAKE_CAPTCHA "make_captcha"

using namespace std;

///класс для работы с собщением прнятым по сокету

class MessageParser{
	public:
		MessageParser(char* msg);
		~MessageParser();
		///возвращает значение данного сегмента сообщения
		/**
		\param[in] variable - имя сегмента сообщения
		*/
		const char* get_value(string variable);
	private:
		map<string,string> container;
};

MessageParser::MessageParser(char* msg){
	
	if(msg[0]!=MSG_START){//right start of msg
		cout<<msg<<endl;
		throw std::invalid_argument("wrong structure of network message: first sumbol should be '[' ");
		return;
	}
	
	for(int i=1;i<MSG_MAX_LEN;i++){
		
		if (msg[i]==MSG_END){//right end;
			return;
			break;
		}
		
		string push ="";//element for handling
		
		while( msg[i]!=MSG_SPLIT && msg[i]!=MSG_END ){//create push
			push+=msg[i++];
		}
		
		
		int found_eq = push.find_first_of(MSG_EQ);
		string variable = push.substr(0,found_eq);
		string value = push.substr(found_eq+1,push.size()-1);
		container[variable]=value;
		if(msg[i]==MSG_END ) return;
	}
	
	throw std::invalid_argument("wrong structure of network meassge: message sholdn't be more then 1000 symbols"); //TODO: make it nice
}

MessageParser::~MessageParser(){
	printf("MessageParser object deleted\n");
}

const char* MessageParser::get_value(string variable){
	try{
		return container[variable].data();
	}catch(exception e){
		throw std::invalid_argument("value not found.");
	}
}

#endif
