#include "App/UnitTest.h"
#include <stdlib.h>

#include "Core/PlugType.h"
#include "Core/Node.h"

using namespace Sww::Core;

class Hoge : public Node{
public:
    Hoge(){}
    virtual bool initialize(){
        _in = new FloatPlug("in",1);
        _out = new FloatPlug("out",1);
        _out->setOutput(true);
        addPlug(_in);
        addPlug(_out);

        return true;
    }

    virtual bool execute(const Sww::Core::PlugBase *plug){
        return true;
    }

//private:
    Sww::Core::FloatPlug* _in;
    Sww::Core::FloatPlug* _out;
};


namespace SwwTest {

UnitTest::UnitTest()
{
    //Node::Ptr node = Node::Ptr(new Node());

    Hoge* hoge0 = new Hoge();
    Hoge* hoge1 = new Hoge();

    hoge0->initialize();
    hoge1->initialize();

    Node::connect(hoge0->_out,hoge1->_in);
    Node::disconnect(hoge0->_out,hoge1->_in);

    exit(57);
}

}//ns
