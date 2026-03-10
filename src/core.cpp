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


moValPtr mo_product(moListPtr items, moEnv& env){
    if(items->size() == 0){
        write_error("množenje, vrednosti nisu prosleđene.");
        return NIL;
    }
    if(items->at(0)->getType() != MO_TYPE::MO_NUMBER){
        write_error("množenje, prva vrednost nije broj.");
        return NIL;
    }
    double result = std::static_pointer_cast<moNumber>(items->at(0))->getValue();
    for(int i=1;i<items->size();i++){
        switch(items->at(i)->getType()){
            case MO_TYPE::MO_NUMBER:
                result *= std::static_pointer_cast<moNumber>(items->at(i))->getValue();
                break;
            case MO_TYPE::MO_STRING:
                result *= string_to_number(items->at(i));
                break;
            default:
                write_error("množenje, vrednost nije broj ili tekst.");
                return NIL;
        }
    }
    return moValPtr(new moNumber(result));
}


moValPtr mo_quotient(moListPtr items, moEnv& env){
    if(items->size() == 0){
        write_error("deljenje, vrednosti nisu prosleđene.");
        return NIL;
    }
    if(items->at(0)->getType() != MO_TYPE::MO_NUMBER){
        write_error("deljenje, prva vrednost nije broj.");
        return NIL;
    }
    double result = std::static_pointer_cast<moNumber>(items->at(0))->getValue();
    for(int i=1;i<items->size();i++){
        double divisor;
        switch(items->at(i)->getType()){
            case MO_TYPE::MO_NUMBER:
                divisor = std::static_pointer_cast<moNumber>(items->at(i))->getValue();
                break;
            case MO_TYPE::MO_STRING:
                divisor = string_to_number(items->at(i));
                break;
            default:
                write_error("deljenje, vrednost nije broj ili tekst.");
                return NIL;
        }
        if(divisor == 0.0){
            write_error("deljenje nulom.");
            return NIL;
        }
        result /= divisor;
    }
    return moValPtr(new moNumber(result));
}


moValPtr mo_truth_value(moListPtr items, moEnv& env){
    if(items->size() == 0){
        write_error("istinitost, vrednost nije prosleđena.");
        return NIL;
    }
    return moValPtr(new moBool(items->at(0)->isTrue()));
}


moValPtr mo_not_truth_value(moListPtr items, moEnv& env){
    if(items->size() == 0){
        write_error("negacija, vrednost nije prosleđena.");
        return NIL;
    }
    return moValPtr(new moBool(!items->at(0)->isTrue()));
}


moValPtr mo_equals(moListPtr items, moEnv& env){
    if(items->size() < 2){
        write_error("jednakost, potrebne su dve vrednosti.");
        return NIL;
    }
    return moValPtr(new moBool(items->at(0)->equals(items->at(1))));
}


moValPtr mo_strict_equals(moListPtr items, moEnv& env){
    if(items->size() < 2){
        write_error("stroga jednakost, potrebne su dve vrednosti.");
        return NIL;
    }
    // Same pointer = same object
    return moValPtr(new moBool(items->at(0).get() == items->at(1).get()));
}


static double _to_number(moValPtr val){
    if(val->getType() == MO_TYPE::MO_NUMBER)
        return std::static_pointer_cast<moNumber>(val)->getValue();
    write_error("poređenje, vrednost nije broj.");
    return 0;
}

moValPtr mo_greater(moListPtr items, moEnv& env){
    if(items->size() < 2){
        write_error("poređenje, potrebne su dve vrednosti.");
        return NIL;
    }
    if(items->at(0)->getType() != MO_TYPE::MO_NUMBER || items->at(1)->getType() != MO_TYPE::MO_NUMBER){
        write_error("poređenje, vrednosti moraju biti brojevi.");
        return NIL;
    }
    double a = std::static_pointer_cast<moNumber>(items->at(0))->getValue();
    double b = std::static_pointer_cast<moNumber>(items->at(1))->getValue();
    return moValPtr(new moBool(a > b));
}


moValPtr mo_less(moListPtr items, moEnv& env){
    if(items->size() < 2){
        write_error("poređenje, potrebne su dve vrednosti.");
        return NIL;
    }
    if(items->at(0)->getType() != MO_TYPE::MO_NUMBER || items->at(1)->getType() != MO_TYPE::MO_NUMBER){
        write_error("poređenje, vrednosti moraju biti brojevi.");
        return NIL;
    }
    double a = std::static_pointer_cast<moNumber>(items->at(0))->getValue();
    double b = std::static_pointer_cast<moNumber>(items->at(1))->getValue();
    return moValPtr(new moBool(a < b));
}


