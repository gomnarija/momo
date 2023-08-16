#include "reader.h"
#include <tokenizer.h>


#include <iostream>
#include <fstream>
#include <filesystem>

#include <plog/Log.h>

namespace momo{

/*
    converts single token base on state
*/
moValPtr read_atom(Tokenizer &toki){
    if(toki.isEnd()){
		write_error("ulaz se nenadano završio usred čitanja.");
        return NIL;
    }
    //get moVal from token based on state
	switch(toki.getState()){ 
		case TokenizerState::NUMBER:
			return moValPtr(new moNumber(toki.curr()));
		case TokenizerState::STRING:
			return moValPtr(new moString(string_remove_quotes(toki.curr())));
		default:
			return moValPtr(new moSymbol(toki.curr()));
	}
}

/*
    converts list of tokens based on states
*/
moValPtr read_list(Tokenizer &toki, std::ifstream *file){
    if(toki.getState() != TokenizerState::L_PARAN){
		write_error("neočekivani token na mestu '['.");
		return NIL;
    }
	
	moListPtr result(new moList);
    //start collecting items
	std::string token = toki.next();
	while(toki.getState() != TokenizerState::R_PARAN){
		if(toki.getState() == TokenizerState::END){
			std::string nextLine;
			if(file == nullptr || !std::getline(*file, nextLine)){
		    	write_error("ulaz se nenadano završio usred čitanja.");
				return NIL;
			}else{
				//go to new line
				toki.setInputString(nextLine);
				LOG_LINE++;
				continue;
			}
        }
        if(toki.getState() == TokenizerState::WHITESPACE){
            token = toki.next();
            continue;
        }

		if(toki.getState() == TokenizerState::L_PARAN){//list in list
			result->insert(read_list(toki, file));
        }
		else{
			result->insert(read_atom(toki));
        }
        
		token = toki.next();		 
	}
	return result;
}


/*
    converts given string into moVal representations
*/
moValPtr read_string(std::string &str){
    Tokenizer toki(str);
    while(!toki.isEnd()){
        if(TokenizerState::WHITESPACE){
            continue;
        }

        switch(toki.getState()){
            case TokenizerState::L_PARAN:
			    return read_list(toki, nullptr);
		    case TokenizerState::NUMBER:
		    case TokenizerState::STRING:
		    case TokenizerState::SYMBOL:
			    return read_atom(toki);	
		default:
	            write_error("nevalidan token.");
			    return NIL;
        }
		toki.next();
    }

    return NIL;
}


moListPtr read_file(const std::string &path){
    std::ifstream file(std::filesystem::u8path(path));
    if(!file.is_open()){
		write_error("fajl " + path + " nije pronađen.");
	    return moListPtr(new moList);
	}

	LOG_FILENAME = get_file_name(path);

	return read_file(file);
}



/*
	reads file line by line,
		expects file to be open,
			closes file after it's done
*/
moListPtr read_file(std::ifstream &file){
	moListPtr result(new moList);
	if(!file.is_open()){
		return result;
	}
	
	//read line by line
    std::string line;
	Tokenizer toki(line);
	while(std::getline(file, line)){
		toki.setInputString(line);
    	while(!toki.isEnd()){
    	    if(TokenizerState::WHITESPACE){
    	        continue;
    	    }
    	    switch(toki.getState()){
    	        case TokenizerState::L_PARAN:
				    result->insert(read_list(toki, &file));
			    	break;
				case TokenizerState::NUMBER:
			    case TokenizerState::STRING:
			    case TokenizerState::SYMBOL:
				    result->insert(read_atom(toki));	
					break;
			default:
		            break;
    	    }
			toki.next();
    	}
		LOG_LINE++;
    }

    //done
    file.close();
	return result;
}


}

