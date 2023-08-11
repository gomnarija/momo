#include "momo.h"

#include <float.h>



namespace momo{


/*
    tries to convert given string value to number
*/
double string_to_number(moValPtr str){
    if(str == nullptr && str->getType() != MO_TYPE::MO_STRING){
        write_error("prosleđena vrednost za pretvaranje u broj nije tekst.");
        return 0;
    }
    return string_to_number(std::static_pointer_cast<moString>(str)->getValue());
}


/*
    tries to convert given string value to number
*/
double string_to_number(const std::string &value){
    try{
		return std::stod(value);
	}
	catch( std::invalid_argument &ia){
		write_error("neuspešno pretvaranje teksta u broj: " + value);
		return 0;
	}
	catch( std::out_of_range &oor){
		write_warning("vrednost broja prelazi maksimalnu dozvoljenu vrednost: " + value);
		return DBL_MAX;
	}	
}



/*
    removes double quotes from given string,
        if present
*/
std::string string_remove_quotes(const std::string &str){
    if(!str.empty() && str.at(0) == '\"' && str.at(str.size()-1) == '\"'){
        return str.substr(1, str.size()-2);
    }else{
        return str;
    }
}


/*
    extracts filename from path
*/
std::string get_file_name(const std::string &path){
    if(path.find_last_of('\\') != std::string::npos){
        return path.substr(path.find_last_of('\\'));
    }else if(path.find_last_of('/') != std::string::npos){
        return path.substr(path.find_last_of('/'));
    }else{
        return std::string(path);
    }
}






}