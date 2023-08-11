#include "tokenizer.h"
#include <momo.h>

#include <plog/Log.h>
#include <iostream>


namespace momo{


/*
	checks to see if current iterator is at the end of the string,
		seting state to END if true
*/
bool Tokenizer::isEnd(){
	if(this->currIt>=this->inputString.end()){
		this->state = TokenizerState::END;		
		return true;
	}

	return false;
}

void Tokenizer::setInputString(std::string inputString){
	this->inputString = inputString;
	this->currIt  = this->inputString.begin();
	this->nextIt  = this->currIt;
		
	if(this->currIt == this->inputString.end()){//empty stream
		this->state	= TokenizerState::END;
	}else{
		this->go_next_token();
	}
}

TokenizerState Tokenizer::getState(){
	return this->state;
}


/*
	returns current token
*/
std::string Tokenizer::curr(){
	if(this->nextIt == this->inputString.begin())
		return this->next();

	return std::string(this->currIt, this->nextIt);
}

/*
	returns next token
*/
std::string Tokenizer::next(){
	if(this->isEnd()){
		return "";
	}
	//go next, if available
	if(!this->go_next_token()){
		//next is not available, return current and place currIt at end
		std::string token = std::string(this->currIt, this->nextIt);
		this->currIt = this->inputString.end();
		this->state = TokenizerState::END;
		return token;
	}else{
		//went next, return current
		return std::string(this->currIt, this->nextIt);
	}
}	

/*
	moves to next token
*/
bool Tokenizer::go_next_token(){
	std::string::iterator it=this->nextIt;
	//check if there is next token available
	if(this->nextIt >= this->inputString.end()){
		return false;
	}

	//go to next token, and determine state
	this->currIt = this->nextIt;
	get_state();
	//find token's end
	do{
		it++;
		if(it > this->inputString.end()){
			write_error("ulaz se nenadano završio usred čitanja.");
			this->state = TokenizerState::END;
			return false;
		}			
	}while(!is_token_end(it));
	this->nextIt = it;
	return true;
}

/*
	determine state based on current token
*/
void Tokenizer::get_state(){
	char c = *(this->currIt);

	switch(c){
		case '[':
			this->state = TokenizerState::L_PARAN;
				break;
		case ']':
			this->state = TokenizerState::R_PARAN;
				break;
		case ' ':
		case '\n':
		case '\t':
			this->state = TokenizerState::WHITESPACE;
				break;
		case '"':
			this->state = TokenizerState::STRING;
				break;
		case ';':
			this->state = TokenizerState::SEMICOLON;
		default:
			if(c>='0' && c<='9')
				this->state = 
					TokenizerState::NUMBER; 
			else
				this->state = 
					TokenizerState::SYMBOL;
	}

}
/*
	checks to see if current token is ending at given iterator
*/
bool Tokenizer::is_token_end(std::string::iterator &it)
{

	char c = *(it);

	//parentheses and semicolon, always length of one
	if(this->state == TokenizerState::L_PARAN || this->state == TokenizerState::R_PARAN || this->state == TokenizerState::SEMICOLON){
			return true;
	}

	//whitespace, parse until first non-whitespace character
	if(this->state == TokenizerState::WHITESPACE && c != ' '){
			return true;
	}

	//string, parse until ", 
	//	then increment it to the next character
	if(this->state == TokenizerState::STRING && c == '"'){
			it++;
			return true;
	}
	//number, parse until first non-digit
	if(this->state == TokenizerState::NUMBER && (c < '0' || c > '9') && c != '.'){
			return true;
	}
	
	//atomic symbol, parse until whitespace,parentheses, semicolon or stream end
	if(this->state == TokenizerState::SYMBOL && (c==' ' || c=='[' || c==']' || c==';' || it == this->inputString.end())){
				return true;
	}

	return false;
}

}