#ifndef SWW_CHAINABLE_H
#define SWW_CHAINABLE_H

#include "Core/Meta.h"
#include <vector>
#include "Core/DualMap.h"

#include "core/SwwNsCore.h"

SWW_CORE_NAMESPACE_BEGIN

class SWWCORESHARED_EXPORT Chainable : public Meta
{

public:
    Chainable();
    virtual ~Chainable(){}
    std::string getPath() const;
    bool addNext(Chainable* c);
    void removeNext(Chainable* c);
    void clearNext();
    bool addPrev(Chainable* c);
    void removePrev(Chainable* c);
    void clearPrev();

    Chainable* getPrev0()const;
    int prevCount()const{return _prev.size();}
    int nextCount()const{return _next.size();}

protected:
    std::vector<Chainable*> _prev;
    std::vector<Chainable*> _next;
};

SWW_CORE_NAMESPACE_END

#endif // SWW_CHAINABLE_H
