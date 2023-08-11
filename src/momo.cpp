#include "momo.h"
#include "tokenizer.h"
#include "reader.h"


#include <iostream>


namespace momo{






}


int main(){
    // std::string input = "[prostorija zgrada  : opis \"sasvim obicna zgrada\"  : pristupacna JESTE; : udaljenost 123.45;]";
    // momo::Tokenizer toki(input);
    // while(!toki.isEnd()){
    //     std::cout << toki.next() << "," << "\n";
    // }

    momo::moEnv env;
    // std::string str = "[ - 26 5 ]";
    // momo::moValPtr res = momo::readString(str);
    // momo::moValPtr eres = momo::eval(res, env);
    // std::cout << eres->print();
    // std::cout << momo::LOG_STREAM.str();

    momo::moListPtr res = momo::readFile("staznam.momo");

    for(int i=0;i<res->size();i++){
        momo::moValPtr eres = momo::eval(res->at(i), env);
        std::cout << eres->print();
        std::cout << momo::LOG_STREAM.str();
        
    }



    return 0;
}