#include "Chainable.h"
#include "Core/VectorHelper.h"

SWW_CORE_NAMESPACE_BEGIN

Chainable::Chainable()
{
}

bool Chainable::addNext(Chainable *c){
    if(vectorContains(_next,c)) return false;
    _next.push_back(c);
    c->_prev.push_back(this);
    return true;
}

void Chainable::removeNext(Chainable *c){
    vectorErase(_next,c);
    vectorErase(c->_prev,this);
}

void Chainable::clearNext(){
    for(auto c:_next){
        vectorErase(c->_prev,this);
    }
    _next.clear();
}

bool Chainable::addPrev(Chainable *c){
    if(vectorContains(_prev,c)) return false;
    _prev.push_back(c);
    c->_next.push_back(this);
    return true;
}

void Chainable::removePrev(Chainable *c){
    vectorErase(_prev,c);
    vectorErase(c->_next,this);
}

void Chainable::clearPrev(){
    for(auto c:_prev){
        vectorErase(c->_next,this);
    }
    _prev.clear();
}

std::string Chainable::getPath()const{

    Chainable* prev = getPrev0();

    if(!prev)return "/";
    std::string result = getName();

    while(prev){
        const std::string& parentName = prev->getName();
        result =  parentName + "/" +result;
        prev = prev->getPrev0();
        if(!prev)result = "/" + result;
    }

    return result;
}

Chainable* Chainable::getPrev0()const{
    if(_prev.empty())return 0;
    return _prev[0];
}

SWW_CORE_NAMESPACE_END
