#include "CommandStack.h"
#include "Core/SwwAssert.h"

namespace SwwKnob {

QUndoStack* CommandStack::_commandStack = 0;

void CommandStack::init(){
    _commandStack = new QUndoStack();
}

void CommandStack::deinit(){
    delete _commandStack;
}

void CommandStack::pushCommand(QUndoCommand *cmd){
    Q_ASSERT_X(cmd,"push command","null command.");
    _commandStack->push(cmd);
}

void CommandStack::undo(){
    _commandStack->undo();
}

void CommandStack::redo(){
    _commandStack->redo();
}

void CommandStack::printCommnadStack(){
    QString text = "command stack";
    for(int i = 0;i < _commandStack->count();i++){
        text += "\n" + _commandStack->text(i);
    }

    SWW_DEBUG(text.toStdString());
}

}//ns
