#include "momo.h"


#include <functional>

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

/*
    based on the first arg type:
        number : returns total product of all arguments if number,
                    if string tries to convert it to number
*/
moValPtr mo_product(moListPtr items, moEnv& env){
    if(items->size() == 0){
        write_error("množenje, vrednosti nisu prosleđene.");
        return NIL;
    }
    double product = 1;
    moVal *current;
    for(int i=0;i<items->size();i++){
        switch(items->at(i)->getType()){
            case MO_TYPE::MO_NUMBER:
                product *= std::static_pointer_cast<moNumber>(items->at(i))->getValue();
                break;
            case MO_TYPE::MO_STRING:
                product *= string_to_number(items->at(i));
                break;
            default:
                write_error("množenje, vrednost nije broj ili tekst.");
                return NIL;
        }
    }

    return moValPtr(new moNumber(product));
}


/*
    based on the first arg type:
        number : returns total difference of all arguments if they are numbers,
                    if string, tries to convert it to number
*/
moValPtr mo_quotient(moListPtr items, moEnv& env){
    if(items->size() == 0){
        write_error("deljenje, vrednosti nisu prosleđene.");
        return NIL;
    }
    if(items->at(0)->getType() != MO_TYPE::MO_NUMBER){
        write_error("deljenje, prva vrednost je nevalidna.");
        return NIL;
    }
    //get first value
    double quot = std::static_pointer_cast<moNumber>(items->at(0))->getValue();
    moVal *current;
    //substract other values from the first one
    for(int i=1;i<items->size();i++){
        double value;
        switch(items->at(i)->getType()){
            case MO_TYPE::MO_NUMBER:
                value = std::static_pointer_cast<moNumber>(items->at(i))->getValue();
                break;
            case MO_TYPE::MO_STRING:
                value = string_to_number(items->at(i));
                break;
            default:
                write_error("deljenje, vrednost nije broj ili tekst.");
                return NIL;
        }
        if(value == 0){
            write_error("deljenje sa nulom.");
            return NIL;
        }else{
            quot /= value;
        }
    }

    return moValPtr(new moNumber(quot));
}


/*
    returns truth value
*/
moValPtr mo_truth_value(moListPtr items, moEnv& env){
    if(items->size() != 1){
		write_error("operator istinitosti, prosleđeno više od jedne vrednosti.");
		return NIL;
    }

	return items->at(0)->isTrue() ? TRUE : FALSE;
}

/*
    returns not truth value
*/
moValPtr mo_not_truth_value(moListPtr items, moEnv& env){
    if(items->size() != 1){
		write_error("operator negacije, prosleđeno više od jedne vrednosti.");
		return NIL;
    }

	return items->at(0)->isTrue() ? FALSE : TRUE;
}

/*
    compares arguments
*/
moValPtr mo_equals(moListPtr items, moEnv& env){
    if(items->size() < 2){
		write_error("operator jednakosti, prosleđeno manje od dve vrednosti.");
		return NIL;
    }

    for(int i=1;i<items->size();i++){
        if(!items->at(i)->equals(items->at(i-1))){
            return FALSE;
        }
    }

	return TRUE;
}


moValPtr _mo_compare_number(moListPtr items, std::function<bool(double, double)> comparator){
    moNumberPtr first = std::static_pointer_cast<moNumber>(items->at(0));
    for(int i=1;i<items->size();i++){
        moValPtr current = items->at(i);
        if(current->getType() != MO_TYPE::MO_NUMBER){
            write_error("poređenje, prosleđena vrednost nije broj.");
            return NIL;
        }
        if(!comparator(first->getValue(), std::static_pointer_cast<moNumber>(current)->getValue())){
            return FALSE;
        }
    }

    return TRUE;
}

moValPtr _mo_compare_string(moListPtr items, std::function<bool(size_t, size_t)> comparator){
    moStringPtr first = std::static_pointer_cast<moString>(items->at(0));
    for(int i=1;i<items->size();i++){
        moValPtr current = items->at(i);
        if(current->getType() != MO_TYPE::MO_STRING){
            write_error("poređenje, prosleđena vrednost nije tekst.");
            return NIL;
        }
        if(!comparator(ustrlen(first->getValue()), ustrlen(std::static_pointer_cast<moString>(current)->getValue()))){
            return FALSE;
        }
    }

    return TRUE;
}


/*
    based on the first arg type:
        number : checks if first number is bigger then the rest
        string : checks if first string is longer then the rest
*/
moValPtr mo_greater(moListPtr items, moEnv& env){
    if(items->size() < 2){
		write_error("operator veće, prosleđeno manje od dve vrednosti.");
		return NIL;
    }

    moVal *first = items->at(0).get();
    if(first == nullptr){
        write_error("operator veće, prva vrednost je nevalidna.");
        return NIL;
    }

    switch(first->getType()){
        case MO_TYPE::MO_STRING:
            return _mo_compare_string(items, std::greater<size_t>());
        case MO_TYPE::MO_NUMBER:
            return _mo_compare_number(items, std::greater<double>());
        default:
            write_error("operator veće, prva vrednost nije broj ni tekst.");
            return NIL;
    }

}