moValPtr mo_greater_or_equals(moListPtr items, moEnv& env){
    if(items->size() < 2){
        write_error("poređenje, potrebne su dve vrednosti.");
        return NIL;
    }
    if(items->at(0)->getType() != MO_TYPE::MO_NUMBER || items->at(1)->getType() != MO_TYPE::MO_NUMBER){
        write_error("poređenje, vrednosti moraju biti brojevi.");
        return NIL;
    }
    double a = std::static_pointer_cast<moNumber>(items->at(0))->getValue();
    double b = std::static_pointer_cast<moNumber>(items->at(1))->getValue();
    return moValPtr(new moBool(a >= b));
}


moValPtr mo_less_or_equals(moListPtr items, moEnv& env){
    if(items->size() < 2){
        write_error("poređenje, potrebne su dve vrednosti.");
        return NIL;
    }
    if(items->at(0)->getType() != MO_TYPE::MO_NUMBER || items->at(1)->getType() != MO_TYPE::MO_NUMBER){
        write_error("poređenje, vrednosti moraju biti brojevi.");
        return NIL;
    }
    double a = std::static_pointer_cast<moNumber>(items->at(0))->getValue();
    double b = std::static_pointer_cast<moNumber>(items->at(1))->getValue();
    return moValPtr(new moBool(a <= b));
}


// Short-circuit: receives unevaluated args (ne_operator)
moValPtr mo_and(moListPtr items, moEnv& env){
    if(items->size() < 3){
        write_error("logičko i, potrebne su dve vrednosti.");
        return NIL;
    }
    // items->at(0) is the operator symbol "&", args start at 1
    moValPtr left = eval(items->at(1), env);
    if(!left->isTrue())
        return left;
    return eval(items->at(2), env);
}


// Short-circuit: receives unevaluated args (ne_operator)
moValPtr mo_or(moListPtr items, moEnv& env){
    if(items->size() < 3){
        write_error("logičko ili, potrebne su dve vrednosti.");
        return NIL;
    }
    // items->at(0) is the operator symbol "|", args start at 1
    moValPtr left = eval(items->at(1), env);
    if(left->isTrue())
        return left;
    return eval(items->at(2), env);
}


// ne_operator: [označi ime vrednost]
// items->at(0) is "označi", at(1) is name symbol, at(2) is unevaluated value
moValPtr mo_oznaci(moListPtr items, moEnv& env){
    if(items->size() < 3){
        write_error("označi, potrebni su ime i vrednost.");
        return NIL;
    }
    if(items->at(1)->getType() != MO_TYPE::MO_SYMBOL){
        write_error("označi, prvo mora biti ime.");
        return NIL;
    }
    std::string name = items->at(1)->print();
    moValPtr value = eval(items->at(2), env);
    env.bindVal(name, value);
    return value;
}


// ne_operator: [ako uvjet onda] or [ako uvjet onda inače]
// items->at(0) is "ako", at(1) is condition, at(2) is then-branch, at(3) is optional else-branch
moValPtr mo_ako(moListPtr items, moEnv& env){
    if(items->size() < 3){
        write_error("ako, potrebni su uvjet i vrednost.");
        return NIL;
    }
    moValPtr condition = eval(items->at(1), env);
    if(condition->isTrue()){
        return eval(items->at(2), env);
    }else{
        if(items->size() >= 4)
            return eval(items->at(3), env);
        return NIL;
    }
}


// ne_operator: [funkcija [args] body...]
// items->at(0) is "funkcija", at(1) is arg list, at(2..) is body
moValPtr mo_funkcija(moListPtr items, moEnv& env){
    if(items->size() < 3){
        write_error("funkcija, potrebni su argumenti i telo.");
        return NIL;
    }
    if(items->at(1)->getType() != MO_TYPE::MO_LIST){
        write_error("funkcija, argumenti moraju biti lista.");
        return NIL;
    }
    moListPtr args = std::static_pointer_cast<moList>(items->at(1));
    moListPtr body(new moList);
    for(size_t i = 2; i < items->size(); i++){
        body->insert(items->at(i));
    }
    auto fn = std::make_shared<moFunction>("", args, body);
    fn->setClosure(env.getAllBindings());
    return fn;
}


}