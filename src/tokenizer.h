#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <string>

namespace momo{

enum TokenizerState{
		WHITESPACE,
		SYMBOL,
		NUMBER,
		STRING,
		L_PARAN,
		R_PARAN,
		SEMICOLON,
		END
};

struct Tokenizer
{
	Tokenizer(std::string &input_str)
		:inputString{input_str}
	{
		this->setInputString(input_str);
	}
	

	void				setInputString(std::string);
	TokenizerState 		getState();
	
	std::string next();
	std::string curr();

	bool	    isEnd();
	
private:
	TokenizerState				state;
	std::string					&inputString;			
	std::string::iterator		currIt,nextIt;
	
	bool go_next_token();
	void get_state();
	bool is_token_end(std::string::iterator &);	

	
};

}
#endif