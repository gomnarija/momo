#include "momo.h"


namespace momo{


/*
    arguments evaluated before beign passed
*/
std::map<std::string,
	moValPtr (*)(moListPtr, moEnv&)> operators = {
		{"+",&mo_sum},
		{"-",&mo_difference},	
		{"*",&mo_product},	
		{"/",&mo_quotient},
		{"?",&mo_truth_value},
		{"!",&mo_not_truth_value},
		{"=",&mo_equals},
		{">",&mo_greater},
		{"<",&mo_less},
		{">=",&mo_greater_or_equals},
		{"<=",&mo_less_or_equals},
		// {"==",&la_real_equals},
		{"&",&mo_and},
		{"|",&mo_or},
		{"ispiši", &mo_print}
	};


/*
    arguments NOT evaluated before beign passed
*/
std::map<std::string,
	moValPtr (*)(moListPtr, moEnv&)> ne_operators=
	{	
		{"ako",&mo_if}
		// {"set",&la_set},
		// {"fset",&la_fset},
		// {"val",&la_val},
		// {"function",&la_function},
		// {"let",&la_let},
		// {"quit",&la_quit}
	};


moValPtr env_search(moValPtr symbol, moEnv &env){
	moEnv *curr = &env;
	while(curr != nullptr){
		moValPtr res = curr->getVal(symbol->print());
		if(res->getType() == MO_TYPE::MO_NIL){
			curr = curr->getUpperEnv();
        }else{			
            return res;
        }
	}

	return NIL;
}



/*
    evaluate single value
*/
moValPtr eval_atom(moValPtr atom,moEnv &env){
	switch(atom->getType())
	{
		//self-eval
		case MO_TYPE::MO_NUMBER:
		case MO_TYPE::MO_STRING:
        case MO_TYPE::MO_NIL:
			return atom;
		case MO_TYPE::MO_SYMBOL:
			return env_search(atom,env);
		case MO_TYPE::MO_FUNCTION:
            //TODO
		default:
			write_error("nevalidan atom.");
			return NIL;
	}
}


moValPtr eval_function(moFunctionPtr function,moListPtr args,moEnv &env){
	moEnv localEnv(&env);
    //add function arguments to local envirionment
	moListPtr funArgument = function->getArgs();
	for(int i=0;i<funArgument->size();i++){
		localEnv.insertVal((funArgument->at(i))->print(),args->at(i));
	}

    //evaluate body, return last value
	moListPtr funBody = function->getBody();
	for(int i=0;i<funBody->size();i++){
        if(i != funBody->size()-1){
		    eval(funBody->at(i), localEnv);
        }else{
		    return eval(funBody->at(i), localEnv);
        }
	}


	return NIL;
}


moValPtr eval_list(moValPtr list, moEnv &env){
    if(list->getType() != MO_TYPE::MO_LIST){
        write_error("nevalidna lista.");
        return NIL;
    }

	moListPtr  listPtr = std::static_pointer_cast<moList>(list);
    //TODO: maybe change this
	if(listPtr->size() == 0){
		return NIL;
    }

    //check operator type
	moValPtr   op;
	op = listPtr->at(0);
	if(op->getType() != MO_TYPE::MO_SYMBOL && op->getType() != MO_TYPE::MO_FUNCTION){
        write_error("nevalidan prefiks.");
		return NIL;
    }

	//no eval operator
	if(ne_operators.find(op->print()) != ne_operators.end())
		return ne_operators.at(op->print())(std::static_pointer_cast<moList>(list), env);



	//eval args and add them to args list
	moListPtr   args(new moList());
	moValPtr    curr;
	for(int i=1;i<listPtr->size();i++){
		curr = listPtr->at(i);
		if(curr->getType() == MO_TYPE::MO_LIST){
			args->insert(eval_list(curr, env));
        }
		else{
			args->insert(eval_atom(curr,env));
        }
    }

	//operator
	if(operators.find(op->print()) !=operators.end()){
		return operators.at(op->print())(args,env);
    }

    //functions
	{
		moValPtr potFun = env_search(op,env);
		if(potFun->getType() == MO_TYPE::MO_FUNCTION)
			return eval_function(std::static_pointer_cast<moFunction>(potFun),args,env);
	}

    return NIL;
}


moValPtr eval(moValPtr ast, moEnv &env){
    if(ast == nullptr){
        write_error("nevalidna vrednost.");
        return NIL;
    }

    switch(ast->getType()){
        case MO_TYPE::MO_NIL:    
        case MO_TYPE::MO_NUMBER:
		case MO_TYPE::MO_STRING:
		case MO_TYPE::MO_SYMBOL:
			return eval_atom(ast,env);
		case MO_TYPE::MO_LIST:
			return eval_list(ast, env);
		case MO_TYPE::MO_FUNCTION:
		default:
        	write_error("nevalidna vrednost.");
			return NIL; 
    }
}


}