/*
    based on the first arg type:
        number : checks if first number is smaller then the rest
        string : checks if first string is shorter then the rest
*/
moValPtr mo_less(moListPtr items, moEnv& env){
    if(items->size() < 2){
		write_error("operator manje, prosleđeno manje od dve vrednosti.");
		return NIL;
    }

    moVal *first = items->at(0).get();
    if(first == nullptr){
        write_error("operator manje, prva vrednost je nevalidna.");
        return NIL;
    }

    switch(first->getType()){
        case MO_TYPE::MO_STRING:
            return _mo_compare_string(items, std::less<size_t>());
        case MO_TYPE::MO_NUMBER:
            return _mo_compare_number(items, std::less<double>());
        default:
            write_error("operator manje, prva vrednost nije broj ni tekst.");
            return NIL;
    }

}

/*
    based on the first arg type:
        number : checks if first number is bigger or equal then the rest
        string : checks if first string is longer or equal then the rest
*/
moValPtr mo_greater_or_equals(moListPtr items, moEnv& env){
    if(items->size() < 2){
		write_error("operator veće-ili-jednako, prosleđeno manje od dve vrednosti.");
		return NIL;
    }

    moVal *first = items->at(0).get();
    if(first == nullptr){
        write_error("operator veće-ili-jednako, prva vrednost je nevalidna.");
        return NIL;
    }

    switch(first->getType()){
        case MO_TYPE::MO_STRING:
            return _mo_compare_string(items, std::greater_equal<size_t>());
        case MO_TYPE::MO_NUMBER:
            return _mo_compare_number(items, std::greater_equal<double>());
        default:
            write_error("operator veće-ili-jednako, prva vrednost nije broj ni tekst.");
            return NIL;
    }

}

/*
    based on the first arg type:
        number : checks if first number is smaller or equal then the rest
        string : checks if first string is shorter or equal then the rest
*/
moValPtr mo_less_or_equals(moListPtr items, moEnv& env){
    if(items->size() < 2){
		write_error("operator manje-ili-jednako, prosleđeno manje od dve vrednosti.");
		return NIL;
    }

    moVal *first = items->at(0).get();
    if(first == nullptr){
        write_error("operator manje-ili-jednako, prva vrednost je nevalidna.");
        return NIL;
    }

    switch(first->getType()){
        case MO_TYPE::MO_STRING:
            return _mo_compare_string(items, std::less_equal<size_t>());
        case MO_TYPE::MO_NUMBER:
            return _mo_compare_number(items, std::less_equal<double>());
        default:
            write_error("operator manje-ili-jednako, prva vrednost nije broj ni tekst.");
            return NIL;
    }

}




/*
    checks if all given truth values are true
*/
moValPtr mo_and(moListPtr items, moEnv& env){
    if(items->size() == 0){
		write_error("operator i, vrednosti nisu prosleđene.");
		return NIL;
    }

    for(int i=0;i<items->size();i++){
        if(!items->at(i)->isTrue()){
            return FALSE;
        }
    }

    return TRUE;
}


/*
    checks if at least one of given truth values is true
*/
moValPtr mo_or(moListPtr items, moEnv& env){
    if(items->size() == 0){
		write_error("operator i, vrednosti nisu prosleđene.");
		return NIL;
    }

    for(int i=0;i<items->size();i++){
        if(items->at(i)->isTrue()){
            return TRUE;
        }
    }

    return FALSE;
}




/*
    prints values into LOG_STREAM,
        returns NIL
*/
moValPtr mo_print(moListPtr items, moEnv& env){
    for(int i=0;i<items->size();i++){
        write_raw(items->at(i)->print());
        if(i==items->size()-1)
            write_raw("\n");
    }


    return NIL;
}




//
//  NOT-EVALUATED
//

/*
    Syntax:
        if condition if-true if-false

    Evaluates condition and if true evaluates if-true, if not if-false;
        if-false can be excluded, if condition is false in that case NIL is returned.
    
*/
moValPtr mo_if(moListPtr items, moEnv &env){
    if(items->size() < 3){
        write_error("operator ako, nedovoljno argumenata prosleđeno.");
        return NIL;
    }else if(items->size() > 4){
        write_error("operator ako, previše argumenata prosleđeno.");
        return NIL;
    }
    //eval condition
	moValPtr condition = eval(items->at(1), env);
	if(condition->isTrue()){//true
		return eval(items->at(2), env);
    }
	else if(items->size() == 4){//false, if-false is provided
		return eval(items->at(3), env);
    }else{//false, if-false is not provided
        return NIL;
    }
}


}