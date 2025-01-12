#include "KnobObject.h"

#include "Core/SwwMacro.h"
#include "Core/PlugBase.h"
#include "Core/JsonDeserializer.h"
#include "Core/JsonSerializer.h"
#include "Knob/CommandStack.h"

KnobObject::KnobObject(Sww::Core::PlugBase *plug, QWidget *parent) :
    QWidget(parent),_plugBase(plug),_name(plug->getName())
{
    //empty
}

KnobObject::~KnobObject()
{
    //empty
}

//bool KnobObject::exec()
//{
//    Q_ASSERT(!_plugBase->isOutput());
//    return  _plugBase->getNode()->execute(_plugBase);
//}


namespace SwwKnob {

UndoCommand::UndoCommand(Sww::Core::PlugBase* plug):_plug(plug),_first(true)
{
    Sww::Core::Node* node = dynamic_cast<Sww::Core::Node*>(plug->_node);
    _node = node->parentNode();
}

void UndoCommand::undo(){
    using namespace Sww::Core;
    SwwJson j = SwwJson::parse(_undoJson);
    Sww::Core::JsonDeserializer jd;
    Imath::Box2f dummy;
    jd.deserialize(j,_node,dummy);
    //SWW_DEBUG_VALUE(_undoJson);
}

void UndoCommand::redo(){
    using namespace Sww::Core;
    if(_first){
        _first = false;
        return;
    }

    SwwJson j = SwwJson::parse(_redoJson);
    Sww::Core::JsonDeserializer jd;
    Imath::Box2f dummy;
    jd.deserialize(j,_node,dummy);
    //SWW_DEBUG_VALUE(_redoJson);
}

void UndoCommand::storeUndoValue(){
    Sww::Core::JsonSerializer js;
    _undoJson = js.serialize(_plug);
}

void UndoCommand::push(){
    Sww::Core::JsonSerializer js;
    _redoJson = js.serialize(_plug);
    SwwKnob::CommandStack::pushCommand(this);
}


}//ns




