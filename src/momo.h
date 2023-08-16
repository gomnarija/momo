#ifndef MOMO_H
#define MOMO_H

#include	<memory>
#include    <vector>
#include	<map>
#include    <string>
#include 	<sstream>

namespace momo{



enum MO_TYPE{
	MO_NIL,
	MO_NUMBER,
	MO_STRING,
	MO_SYMBOL,
	MO_LIST,
	MO_FUNCTION
};


struct moVal{
	virtual std::string print() = 0;
	virtual MO_TYPE getType() = 0;
	virtual bool isTrue() = 0;
	virtual bool equals(std::shared_ptr<moVal> other) = 0;
};

typedef	std::shared_ptr<moVal> moValPtr;


struct moNumber : moVal{
	moNumber();
	moNumber(double value);
	moNumber(std::string value);

    double	        getValue() ;
	std::string     print()  override;
	 MO_TYPE    getType() override;
	 bool      isTrue() override;
	 bool      equals (moValPtr other)override;

private:
	double		value;
};


struct moString : moVal{
	moString();
	moString(std::string value);

	std::string	getValue() ;
	std::string print() override;

	 MO_TYPE    getType() override;
	 bool      isTrue() override ;
	 bool      equals (moValPtr other) override;

private:
	std::string	value;
};


struct moList : moVal{
	
	moList();
	moList(std::vector<moValPtr> list);


	void 	    insert(moValPtr val);
	size_t 	    size();
	moValPtr    at( size_t);


	std::string     print() override;	
	 MO_TYPE   getType() override;
	 bool      isTrue() override;
	 bool      equals(std::shared_ptr<moVal> other) override;

private:
	std::vector<moValPtr>	items;
};


struct moSymbol : moVal{
	moSymbol();
	moSymbol(std::string value);

	std::string         print()override;
	MO_TYPE       		getType() override;
	bool            	isTrue() override;
	bool            	equals (moValPtr other) override;

private:
	std::string 	value;
};

typedef	std::shared_ptr<moNumber>
		moNumberPtr;
typedef	std::shared_ptr<moString>
		moStringPtr;
typedef	std::shared_ptr<moSymbol>
		moSymbolPtr;
typedef	std::shared_ptr<moList>
		moListPtr;

struct moFunction : moVal{
	moFunction();
	moFunction(std::string name, moListPtr args, moListPtr body);

	std::string print() override;
	 MO_TYPE getType() override;

	std::string	getName();
	moListPtr	getArgs();
	moListPtr	getBody();

private:
	std::string	    name;
	moListPtr	    args;
	moListPtr	    body;
};
typedef	std::shared_ptr<moFunction>
		moFunctionPtr;


struct _moNil : moVal{
	_moNil();	
	static moValPtr Nil();	

	std::string     print() override;
	MO_TYPE         getType() override;
    bool            isTrue() override;
	bool            equals (moValPtr other) override;
};

static _moNil moNil;
static std::shared_ptr<moVal> NIL = 
    std::make_shared<_moNil>(moNil);



struct moEnv{
	moEnv();
	moEnv(moEnv*);
	moEnv(moEnv*, std::map<std::string, moValPtr>);
	

	moValPtr	getVal(std::string);
	void		setVal(std::string, moValPtr);
	void		insertVal(std::string, moValPtr);
	void		fsetTal(std::string, moValPtr);
		
	moEnv*		getUpperEnv();

private:
	std::map<std::string, moValPtr>	symbolValueMap;
	moEnv*							upperEnv;
};




static moNumber moTrue(1);
static std::shared_ptr<moVal> TRUE = 
    std::make_shared<moNumber>(moTrue);

static moNumber moFalse(0);
static std::shared_ptr<moVal> FALSE = 
    std::make_shared<moNumber>(moFalse);

//error handling
extern std::string				LOG_FILENAME;
extern uint32_t					LOG_LINE;
extern std::stringstream		LOG_STREAM;
void							write_error(const std::string &);
void							write_warning(const std::string &);
void							write_debug(const std::string &);
void							write_raw(const std::string &);
void							write_raw_line(const std::string &);



//eval
moValPtr 			eval(moValPtr, moEnv&);




//core
moValPtr 	mo_sum(moListPtr, moEnv&);
moValPtr 	mo_difference(moListPtr, moEnv&);
moValPtr 	mo_product(moListPtr, moEnv&);
moValPtr	mo_quotient(moListPtr, moEnv&);
moValPtr	mo_truth_value(moListPtr, moEnv&);
moValPtr	mo_not_truth_value(moListPtr, moEnv&);
moValPtr	mo_equals(moListPtr, moEnv&);
moValPtr	mo_greater(moListPtr, moEnv&);
moValPtr	mo_less(moListPtr, moEnv&);
moValPtr	mo_greater_or_equals(moListPtr, moEnv&);
moValPtr	mo_less_or_equals(moListPtr, moEnv&);
moValPtr	mo_and(moListPtr, moEnv&);
moValPtr	mo_or(moListPtr, moEnv&);

moValPtr	mo_if(moListPtr, moEnv&);


moValPtr	mo_print(moListPtr, moEnv&);

//utils, TODO: move to another header
double			string_to_number(const std::string &);
double			string_to_number(moValPtr);
std::string		string_remove_quotes(const std::string &);
std::string 	get_file_name(const std::string &path);
size_t          ustrlen(const std::string &);
size_t			ustrlen(const char *s);



}

#endif