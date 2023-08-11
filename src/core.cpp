#include "momo.h"

namespace momo{




/*
    sum of given numbers,
        if string is encountered, try to convert it to number
            if something else return NIL
*/
moValPtr _mo_sum_number(moListPtr items){
    double sum = 0;
    moVal *current;
    for(int i=0;i<items->size();i++){
        switch(items->at(i)->getType()){
            case MO_TYPE::MO_NUMBER:
                sum += std::static_pointer_cast<moNumber>(items->at(i))->getValue();
                break;
            case MO_TYPE::MO_STRING:
                sum += string_to_number(items->at(i));
                break;
            default:
                write_error("sabiranje, vrednost nije broj ili tekst.");
                return NIL;
        }
    }

    return moValPtr(new moNumber(sum));
}


/*
    concat given string,
        if something else is encountered concat it's print()
*/
moValPtr _mo_sum_string(moListPtr items){
    std::string result;
    moVal *current;
    for(int i=0;i<items->size();i++){
            current = items->at(i).get();
            result += current->print();
    }
    
    return moValPtr(new moString(result));
}


/*
    based on the first arg type:
        number : returns total sum of all arguments if they are numbers,
                    if string, tries to convert it to number
        string : concats all arguments,
                    if some other type use it's print value
*/
moValPtr mo_sum(moListPtr items, moEnv& env){
    if(items->size() == 0){
        write_error("sabiranje, vrednosti nisu prosleđene.");
        return NIL;
    }
    moVal *first = items->at(0).get();
    if(first == nullptr){
        write_error("sabiranje, prva vrednost je nevalidna.");
        return NIL;
    }
    switch(first->getType()){
        case MO_TYPE::MO_STRING:
            return _mo_sum_string(items);
        case MO_TYPE::MO_NUMBER:
            return _mo_sum_number(items);
        default:
            write_error("sabiranje, prva vrednost je nevalidna.");
            return NIL;
    }
}

/*
    based on the first arg type:
        number : returns total difference of all arguments if they are numbers,
                    if string, tries to convert it to number
*/
moValPtr mo_difference(moListPtr items, moEnv& env){
    if(items->size() == 0){
        write_error("oduzimanje, vrednosti nisu prosleđene.");
        return NIL;
    }
    if(items->at(0)->getType() != MO_TYPE::MO_NUMBER){
        write_error("oduzimanje, prva vrednost je nevalidna.");
        return NIL;
    }
    //get first value
    double diff = std::static_pointer_cast<moNumber>(items->at(0))->getValue();
    moVal *current;
    //substract other values from the first one
    for(int i=1;i<items->size();i++){
        switch(items->at(i)->getType()){
            case MO_TYPE::MO_NUMBER:
                diff -= std::static_pointer_cast<moNumber>(items->at(i))->getValue();
                break;
            case MO_TYPE::MO_STRING:
                diff -= string_to_number(items->at(i));
                break;
            default:
                write_error("oduzimanje, vrednost nije broj ili tekst.");
                return NIL;
        }
    }

    return moValPtr(new moNumber(diff));
}

}