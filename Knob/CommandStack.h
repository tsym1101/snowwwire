#ifndef SWW_COMMANSWWTACK_H
#define SWW_COMMANSWWTACK_H
#include <QUndoStack>
#include "KnobAPI.h"

namespace SwwKnob {

class SWWKNOBSHARED_EXPORT CommandStack
{
public:
    CommandStack(){}
    static void pushCommand(QUndoCommand *cmd);
    static void undo();
    static void redo();
    static void init();
    static void deinit();
    static void printCommnadStack();

private:
    static QUndoStack* _commandStack;
};
}//ns

#endif // SWW_COMMANSWWTACK_H
