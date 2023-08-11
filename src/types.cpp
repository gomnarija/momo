#include "momo.h"

#include <plog/Log.h>
#include <stdexcept>

namespace momo{




// NUMBER

moNumber::moNumber(){
    this->value = 0;
}

moNumber::moNumber(double value){
    this->value = value;
}

moNumber::moNumber(std::string value){
    this->value = string_to_number(value);
}


double
moNumber::getValue() {
    return this->value;
}

std::string 
moNumber::print(){
    return std::to_string(this->value);
}

 MO_TYPE
moNumber::getType(){
    return MO_TYPE::MO_NUMBER;
}

bool 
moNumber::isTrue(){
    return this->value != 0;
}

bool
moNumber::equals (moValPtr other){
    return other->getType() == MO_TYPE::MO_NUMBER &&
            this->value == (std::dynamic_pointer_cast<moNumber>(other))->getValue();
}



// STRING

moString::moString(){}

moString::moString(std::string value){
    this->value = value;
}

std::string
moString::getValue() {
    return this->value;
}

std::string 
moString::print(){
    return this->value;
}

 MO_TYPE
moString::getType(){
    return MO_TYPE::MO_STRING;
}

bool 
moString::isTrue(){
    return !this->value.empty();
}

bool
moString::equals (moValPtr other){
    return other->getType() == MO_TYPE::MO_STRING &&
            this->value == (std::dynamic_pointer_cast<moString>(other))->getValue();
}


// LIST

moList::moList(){}

moList::moList(std::vector<moValPtr> items){
    this->items = items;
}

void
moList::insert(moValPtr val){
    this->items.push_back(val);
}

size_t 
moList::size(){
    return this->items.size();
}

moValPtr
moList::at(size_t index){
    if(index < this->items.size()){
        return this->items.at(index);
    }else{
        return NIL;
    }
}

std::string 
moList::print(){
    std::string res("(");
	for(int i=0;i<this->size();i++){
		res += (this->items[i])->print();	
		if(i<this->size()-1)
			res += " ";
	}

	res+= ")";
	return res;
}

 MO_TYPE
moList::getType(){
    return MO_TYPE::MO_LIST;
}

bool 
moList::isTrue(){
    return !this->items.empty();
}

bool 
moList::equals(moValPtr other){
    if(other->getType() != MO_TYPE::MO_LIST)
        return false;

	moListPtr list = std::dynamic_pointer_cast<moList>(other);
	for(int i=0;i<this->size();i++){
		moValPtr first  = this->at(i);
		moValPtr second = this->at(i);
	
		if(first->getType() != second->getType() || !first->equals(second)){
			return false;
        }
	}

	return true;
}

// SYMBOL

moSymbol::moSymbol(){}

moSymbol::moSymbol(std::string value){
    this->value = value;
}

std::string 
moSymbol::print(){
    return this->value;
}

MO_TYPE
moSymbol::getType(){
    return MO_TYPE::MO_SYMBOL;
}

bool
moSymbol::isTrue(){
    //TODO: idk ?
    return false;
}

bool
moSymbol::equals(moValPtr other){
    //TODO: idk ?
    return false;
}


// FUNCTION

moFunction::moFunction(){

}

moFunction::moFunction(std::string name, moListPtr args, moListPtr body){
    this->name = name;
    this->args = args;
    this->body = body;
}

std::string 
moFunction::print(){
    return this->name;
}

 MO_TYPE 
moFunction::getType(){
    return MO_TYPE::MO_FUNCTION;
}

std::string
moFunction::getName(){
    return this->name;
}

moListPtr
moFunction::getArgs(){
    return this->args;
}

moListPtr
moFunction::getBody(){
    return this->body;
}



// Nil

_moNil::_moNil(){

}	

std::string 
_moNil::print(){
    return "Ništa";
}

 MO_TYPE
_moNil::getType(){
    return MO_TYPE::MO_NIL;
}

bool
_moNil::equals(moValPtr other){
    return other->getType() == MO_TYPE::MO_NIL;
}

bool
_moNil::isTrue(){
    return false;
}


// Env
moEnv::moEnv(){
    this->upperEnv = nullptr;
}

moEnv::moEnv(moEnv *upperEnv){
    this->upperEnv = upperEnv;
}

moEnv::moEnv(moEnv *upperEnv, std::map<std::string, moValPtr> symbolValueMap){
    this->upperEnv = upperEnv;
    this->symbolValueMap = symbolValueMap;
}
	
moValPtr	
moEnv::getVal(std::string key){
    if(this->symbolValueMap.find(key) == this->symbolValueMap.end())
        return NIL;
    
    return this->symbolValueMap.at(key);
}

void
moEnv::setVal(std::string key, moValPtr val){
    if(this->symbolValueMap.find(key) != this->symbolValueMap.end())
        this->symbolValueMap[key] = val;
}

void
moEnv::insertVal(std::string key, moValPtr val){
    if(this->symbolValueMap.find(key) == this->symbolValueMap.end())
        this->symbolValueMap[key] = val;
}
		
moEnv*
moEnv::getUpperEnv(){
    return this->upperEnv;
}





}